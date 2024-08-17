#include "actions.h"

/**
 * intialize_actions_array - 
 * Initializes an array of Action structures with predefined values.
 *
 * @param actions A pointer to the array of Action structures to be initialized.
 */
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
char* get_action_code(Action* actions, char* action_name) {
	int i;
	for (i = 0; i < NUM_OF_ACTIONS; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			/*Action name found*/
			return (char*)actions[i].action_code;
		}
	}
	return NULL; /* Return NULL if action name not found*/
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