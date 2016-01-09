//
// Created by bensoer on 08/01/16.
//

#ifndef INC_8005_A1_PT_BENCHMARKER_TASKS_H
#define INC_8005_A1_PT_BENCHMARKER_TASKS_H

#include <string>
#include <mutex>
#include <queue>
#include "Task.h"

using namespace std;

class TaskManager{

private:

    queue<Task> tasks;
    mutex mu;

public:
    void addTask(Task task);
    Task getTask();

};

#endif //INC_8005_A1_PT_BENCHMARKER_TASKS_H
