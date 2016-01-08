//
// Created by bensoer on 08/01/16.
//

#ifndef INC_8005_A1_PT_BENCHMARKER_WORKERTHREAD_H
#define INC_8005_A1_PT_BENCHMARKER_WORKERTHREAD_H


#include "../common/TaskManager.h"

class WorkerThread {

public:
    WorkerThread(TaskManager * taskManager);
    void start();
private:

    void checkForMoreWork();

};


#endif //INC_8005_A1_PT_BENCHMARKER_WORKERTHREAD_H
