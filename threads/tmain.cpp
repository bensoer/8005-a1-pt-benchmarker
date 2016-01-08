#include <iostream>
#include "../common/TaskManager.h"
#include "../common/argparcer.h"
#include "./WorkerThread.h"
#include <pthread.h>
#include <stdio.h>

using namespace std;

/**
 * bootstrapper wraps the worker thread so that pthread can be happy calling only a function in its thread. Within
 * the bootstrapper we cast back our object and tell it to start which contains our context
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

    TaskManager myTasks;

    WorkerThread * wt1 = new WorkerThread(&myTasks);
    /*WorkerThread wt2(myTasks);
    WorkerThread wt3(myTasks);
    WorkerThread wt4(myTasks);
    WorkerThread wt5(myTasks);*/

    pthread_t t1;
    /*pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_t t5;*/

    pthread_create(&t1,NULL, &bootstrapper, wt1);
    /*pthread_create(&t2,NULL, wt2.start ,NULL);
    pthread_create(&t3,NULL, wt3->start ,NULL);
    pthread_create(&t4,NULL, wt4->start ,NULL);
    pthread_create(&t5,NULL, wt5->start ,NULL);*/

    /*TaskManager myTasks;
    myTasks.addTask("Hello");
    myTasks.addTask("World");
    myTasks.addTask("AGAIN");


    string task1 = myTasks.getTask();
    string task2 = myTasks.getTask();
    string task3 = myTasks.getTask();
    string task4 = myTasks.getTask();

    cout << task1 << endl;
    cout << task2 << endl;
    cout << task3 <<endl;
    cout << task4 << endl;*/




    return 0;
}

