#include "register_builder.h"

/**
 * register_are -  
 * Function to return the ARE string.
 * @return The ARE string.
 */
char* register_are() {
	return "100";
}

/**
 * generate_single_register_line -
 * Generates a binary string for a register line.
 *
 * @param register_number The number of the register (0-7).
 * @param is_source Boolean flag indicating if the register is a source (1) or destination (0).
 * @return A string representing the register line in binary format, or NULL if memory allocation fails.
 */
char* generate_single_register_line(int register_number, int is_source) {
	char* register_number_string = intToThreeBitBinary(register_number);
	char* staticString = register_are();
	char* res;

	if (!register_number_string) {
		return NULL; /* Memory allocation failed or invalid input*/
	}

	/* Determine the required length of the result string*/
	res = (char*)malloc(15);
	if (!res) {
		free(register_number_string);
		return NULL; /* Memory allocation failed*/
	}

	/* Initialize and concatenate the result string*/
	if (is_source) {
		strcpy(res, "000000");
		strcat(res, register_number_string);
		strcat(res, "000");
		strcat(res, staticString);
	}
	else {
		strcpy(res, "000000000");
		strcat(res, register_number_string);
		strcat(res, staticString);
	}

	/* Free the allocated memory for the register number string*/
	free(register_number_string);

	return res;
}

/**
 * generate_combined_register_line -
 * Converts an integer to a 3-bit binary string.
 *
 * @param num The integer number to convert (0-7).
 * @return A string representing the number in 3-bit binary format, or NULL if memory allocation fails.
 */
char* generate_combined_register_line(int a_register_number, int b_register_number) {
	char* a_register_number_string = intToThreeBitBinary(a_register_number);
	char* b_register_number_string = intToThreeBitBinary(b_register_number);
	char* staticString = register_are();
	char* res;

	if (!a_register_number_string || !b_register_number_string) {
		free(a_register_number_string);
		free(b_register_number_string);
		return NULL; /* Memory allocation failed or invalid input*/
	}

	/* Allocate memory for the result string*/
	res = (char*)malloc(16);
	if (!res) {
		free(a_register_number_string);
		free(b_register_number_string);
		return NULL; /* Memory allocation failed*/
	}

	/* Initialize and concatenate the result string*/
	strcpy(res, "000000");
	strcat(res, a_register_number_string);
	strcat(res, b_register_number_string);
	strcat(res, staticString);

	/* Free the allocated memory for the register number strings*/
	free(a_register_number_string);
	free(b_register_number_string);

	return res;
}

