//
// Created by bensoer on 08/01/16.
//

#include "PrimeDecomposer.h"

mpz_t PrimeDecomposer::decompose(mpz_t n) {

    //int i = 0;
    mpz_t tmp;
    mpz_t d;

    mpz_init(tmp);
    mpz_init(d);

    mpz_set_ui(d, 1);
    do
    {
        mpz_add_ui(tmp, d, 1);
        mpz_swap(tmp, d);
    } while(!mpz_divisible_p(n, d));

    //d at this point is a divisable number

    mpz_divexact(tmp, n, d);
    mpz_swap(tmp, n);

   if(mpz_cmp_si (n, 1)){
       //put n into the array to be tried again
   }

    return d;
}