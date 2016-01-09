//
// Created by bensoer on 08/01/16.
//

#include <ostream>
#include <iostream>
#include "WorkerThread.h"
#include <gmp.h>

WorkerThread::WorkerThread(TaskManager *taskManager) {
    this->tasks = taskManager;
}

void WorkerThread::checkForMoreWork() {

    struct timespec waitTime;
    waitTime.tv_nsec = 1000 * 2;
    waitTime.tv_sec = 5;

    Task task;
    while(!(task = this->tasks->getTask()).isValid()){
        cout << pthread_self() << " Task Is Null. Waiting" << endl;
        this->idle = true;
        int result = nanosleep(&waitTime, NULL);
        if(result != 0){
            cout << pthread_self() << " ERROR. Sleep Failure" << endl;
        }
    }

    //task now has a value
    this->idle = false;
    this->currentTask = task;


}

void WorkerThread::start() {

    while(1){
        cout << "Now Checking For More Work" << endl;
        this->checkForMoreWork();
        cout << pthread_self() << " Recieved More Work" << endl;


        mpz_t n;
        mpz_t tmp;
        mpz_t d;

        mpz_init(tmp);
        mpz_init(d);
        mpz_init(n);

        mpz_set(n, this->currentTask.n);
        mpz_set(d, this->currentTask.d);

        gmp_printf("%Zd\n", n);
        gmp_printf("%Zd\n", d);



        //mpz_set_ui(d, 1);


        /*do
        {
            mpz_add_ui(tmp, d, 1);
            mpz_swap(tmp, d);
        } while(!mpz_divisible_p(n, d));
        */

        //mpz_add_ui(tmp,d,1);
        mpz_swap(tmp,d);

        if(mpz_divisible_p(n,d)){
            //d at this point is a divisable number
            gmp_printf("%Zd", d);

            mpz_divexact(tmp, n, d);
            mpz_swap(tmp, n);

            if(mpz_cmp_si (n, 1)){
                //put n into the array to be tried again
                //cycle through and generate all of the d values
                for(unsigned int i = 0; i < mpz_get_ui(n) ; i++){
                    Task * task = new Task();
                    mpz_set(task->n,n);
                    mpz_set_ui(task->d,i);
                    this->tasks->addTask(*task);
                }
            }
        }




    }

}
