#include <iostream>
#include "../common/TaskManager.h"
#include "../common/argparcer.h"
#include "./WorkerThread.h"
#include "../common/logger.h"
#include <pthread.h>
#include <stdio.h>
#include <vector>

using namespace std;

/**
 * bootstrapper wraps the worker thread so that pthread can be happy calling only a function in its thread. Within
 * the bootstrapper we cast back our object and tell it to start which contains our context
 * @param workerObject void* - A pointer to the worker thread object to execute upon starting the thread
 */
void * bootstrapper(void * workerObject){
    WorkerThread * worker = (WorkerThread*)workerObject;
    worker->start();
}

int main(int argc, char * argv[]) {

    ArgParcer parcer;
    int number = parcer.GetTagVal("-n", argv, argc);
    if(number == -1){
        cout << "A Parameter is Invalid or Not Included" <<endl;
        cout << "Expected Use: tmain.cpp -n <number>"<< endl;
        return 1;
    }else{
        cout << "Parameter Found" << endl;
    }

    //setup the task manager and generate all initial tasks
    TaskManager myTasks;

    cout << "Loading Original Tasks" << endl;
    //find all numbers from 1 to n
    for(unsigned int i = 0; i < number; i++){
        Task task;
        mpz_set_ui(task.n, number);
        mpz_set_ui(task.d, i);

        myTasks.addTask(task);
    }

    vector<long> primeTimes;

    WorkerThread * wt1 = new WorkerThread(&myTasks, &primeTimes);
    WorkerThread * wt2 = new WorkerThread(&myTasks, &primeTimes);
    WorkerThread * wt3 = new WorkerThread(&myTasks, &primeTimes);
    WorkerThread * wt4 = new WorkerThread(&myTasks, &primeTimes);
    WorkerThread * wt5 = new WorkerThread(&myTasks, &primeTimes);

    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_t t5;

    pthread_create(&t1,NULL, &bootstrapper, wt1);
    pthread_create(&t2,NULL, &bootstrapper, wt2);
    pthread_create(&t3,NULL, &bootstrapper, wt3);
    pthread_create(&t4,NULL, &bootstrapper, wt4);
    pthread_create(&t5,NULL, &bootstrapper, wt5);


    //Check for IDLE processes
    while(1){
        if(wt1->isIdle() && wt2->isIdle() && wt3->isIdle() && wt4->isIdle() && wt5->isIdle()){
            cout << "All Are Idle. Processing Complete" << endl;
            wt1->stop();
            wt2->stop();
            wt3->stop();
            wt4->stop();
            wt5->stop();

            delete(wt1);
            delete(wt2);
            delete(wt3);
            delete(wt4);
            delete(wt5);

            break;
        }else{
            cout << "A Process Is still Running. Waiting." <<endl;
        }
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);

    cout << "Writing Out Prime Times to File" << endl;


    for(unsigned int i = 0; i < primeTimes.size(); i++){
        string message = "Prime Number factor found in: " + to_string(primeTimes.at(i)) + "ms";
        Logger::logToFile(message);
    }

    primeTimes.clear();

    return 0;
}

