//
// Created by bensoer on 08/01/16.
//

#include <ostream>
#include <iostream>
#include "WorkerThread.h"
#include "../common/logger.h"
#include <gmp.h>
#include <sys/time.h>

WorkerThread::WorkerThread(TaskManager *taskManager, vector<long> * primeTimes) {
    this->tasks = taskManager;
    this->primeTimes = primeTimes;
}

WorkerThread::~WorkerThread() {

}

bool WorkerThread::isIdle() {
    return this->idle;
}

void WorkerThread::stop() {
    this->continueRunning = false;
}

void WorkerThread::checkForMoreWork() {

    Task task;
    while(!(task = this->tasks->getTask()).isValid()){
        cout << pthread_self() << " Task Is Not Valid. Waiting" << endl;
        this->idle = true;
        if(this->continueRunning == false){
            return;
        }
    }

    cout << "Got A Task. Were Not Idling" << endl;
    //task now has a value
    this->idle = false;
    this->currentTask = task;
}

void WorkerThread::start() {

    while(1){

        struct timeval startTime;
        struct timeval endTime;
        struct timeval primeTime;

        cout << "Now Checking For More Work" << endl;
        this->checkForMoreWork();
        if(this->continueRunning == false){
            break;
        }
        cout << pthread_self() << " Recieved More Work" << endl;
        gettimeofday(&startTime,NULL);

        /* -- Initialization For Calculations -- */

        mpz_t n;
        mpz_t tmp;
        mpz_t d;

        mpz_init(tmp);
        mpz_init(d);
        mpz_init(n);

        mpz_set(n, this->currentTask.n);
        mpz_set(d, this->currentTask.d);

        //gmp_printf("%Zd\n", n);
        //gmp_printf("%Zd\n", d);

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
                    Task * task = new Task();
                    mpz_set(task->n,n);
                    mpz_set_ui(task->d,i);
                    this->tasks->addTask(*task);
                }
            //else you have found some kind of prime factor
            //this means the d could equal to 1 or n is greater then 1
            }else{

                //gmp_printf("FOUND PRIME FACTOR: %Zd\n", n);

                long threadid = pthread_self();
                string message = to_string(threadid) + " has found a Prime Number Factor Of: " + to_string(mpz_get_ui(n));
                Logger::logToFile(message);

                gettimeofday(&primeTime,NULL);
                this->primeTimes->push_back(primeTime.tv_usec);
            }
        }else{
            gmp_printf("%Zd is not divisible by %Zd\n", n, d);
        }

        gettimeofday(&endTime,NULL);

        long timeDifference = endTime.tv_usec - startTime.tv_usec;
        long threadid = pthread_self();

        string message = to_string(threadid) + " has completed calculations in " + to_string(timeDifference) + "ms";
        Logger::logToFile(message);

    }

}


