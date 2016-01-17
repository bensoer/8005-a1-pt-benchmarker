//
// Created by bensoer on 08/01/16.
//

#include "Task.h"

/**
 * isValid is a helper method to create invalid Tasks. This is used to represent empty tasks or tasks that have no
 * calculations or data.
 */
bool Task::isValid() {
    return this->valid;
}
/**
 * constructor initializes mpz variables to store data for the given task
 */
Task::Task() {
    mpz_init(this->n);
    mpz_init(this->d);
}

Task::~Task() {
    mpz_clear(this->n);
    mpz_clear(this->d);
}