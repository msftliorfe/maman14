#include "register_builder.h"

/**
 * Function to return the ARE string.
 * @return The ARE string.
 */
char* register_are() {
	return "100";
}

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

