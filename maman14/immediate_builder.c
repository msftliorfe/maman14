#include "immediate_builder.h"

/**
 * immediate_are - 
 * Returns a static string representation for Immediate addressing.
 *
 * @return A string representing the Addressing Mode for Immediate values.
 */
char* immediate_are() {
	return "100";
}

/**
 * generate_immediate_line -
 * Generates the binary code for an Immediate operand.
 *
 * @param num The immediate value to be encoded.
 * @return A string representing the binary code for the immediate operand, or NULL if memory allocation fails or an error occurs.
 */
char* generate_immediate_line(int num) {
	char* combinedString;
	int combinedLength;
	/* Generate binary string representation*/
	char* binaryString = intTo12Bit2ComplementString(num);
	char* staticString = immediate_are(); /* Static string*/
	
	/* Check for NULL pointers and free allocated memory if necessary*/
	if (!binaryString) {
		return NULL; /* Return NULL if binaryString is NULL*/
	}

	/* Allocate memory for the combined result*/
	combinedLength = strlen(binaryString) + strlen(staticString) + 1;
	combinedString = (char*)malloc(combinedLength);
	if (!combinedString) {
		free(binaryString); /* Free the binary string if memory allocation failed*/
		return NULL; /* Memory allocation failed*/
	}

	/* Combine the strings*/
	strcpy(combinedString, binaryString);
	strcat(combinedString, staticString);

	/* Free the allocated memory for the binary string*/
	free(binaryString);

	return combinedString;
}

