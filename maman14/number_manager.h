#ifndef NUMBER_MANAGER_H
#define NUMBER_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"
#include "error_manager.h"

#define BITS 12

/* Convert an unsigned int to a 12-bit unsigned string*/
char* intTo12BitUnsignedString(unsigned int num);

/* Convert an int to a 12-bit two's complement string*/
char* intTo12Bit2ComplementString(int num);

/* Convert an int to a 3-bit binary string*/
char* intToThreeBitBinary(int num);

/* Convert an int to a 4-bit string*/
char* intTo4BitString(int number);

char* int_to_15bit_twos_complement(int number);

int calc_array_length(char** array);

char* bitStringToOctal(const char* bitString);
#endif /*NUMBER_MANAGER_H*/
