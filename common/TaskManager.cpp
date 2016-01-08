//
// Created by bensoer on 08/01/16.
//

#include "TaskManager.h"

void TaskManager::addTask(string task) {

    this->mu.lock();
    this->tasks.push(task);
    this->mu.unlock();
}

string TaskManager::getTask() {
    this->mu.lock();

    if(this->tasks.empty()){
        this->mu.unlock();
        return "";
    }
    string task = tasks.front();
    this->tasks.pop();
    this->mu.unlock();
    return task;
}