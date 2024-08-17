#include "first_line_builder.h"

/**
 * generate_operand_code - 
 * Helper function to generate operand code and return it as a new string
 * Generates a 4-bit code for a given operand and returns it as a new string.
 *
 * @param operand The operand string for which the code is to be generated.
 * @return A string containing the 4-bit code for the operand, or NULL if memory allocation fails.
 */
char* generate_operand_code(const char* operand) {
	char* code = malloc(5); /* Allocate memory for the code + null terminator*/
	
	/* Memory allocation failed*/
	if (code == NULL) {
		log_error("generate_operand_code", 16, "first_line_builder.c", "Memory allocation failed");
		return NULL;
	}

	/* Check if the operand is not null or empty */
	if (!is_null_or_empty(operand)) {
		char first_letter = operand[0];
		switch (first_letter) {
		case '#':
			strcpy(code, "0001"); /* Immediate addressing */
			break;
		case '*':
			strcpy(code, "0100"); /* Indirect addressing */
			break;
		default:
			if (is_valid_register(operand)) {
				strcpy(code, "1000"); /* Valid register */
			}
			else {
				strcpy(code, "0010"); /* Other valid operand */
			}
			break;
		}
	}
	else {
		strcpy(code, "0000"); /* Null or empty operand */
	}

	return code;
}

/**
 * generate_first_line -
 * Generates the binary representation of the first line of an action.
 *
 * @param actions Array of Action structs containing action information.
 * @param action_name The name of the action for which to generate the code.
 * @param operand_target The target operand.
 * @param operand_source The source operand.
 * @return A string representing the binary code for the action line, or NULL if memory allocation fails or if any error occurs.
 */
char* generate_first_line(Action* actions, const char* action_name, const char* operand_target, const char* operand_source) {
	char* operand_target_code;
	char* operand_source_code;
	char* action_code_string;
	int action_code;
	/* Allocate memory for result string*/
	char* res = malloc(WORD_SIZE_IN_BITS + 1); /* +1 for the null terminator*/
	if (res == NULL) {
		log_error("generate_first_line", 65, "first_line_builder.c", "Memory allocation failed");
		return NULL;
	}

	/* Initialize result string*/
	res[0] = '\0';

	/* Generate action code (11-14)*/
	action_code = get_action_code(actions, action_name);
	action_code_string = intTo4BitString(action_code);
	strcat(res, action_code_string);
	free(action_code_string); /* Free the allocated memory*/

	/* Generate operand source (7-10)*/
	operand_source_code = generate_operand_code(operand_source);
	if (operand_source_code == NULL) {
		log_error("generate_first_line", 81, "first_line_builder.c", "operand source code error");
		return NULL;
	}
	strcat(res, operand_source_code);
	free(operand_source_code); /* Free the allocated memory*/

	/* Generate operand target (3-6)*/
	operand_target_code = generate_operand_code(operand_target);
	if (operand_target_code == NULL) {
		log_error("generate_first_line", 90, "first_line_builder.c", "operand source code error");
		return NULL;
	}
	strcat(res, operand_target_code);
	free(operand_target_code); /* Free the allocated memory*/

	strcat(res, "100");
	return res;
}

/**
 * process_first_line -
 * Processes the first line of an action and generates its binary representation.
 *
 * @param actions Array of Action structs containing action information.
 * @param line Array of strings representing the action and its operands.
 * @return A string representing the binary code for the action line, or NULL if memory allocation fails or any error occurs.
 */
char* process_first_line(Action* actions, const char** line) {
	int arraySize = 0;

	/*Determine the number of elements in the `line` array*/
	while (line[arraySize] != NULL) {
		arraySize++;
	}
	/* Check for valid number of elements and generate the appropriate binary code*/
	if (arraySize == 1) {
		return	generate_first_line(actions, line[0], NULL, NULL);
	}
	else if (arraySize == 2) {
		return generate_first_line(actions, line[0], line[1], NULL);
	}
	else {
		return generate_first_line(actions, line[0], line[2], line[1]);
	}
}


