//
// Created by bensoer on 08/01/16.
//

#ifndef INC_8005_A1_PT_BENCHMARKER_TASKS_H
#define INC_8005_A1_PT_BENCHMARKER_TASKS_H

#include <string>
#include <queue>
#include <pthread.h>
#include "Task.h"

using namespace std;

class TaskManager{

private:

    queue<Task> tasks;
    pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;



public:
    void addTask(Task task);
    Task getTask();

    ~TaskManager();

};

#endif //INC_8005_A1_PT_BENCHMARKER_TASKS_H
