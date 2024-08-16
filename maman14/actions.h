#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "error_manager.h"

/* Define a struct to hold the action details*/
typedef struct {
	char* action_name;
	int action_code;
	char* source_operands;
	char* destination_operands;
} Action;



/* Function to print the actions - used during work not nusseccary*/
void print_actions(Action* actions);

/* Function to get source operands by action name*/
char* get_source_operands(Action* actions, char* action_name);

/* Function to get destination operands by action name*/
char* get_destination_operands(Action* actions, char* action_name);

/* Function to get action code by action name*/
char* get_action_code(Action* actions, char* action_name);

/* Function to get if action exists by action name*/
int action_exists(Action* actions, char* action_name);

void intialize_actions_array(Action* actions);
#endif /* ACTIONS_H*/


