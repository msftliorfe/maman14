#include "actions.h"

/**
 * intialize_actions_array -
 * Initializes an array of Action structures with predefined values.
 *
 * @param actions A pointer to the array of Action structures to be initialized.
 */
void intialize_actions_array(Action* actions)
{
	actions[0].action_name = "mov";
	actions[0].action_code = 0;
	actions[0].source_operands = "0,1,2,3";
	actions[0].destination_operands = "1,2,3";

	actions[1].action_name = "cmp";
	actions[1].action_code = 1;
	actions[1].source_operands = "0,1,2,3";
	actions[1].destination_operands = "0,1,2,3";

	actions[2].action_name = "add";
	actions[2].action_code = 2;
	actions[2].source_operands = "0,1,2,3";
	actions[2].destination_operands = "1,2,3";

	actions[3].action_name = "sub";
	actions[3].action_code = 3;
	actions[3].source_operands = "0,1,2,3";
	actions[3].destination_operands = "1,2,3";

	actions[4].action_name = "lea";
	actions[4].action_code = 4;
	actions[4].source_operands = "1";
	actions[4].destination_operands = "1,2,3";

	actions[5].action_name = "clr";
	actions[5].action_code = 5;
	actions[5].source_operands = "-1";
	actions[5].destination_operands = "1,2,3";

	actions[6].action_name = "not";
	actions[6].action_code = 6;
	actions[6].source_operands = "-1";
	actions[6].destination_operands = "1,2,3";

	actions[7].action_name = "inc";
	actions[7].action_code = 7;
	actions[7].source_operands = "-1";
	actions[7].destination_operands = "1,2,3";

	actions[8].action_name = "dec";
	actions[8].action_code = 8;
	actions[8].source_operands = "-1";
	actions[8].destination_operands = "1,2,3";

	actions[9].action_name = "jmp";
	actions[9].action_code = 9;
	actions[9].source_operands = "-1";
	actions[9].destination_operands = "1,2,3";

	actions[10].action_name = "bne";
	actions[10].action_code = 10;
	actions[10].source_operands = "-1";
	actions[10].destination_operands = "1,2";

	actions[11].action_name = "red";
	actions[11].action_code = 11;
	actions[11].source_operands = "-1";
	actions[11].destination_operands = "1,2";

	actions[12].action_name = "prn";
	actions[12].action_code = 12;
	actions[12].source_operands = "-1";
	actions[12].destination_operands = "1,2,3";

	actions[13].action_name = "jsr";
	actions[13].action_code = 13;
	actions[13].source_operands = "-1";
	actions[13].destination_operands = "0,1,2,3";

	actions[14].action_name = "rts";
	actions[14].action_code = 14;
	actions[14].source_operands = "-1";
	actions[14].destination_operands = "1,2";

	actions[15].action_name = "stop";
	actions[15].action_code = 15;
	actions[15].source_operands = "-1";
	actions[15].destination_operands = "-1";
}
/**
 * print_actions -
 * Prints the details of each action in an array of Action structures.
 * Used only for work purposes

 * @param actions A pointer to the array of Action structures to be printed.
 */
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
/**
 * get_source_operands -
 *  Retrieves the source operands for a specified action.
 *
 * @param actions A pointer to the array of Action structures.
 * @param action_name The name of the action to search for (e.g., "mov", "add").
 * @return A pointer to the string of source operands if the action is found,
 *         or NULL if the action name is not found.
 */
char* get_source_operands(Action* actions, char* action_name) {
	int i;
	/* Iterate through each action in the array and compare the current action's name with the provided action name*/
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			/*A match is found*/
			return actions[i].source_operands;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/**
 * get_destination_operands -
 * Retrieves the destination operands for a specified action.
 *
 * @param actions A pointer to the array of Action structures.
 * @param action_name The name of the action to search for (e.g., "mov", "add").
 * @return A pointer to the string of destination operands if the action is found,
 *         or NULL if the action name is not found.
 */
char* get_destination_operands(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			/* Action name is found*/
			return actions[i].destination_operands;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/**
 * get_action_code -
 * Retrieves the action code by action name.
 *
 * @param actions A pointer to the array of Action structures.
 * @param action_name The name of the action to search for (e.g., "mov", "add").
 * @return A pointer to the action code cast as a `char*` if the action is found,
 *         or NULL if the action name is not found.
 */
int get_action_code(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			/*Action name found*/
			return actions[i].action_code;
		}
	}
	return -1; /* Return NULL if action name not found*/
}

/**
 * action_exists -
 * Checks if a string exists as an action name.
 *
 * @param actions A pointer to the array of Action structures.
 * @param action_name The name of the action to search for (e.g., "mov", "add").
 * @return FOUND if the action name is found, or NOT_FOUND if it is not found.
 */
int action_exists(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return FOUND; /* Action name found*/
		}
	}
	return NOT_FOUND; /* Action name not found*/
}