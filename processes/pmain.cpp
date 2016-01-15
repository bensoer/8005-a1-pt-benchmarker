//
// Created by bensoer on 05/01/16.
//

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <gmp-x86_64.h>
#include <fcntl.h>
#include <fcntl.h>
#include <sys/time.h>
#include "../common/argparcer.h"
#include "WorkerProcess.h"
#include "../common/Task.h"
#include "../common/logger.h"

using namespace std;

/** MSGSIZE - The size of a message sent from the parent to a child process **/
const int MSGSIZE = 23;
/** STATUSMSGSIZE - The size of the message sent from the child to the parent process **/
const int STATUSMSGSIZE = 14;

int main(int argc, char *argv[]){

    ArgParcer parcer;
    int number = parcer.GetTagVal("-n", argv, argc);
    if(number == -1){
        cout << "A Parameter is Invalid or Not Included" <<endl;
        cout << "Expected Use: tmain.cpp -n <number>"<< endl;
        return 1;
    }else{
        cout << "Parameter Found" << endl;
    }


    char inbuf[MSGSIZE];
    int pipeConnectionToChild[2];

    int pipeConnectionToParent[2];


    if (pipe(pipeConnectionToChild) < 0)
    {
        cout << "Pipe Establishment To Child  Failed" << endl;
        exit(1);
    }else{
        cout << "Pipe Establishment To Child Successful" << endl;
    }

    if (fcntl (pipeConnectionToChild[0], F_SETFL, O_NDELAY) < 0){
        cout << "Setting Non Blocking Pipe To Child Failed" << endl;
        exit(1);
    }

    if (pipe(pipeConnectionToParent) < 0)
    {
        cout << "Pipe Establishment To Parent  Failed" << endl;
        exit(1);
    }else{
        cout << "Pipe Establishment To Parent Successful" << endl;
    }

    if (fcntl (pipeConnectionToParent[0], F_SETFL, O_NDELAY) < 0){
        cout << "Setting Non Blocking Pipe To Parent Failed" << endl;
        exit(1);
    }


    //PARENT - Setup Original Tasks
    cout << "Loading Original Tasks" << endl;
    //find all numbers from 1 to n
    for(unsigned int i = 0; i < number; i++){

        string strNumber = to_string(number);
        string strDivisor = to_string(i);

        while(strNumber.length() < 10){
            strNumber = "0" + strNumber;
        }

        while(strDivisor.length() < 10){
            strDivisor = "0" + strDivisor;
        }

        string message = "{" + strNumber + "." + strDivisor + "}";
        write(pipeConnectionToChild[1], message.c_str(), message.length());

    }

    //PARENT START THE TIMER TO FIND THE PRIMES
    struct timeval initiationTime;
    gettimeofday(&initiationTime,NULL);

    //PARENT - create the child processes

    pid_t children[5];

    pid_t pid;
    for (unsigned int i = 0; i < 5; i++){
         if((pid = fork()) == 0){
             //this is now the child process

             //CHILD - create worker process and execute
             WorkerProcess * worker = new WorkerProcess(pipeConnectionToChild, pipeConnectionToParent, i);
             worker->start();

             delete(worker);

             cout << "About to Return in Child" << endl;
             //isolate the child process from the parent so that it never runs parent content
             return 0;

         }else{
             children[i] = pid;
         }
    }


    //PARENT - check for idle processes

    //Child state holds state of child whether it is idle or not. 1 means it is idle, 0 means it is not
    int childrenstate[5] = {0};
    bool validRead = false;

    while(1){

        //Message Structure: {<indexID>:<status>}
        // if <indexID> = T then it is a primeTime
        //Status can be IDLE or BUSY


        char inbuf2[STATUSMSGSIZE + 1];
        long bytesRead = read (pipeConnectionToParent[0], inbuf2, STATUSMSGSIZE);
        if(bytesRead > 0){
            cout << "Parent Has Made A Valid Read" << endl;
            validRead = true;

            inbuf2[STATUSMSGSIZE] = '\0';
        }else{
            cout << "Parent Has Made An Invalid Read" << endl;
            validRead = false;
        };

        if(validRead){
            string strBuffer(inbuf2);

            //cout << "Full String Read In " << inbuf2 << "  |  " << strBuffer << endl;

            string status = strBuffer.substr(strBuffer.find(':') + 1 , strBuffer.find('}') - strBuffer.find(':') - 1);

            string index = strBuffer.substr(1, strBuffer.find(':')-1);
            //cout << "String of Index: " << index << endl;
            string index2 = index;
            //if index is T then were dealing with a PrimeTime

            int intIndex;
            if(index2.compare("T")==0){
                //cout << "GOT TIME OF PRIME" << endl;
                //cout << status << endl;

                long primeTime = stoi(status);
                long timeDifference = primeTime - initiationTime.tv_usec;

                string message = "Prime Number Found In " + to_string(timeDifference) + "microsec";
                Logger::logToFile(message);
            }else{
                intIndex = stoi(index2, nullptr, 10);
                //cout << "Status of Child Process Is: Index: " << intIndex << " Status: " << status << endl;

                if(status.compare("IDLE000000") == 0){
                  //  cout << "Process In Index: " << intIndex << " Is IDLE" << endl;
                    childrenstate[intIndex] = 1;


                }else{
                   // cout << "Process In Index: " << intIndex << " Is BUSY" << endl;
                    childrenstate[intIndex] = 0;
                }
            }


        }else{
            cout << "Parent Assessing If All Children Are IDLE" << endl;
            //check if all child processes are now idle
            bool allIdle = true;
            for(unsigned int i = 0 ; i < (sizeof(childrenstate)/sizeof(children[0])) ; i++){
                if(childrenstate[i] == 0){
                    cout << "Found Non IDLE Child Index: " << i << endl;
                    allIdle = false;
                    break;
                }
            }


            //if all are idle commence shutdown
            if(allIdle){

                //We should have a final check here to confirm the pipe is empty before closing everything

                cout << "ALL CHILDREN ARE IDLE. BREAKING WHILE" << endl;

                for(unsigned int i = 0; i < 5; i++){
                    string message = "{TERMINATE}000000000000";
                    write(pipeConnectionToChild[1], message.c_str(), message.length());
                }

                break;

            //if they are not, get off the CPU and give someone else a chance
            }else{
                cout << "All Processes Are Not Idle. Releasing CPU" << endl;

                struct timespec waitTime;
                waitTime.tv_nsec = 0;
                waitTime.tv_sec = 0;

                int result = nanosleep(&waitTime, NULL);
                if(result != 0){
                    cout << pthread_self() << " ERROR. Sleep/Release CPU Failure" << endl;
                }

            }
        }

    }


    //PARENT - Wait For Children To Terminate

    waitpid(children[0], NULL, 0);
    waitpid(children[1], NULL, 0);
    waitpid(children[2], NULL, 0);
    waitpid(children[3], NULL, 0);
    waitpid(children[4], NULL, 0);

    //PARENT - Close All Pipes
    close(pipeConnectionToChild[0]);
    close(pipeConnectionToChild[1]);
    close(pipeConnectionToParent[0]);
    close(pipeConnectionToParent[1]);


    cout << "About To Return in Parent" << endl;

    return 0;
}