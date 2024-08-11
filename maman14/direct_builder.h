#ifndef DIRECT_BUILDER_H
#define DIRECT_BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "number_manager.h"
#include "symbols_manager.h"
#include "constants.h"
#include "error_manager.h"

#include <stddef.h>/*ToDo -  change this!*/

/* Function to generate the ARE bits for a direct addressing line*/
char* direct_are(int num);

/* Function to generate a complete direct addressing line*/
char* generate_direct_line(int num);

#endif /* DIRECT_BUILDER_H*/

