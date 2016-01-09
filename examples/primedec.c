#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "primedecompose.h"

// uses the GMP (GNU Multiple Precision Library for the computations
//gcc -Wall -o pdec primedec.c primedecompose.c -lgmp

#define MAX_FACTORS	1024



// Globals
mpz_t dest[MAX_FACTORS]; // must be large enough to hold all the factors! 
 
int main(int argc, char **argv) 
{
	mpz_t n;
  	int i, l;
 
  	if(argc != 2) 
	{
    		puts("Usage: ./pdec <number to be factored>");
    		return EXIT_SUCCESS;
  	}
 
  	mpz_init_set_str(n, argv[1], 10);
  	l = decompose(n, dest);
 
  	for(i=0; i < l; i++) 
	{
    		gmp_printf("%s%Zd", i?" * ":"", dest[i]);
    		mpz_clear(dest[i]);
  	}
  	printf("\n");
 
  	return EXIT_SUCCESS;
}
