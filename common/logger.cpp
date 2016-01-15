//
// Created by bensoer on 09/01/16.
//

#include <fstream>
#include <iostream>
#include "logger.h"

/**
 * logToFile logs the passed message to the syslog.log file associated with the executing program.
 * @param message string - The message being written to the syslog.log file
 */
void Logger::logToFile(string message) {

    std::fstream fs;
    fs.open ("./syslog.log", std::fstream::in | std::fstream::out | std::fstream::app);

    fs << message << endl;

    fs.close();
}
/**
 * logToConsole logs the passed in message to the console.
 * @param message string - the message being written to console
 */
void Logger::logToConsole(string message) {

    //TODO:All mutual exclusion control to the console

    //pthread_mutex_lock(&Logger::mu);
    cout << message << endl;
    //pthread_mutex_unlock(&Logger::mu);

}