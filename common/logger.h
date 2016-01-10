//
// Created by bensoer on 09/01/16.
//

#ifndef INC_8005_A1_PT_BENCHMARKER_LOGGER_H
#define INC_8005_A1_PT_BENCHMARKER_LOGGER_H

#include <string>

using namespace std;

class Logger {

private:
   // static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
public:
    static void log(string message);

    static void logToFile(string message);
    static void logToConsole(string message);

};


#endif //INC_8005_A1_PT_BENCHMARKER_LOGGER_H
