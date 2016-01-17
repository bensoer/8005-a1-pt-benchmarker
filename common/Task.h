//
// Created by bensoer on 08/01/16.
//

#ifndef INC_8005_A1_PT_BENCHMARKER_TASK_H
#define INC_8005_A1_PT_BENCHMARKER_TASK_H


#include <gmp.h>

class Task {

public:
    bool isValid();
    bool valid = true;
    mpz_t n;
    mpz_t d;

    Task();
    ~Task();

private:

};


#endif //INC_8005_A1_PT_BENCHMARKER_TASK_H
