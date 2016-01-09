//
// Created by bensoer on 08/01/16.
//

#include "Task.h"

bool Task::isValid() {
    return this->valid;
}

Task::Task() {
    mpz_init(this->n);
    mpz_init(this->d);
}