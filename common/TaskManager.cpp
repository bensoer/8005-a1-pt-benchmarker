//
// Created by bensoer on 08/01/16.
//

#include "TaskManager.h"

/**
 * destructor empties the task manager of any tasks stored within it.
 */
TaskManager::~TaskManager() {
    while (!this->tasks.empty())
    {
        this->tasks.pop();
    }
}
/**
 * addTask adds the passed in task to the tasks list using mutual exclusion.
 * @param task Task - a given task to be added to the tasks queue
 */
void TaskManager::addTask(Task task) {

    //this->mu.lock();
    pthread_mutex_lock(&this->mu);
    this->tasks.push(task);
    pthread_mutex_unlock(&this->mu);
}
/**
 * getTask gets the oldest task from the list of tasks using mutual exlusion. This creates a copy of the task before
 * deleting it out of the queue and then returning it to the client
 */
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