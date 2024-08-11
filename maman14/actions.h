#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "error_manager.h"

/* Define a struct to hold the action details*/
typedef struct {
	const char* action_name;
	const int action_code;
	const char* source_operands;
	const char* destination_operands;
} Action;

/* Function to print the actions*/
void print_actions(void);

/* Function to get source operands by action name*/
char* get_source_operands(char* action_name);

/* Function to get destination operands by action name*/
char* get_destination_operands(char* action_name);

/* Function to get action code by action name*/
char* get_action_code(char* action_name);

/* Function to get if action exists by action name*/
int action_exists(char* action_name);

#endif /* ACTIONS_H*/


