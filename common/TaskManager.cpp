//
// Created by bensoer on 08/01/16.
//

#include "TaskManager.h"

void TaskManager::addTask(Task task) {

    this->mu.lock();
    this->tasks.push(task);
    this->mu.unlock();
}

Task TaskManager::getTask() {
    this->mu.lock();


    if(this->tasks.empty()){
        this->mu.unlock();
        Task invalid;
        invalid.valid = false;
        return invalid;
    }
    Task task = tasks.front();
    this->tasks.pop();
    this->mu.unlock();
    return task;
}