#ifndef IMMEDIATE_BUILDER_H
#define IMMEDIATE_BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "number_manager.h"
#include "constants.h"
#include "error_manager.h"


#define BITS 12


/* Function prototypes*/
char* immediate_are(void);
char* generate_immediate_line(int num);

#endif /* NUMBER_HANDLER_H*/

