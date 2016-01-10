//
// Created by bensoer on 08/01/16.
//

#include "TaskManager.h"

void TaskManager::addTask(Task task) {

    //this->mu.lock();
    pthread_mutex_lock(&this->mu);
    this->tasks.push(task);
    pthread_mutex_unlock(&this->mu);
}

Task TaskManager::getTask() {
    pthread_mutex_lock(&this->mu);

    if(this->tasks.empty()){
        pthread_mutex_unlock(&this->mu);
        Task invalid;
        invalid.valid = false;
        return invalid;
    }
    Task task = tasks.front();
    this->tasks.pop();
    pthread_mutex_unlock(&this->mu);
    return task;
}