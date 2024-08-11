#include "actions.h"

/* Define the actions array*/
/*ToDo - no globals - not allowd!*/
Action actions[] = {
	{"mov",0, "0,1,2,3", "1,2,3"},
	{"cmp",1, "0,1,2,3", "0,1,2,3"},
	{"add",2, "0,1,2,3", "1,2,3"},
	{"sub",3, "0,1,2,3", "1,2,3"},
	{"lea",4, "1", "1,2,3"},
	{"clr",5, "-1", "1,2,3"},
	{"not",6, "-1", "1,2,3"},
	{"inc",7, "-1", "1,2,3"},
	{"dec",8, "-1", "1,2,3"},
	{"jmp",9, "-1", "1,2,3"},
	{"bne",10, "-1", "1,2"},
	{"red",11, "-1", "1,2"},
	{"prn",12, "-1", "1,2,3"},
	{"jsr",13, "-1", "0,1,2,3"},
	{"rts",14, "-1", "1,2"},
	{"stop",15, "-1", "-1"}
};

/* Calculate the number of actions*/
/*ToDo - no globals - not allowd!*/
int num_actions = sizeof(actions) / sizeof(actions[0]);

/* Define the function to print the actions*/
void print_actions(void) {
	int i; 
	for (i = 0; i < num_actions; i++) {
		printf("Action: %s\n", actions[i].action_name);
		printf("Source Operands: %s\n", actions[i].source_operands);
		printf("Destination Operands: %s\n", actions[i].destination_operands);
		printf("\n");
	}
}

/* Function to get source operands by action name*/
char* get_source_operands(char* action_name) {
	int i; 
	for (i = 0; i < num_actions; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return actions[i].source_operands;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/* Function to get destination operands by action name*/
char* get_destination_operands(char* action_name) {
	int i; 
	for (i = 0; i < num_actions; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return actions[i].destination_operands;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/* Function to get action code by action name*/
char* get_action_code(char* action_name) {
	int i; 
	for (i = 0; i < num_actions; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return (char*)actions[i].action_code;
		}
	}
	return NULL; /* Return NULL if action name not found*/
}

/* Function to check if a string exists as an action name*/
int action_exists(char* action_name) {
	int i; 
	for (i = 0; i < num_actions; i++) {
		if (strcmp(actions[i].action_name, action_name) == 0) {
			return FOUND; /* Action name found*/
		}
	}
	return NOT_FOUND; /* Action name not found*/
}


