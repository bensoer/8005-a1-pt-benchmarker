//
// Created by bensoer on 09/01/16.
//

#include <unistd.h>
#include "WorkerProcess.h"
#include "../common/logger.h"
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <gmp.h>

using namespace std;

/**
 * constructor initializes the WorkerProcess before the start() funciton is called. It is passed the pipe connecitons
 * for the WorkerProcess and the parent process to communicate bidirecitonaly. It is also passed the index ID the
 * WorkerProcess is located in the parent process so as to minimize the amount of data needing to be sent in the pipe
 * when identifying the source of the messages being passed from the WorkerProcess to the parent
 * @param pipeConnectionToChild int* - A pointer to the pipe connection for the parent to communicate with the WorkerProcess
 * @param pipeConnectionToParent int* - A pointer to the pipe conneciton for the WorkerProcess to communicate with the parent
 * @param indexID int - The ID of the WorkerProcess in terms of its index position in the array storing state information
 * about this WorkerProcess. Needed for identification when communicating back to the parent
 */
WorkerProcess::WorkerProcess(int* pipeConnectionToChild, int* pipeConnectionToParent, int indexID) {
    this->pipeConnectionToChild = pipeConnectionToChild;
    this->pipeConnectionToParent = pipeConnectionToParent;
    this->indexID = indexID;
}
/**
 * parseValueFromWork is a private helper function that parses out of the currentWork string the N value and D value
 * and then converts them into long values so that they can be calculated
 */
void WorkerProcess::parseValuesFromWork() {

    string strN = this->currentWork.substr(1,10);
    string strD = this->currentWork.substr(this->currentWork.find('.') + 1, 10);

    //cout << "N : " << strN << endl;
    //cout << "D : " << strD << endl;

    this->n = stoi(strN);
    this->d = stoi(strD);
}

/**
 * start is the main entry point for the WorkerProcess. It is a nearly infinite while loop which checks for work from
 * and at which point then processes the work pending it is allowed to continue. Being allowed to continue varies as to
 * whether the WorkerProcess should be terminaitng or not. If The WorkerPRocess is not allowed to continue working, it
 * will break out of the while loop and return
 */
void WorkerProcess::start() {

    while(1){

        cout << getpid() << " Checking For More Work" << endl;
        this->checkForMoreWork();
        cout << getpid() << " Received Work: " << this->currentWork << endl;


        if(this->continueWorking){
            this->parseValuesFromWork();

            struct timeval startTime;
            struct timeval endTime;
            struct timeval primeTime;

            cout << getpid() << " Recieved More Work" << endl;
            gettimeofday(&startTime,NULL);

            /* -- Initialization For Calculations -- */

            mpz_t n;
            mpz_t tmp;
            mpz_t d;

            mpz_init(tmp);
            mpz_init(d);
            mpz_init(n);

            mpz_set_ui(n, this->n);
            mpz_set_ui(d, this->d);

           // gmp_printf("%Zd\n", n);
           // gmp_printf("%Zd\n", d);

            /* -- -- */


            if(mpz_divisible_p(n,d)){
                //d at this point is a divisable number
                //cout << "FOUND A DIVISIBLE" << endl;
                gmp_printf("%Zd is divisible by %Zd\n", n, d);

                mpz_divexact(tmp, n, d);
                mpz_swap(tmp, n);

                //if the n is greater then 1 and d (the divisble number) does not equal 1, there might be
                //more to do
                if(mpz_cmp_si (n, 1) && (mpz_get_ui(d) != 1)){
                    //put n into the array to be tried again
                    //cycle through and generate all of the d values
                    for(unsigned int i = 0; i < mpz_get_ui(n) ; i++){

                        long intN = mpz_get_ui(n);
                        //write to the pipe for the child processes
                        string strNumber = to_string(intN);
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
                    //else you have found some kind of prime factor
                    //this means the d could equal to 1 or n is greater then 1
                }else{

                    //gmp_printf("FOUND PRIME FACTOR: %Zd\n", n);

                    long threadid = pthread_self();
                    string message = to_string(threadid) + " has found a Prime Number Factor Of: " + to_string(mpz_get_ui(n));
                    Logger::logToFile(message);

                    gettimeofday(&primeTime,NULL);
                    string strPrimeTime = to_string(primeTime.tv_usec);

                    while(strPrimeTime.length() < 10){
                        strPrimeTime = "0" + strPrimeTime;
                    }

                    string message2 = "{T:" + strPrimeTime + "}";
             //       cout << "Writing Message For Found Prime Factor: " << message2 << endl;
                    write(pipeConnectionToParent[1], message2.c_str(), message2.length());

                }
            }else{
                gmp_printf("%Zd is not divisible by %Zd\n", n, d);
            }

            gettimeofday(&endTime,NULL);

            long timeDifference = endTime.tv_usec - startTime.tv_usec;
            long threadid = pthread_self();

            string message = to_string(threadid) + " has completed calculations in " + to_string(timeDifference) + "microsec";
            Logger::logToFile(message);

        }else{
            cout << getpid() << " Received Message To Terminate. Terminating." << endl;
            break;
        }
    }
}
/**
 * stop function changes the state of the WorkerProcess so that it ccan no longer continue working. This value is checked
 * in the start() function which when set to false causes the infinite while loop to break
 */
void WorkerProcess::stop() {

    //execute order 66
    this->continueWorking = false;
}
/**
 * setIdleState sets the state of the WorkerProcess. If the state passed in by the parameter is different then the
 * current state of the WorkerPRocess a message is sent to the parent process to inform it of its changed state. This
 * is then used by the parent to determine if all work has been completed.
 * @param isIdle bool - The new state being set for the WorkerProcess
 */
void WorkerProcess::setIdleState(bool isIdle) {

    //only if the state is different then it currently is do we change the state and thus inform
    //the parent that this worker process is idle
    if(this->isIdle != isIdle){
        this->isIdle = isIdle;

        //inform the parent process of the new state
        if(isIdle){

            string message = "{" + to_string(this->indexID) + ":IDLE000000}";
            //cout << "Message Being Sent: " << message << endl;
            write(this->pipeConnectionToParent[1], message.c_str(), message.length());

        }else{
            string message = "{" + to_string(this->indexID) + ":BUSY000000}";
            //cout << "Message Being Sent: " << message << endl;
            write(this->pipeConnectionToParent[1], message.c_str(), message.length());
        }
    }

}
/**
 * checkForMoreWork checks the pipeConnectionToChild pipe for more work. It is a non blocking read and will continue
 * trying to read from the pipe until it receives more work, or receives a TERMINATE message from the parent process.
 * When there is not work, checkForMoreWork will release the current process from the CPU allowing other processes to
 * have a turn. If work is received, the work is set to the currentWork variable
 */
void WorkerProcess::checkForMoreWork() {

    char inbuf[MSGSIZE];
    bool validRead;

    while(1){
        //Check the PIPE for work
        cout << getpid() << " About To Check For Work" << endl;
        long bytesRead = read (this->pipeConnectionToChild[0], inbuf, MSGSIZE);
        if(bytesRead > 0){
            validRead = true;
        }else{
            validRead = false;
        }

        if(validRead){
            char input = inbuf[0];
            cout << getpid() << " Done Looking For Work" << endl;
            string strMessage = string(inbuf);

            if(strMessage.compare("{TERMINATE}000000000000") == 0){
                this->stop();
                this->currentWork = string(inbuf);
            }else{
                this->currentWork = string(inbuf);
                this->setIdleState(false);
            }


            break;
        }else{

            cout << getpid() << " No Work Available. Releasing CPU" << endl;

            this->setIdleState(true);

            struct timespec waitTime;
            waitTime.tv_nsec = 0;
            waitTime.tv_sec = 0;

            int result = nanosleep(&waitTime, NULL);
            if(result != 0){
                cout << pthread_self() << " ERROR. Sleep/CPU Release Failure" << endl;
            }
        }
    }






}