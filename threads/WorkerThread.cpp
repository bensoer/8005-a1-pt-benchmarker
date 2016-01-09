//
// Created by bensoer on 08/01/16.
//

#include <ostream>
#include <iostream>
#include "WorkerThread.h"

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
        cout << "Recieved More Work" << endl;

    }

}
