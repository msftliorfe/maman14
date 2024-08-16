#include "actions.h"

void intialize_actions_array(Action* actions)
{
	actions[0] = (Action){ "mov", 0, "0,1,2,3", "1,2,3" };
	actions[1] = (Action){ "cmp", 1, "0,1,2,3", "0,1,2,3" };
	actions[2] = (Action){ "add", 2, "0,1,2,3", "1,2,3" };
	actions[3] = (Action){ "sub", 3, "0,1,2,3", "1,2,3" };
	actions[4] = (Action){ "lea", 4, "1", "1,2,3" };
	actions[5] = (Action){ "clr", 5, "-1", "1,2,3" };
	actions[6] = (Action){ "not", 6, "-1", "1,2,3" };
	actions[7] = (Action){ "inc", 7, "-1", "1,2,3" };
	actions[8] = (Action){ "dec", 8, "-1", "1,2,3" };
	actions[9] = (Action){ "jmp", 9, "-1", "1,2,3" };
	actions[10] = (Action){ "bne", 10, "-1", "1,2" };
	actions[11] = (Action){ "red", 11, "-1", "1,2" };
	actions[12] = (Action){ "prn", 12, "-1", "1,2,3" };
	actions[13] = (Action){ "jsr", 13, "-1", "0,1,2,3" };
	actions[14] = (Action){ "rts", 14, "-1", "1,2" };
	actions[15] = (Action){ "stop", 15, "-1", "-1" };
}
/* Define the function to print the actions*/
void print_actions(Action* actions) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		printf("Action: %s\n", actions[i].action_name);
		printf("Source Operands: %s\n", actions[i].source_operands);
		printf("Destination Operands: %s\n", actions[i].destination_operands);
		printf("\n");
	}
}

/* Function to get source operands by action name*/
char* get_source_operands(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return actions[i].source_operands;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/* Function to get destination operands by action name*/
char* get_destination_operands(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return actions[i].destination_operands;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/* Function to get action code by action name*/
char* get_action_code(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return (char*)actions[i].action_code;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/* Function to check if a string exists as an action name*/
int action_exists(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return FOUND; /* Action name found*/
		}
	}
	return NOT_FOUND; /* Action name not found*/
}


