//
// Created by bensoer on 09/01/16.
//

#ifndef INC_8005_A1_PT_BENCHMARKER_WORKERPROCESS_H
#define INC_8005_A1_PT_BENCHMARKER_WORKERPROCESS_H

#include <string>

using namespace std;

class WorkerProcess {
public:
    WorkerProcess(int* pipeConnectionToChild, int* pipeConnectionToParent, int indexID);
    void start();
    void setIdleState(bool isIdle);
    void stop();
private:

    int* pipeConnectionToChild;
    int* pipeConnectionToParent;
    int indexID;

    void checkForMoreWork();
    void parseValuesFromWork();

    bool isIdle = false;
    bool continueWorking = true;

    const int MSGSIZE = 23;

    string currentWork;
    int n;
    int d;

};


#endif //INC_8005_A1_PT_BENCHMARKER_WORKERPROCESS_H
