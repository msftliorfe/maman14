#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "strings_manager.h"
#include "number_manager.h"
#include "constants.h"
#include "error_manager.h"

char** handle_numbers(char** number_strings);
char** handle_strings(const char* input_array);
char** generateDataLine(char** input_array);

#endif /*DATA_MANAGER_H*/

