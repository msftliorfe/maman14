#include "first_line_builder.h"

/* Helper function to generate operand code and return it as a new string*/
char* generate_operand_code(const char* operand) {
	char* code = malloc(5); /* Allocate memory for the code + null terminator*/
	if (code == NULL) {
		LOG_ERROR("Memory allocation failed");
		return NULL;
	}

	if (!is_null_or_empty(operand)) {
		char first_letter = operand[0];
		switch (first_letter) {
		case '#':
			strcpy(code, "0001");
			break;
		case '*':
			strcpy(code, "0100");
			break;
		default:
			if (is_valid_register(operand)) {
				strcpy(code, "1000");
			}
			else {
				strcpy(code, "0010");
			}
			break;
		}
	}
	else {
		strcpy(code, "0000");
	}

	return code;
}

char* generate_first_line(const char* action_name, const char* operand_target, const char* operand_source) {
	char* operand_target_code;
	char* operand_source_code;
	char* action_code_string;
	char* action_code;
	/* Allocate memory for result string*/
	char* res = malloc(WORD_SIZE_IN_BITS + 1); /* +1 for the null terminator*/
	if (res == NULL) {
		LOG_ERROR("Memory allocation failed");
		return NULL;
	}

	/* Initialize result string*/
	res[0] = '\0';

	/* Generate action code (11-14)*/
	action_code = get_action_code(action_name);
	action_code_string = intTo4BitString(action_code);
	strcat(res, action_code_string);
	free(action_code_string); /* Free the allocated memory*/

	/* Generate operand source (7-10)*/
	operand_source_code = generate_operand_code(operand_source);
	if (operand_source_code == NULL) {
		LOG_ERROR("operand source code error");
		return NULL;
	}
	strcat(res, operand_source_code);
	free(operand_source_code); /* Free the allocated memory*/

	/* Generate operand target (3-6)*/
	operand_target_code = generate_operand_code(operand_target);
	if (operand_target_code == NULL) {
		LOG_ERROR("operand source code error");
		return NULL;
	}
	strcat(res, operand_target_code);
	free(operand_target_code); /* Free the allocated memory*/

	strcat(res, "100");
	return res;
}

char* process_first_line(const char** line) {
	int arraySize = 0;
	while (line[arraySize] != NULL) {
		arraySize++;
	}
	if (arraySize == 1) {
		return	generate_first_line(line[0], NULL, NULL);
	}
	else if (arraySize == 2) {
		return generate_first_line(line[0], line[1], NULL);
	}
	else {
		return generate_first_line(line[0], line[2], line[1]);
	}
}


