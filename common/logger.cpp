//
// Created by bensoer on 09/01/16.
//

#include <fstream>
#include <iostream>
#include "logger.h"

void Logger::log(string message) {

}

void Logger::logToFile(string message) {

    std::fstream fs;
    fs.open ("./syslog.log", std::fstream::in | std::fstream::out | std::fstream::app);

    fs << message << endl;

    fs.close();
}

void Logger::logToConsole(string message) {

    //pthread_mutex_lock(&Logger::mu);
    cout << message << endl;
    //pthread_mutex_unlock(&Logger::mu);

}