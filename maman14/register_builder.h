#ifndef REGISTER_BUILDER_H
#define REGISTER_BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "number_manager.h"
#include "constants.h"
#include "error_manager.h"


char* register_are();
char* generate_single_register_line(int register_number, int is_source);

char* generate_combined_register_line(int a_register_number, int b_register_number);
#endif /*REGISTER_BUILDER_H*/
