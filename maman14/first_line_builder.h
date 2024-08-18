#ifndef FIRST_LINE_BUILDER_H
#define FIRST_LINE_BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbols_manager.h"
#include "actions.h"
#include "strings_manager.h"
#include "operands.h"
#include "number_manager.h"
#include "constants.h"
#include "error_manager.h"


#define ACTION_CODE_LENGTH 4
#define OPERAND_CODE_LENGTH 4
#define TOTAL_LENGTH (ACTION_CODE_LENGTH + 2 * OPERAND_CODE_LENGTH + 1) // Adjust based on your needs

/* Function to generate operand code based on the provided operand string*/
char* generate_operand_code(Registers* registers, const char* operand);

/* Function to generate the first line of output based on the given parameters*/
char* generate_first_line(Action* actions, char* action_name, const char* operand_target, const char* operand_source, Registers* registers);

char* process_first_line(Action* actions,  char** line,Registers* registers);

#endif /* FIRST_LINE_BUILDER_H*/

