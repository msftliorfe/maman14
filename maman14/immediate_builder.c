#include "immediate_builder.h"

/* Function to return a static string representation*/
char* immediate_are() {
	return "100";
}

/* Function to generate a line based on a given number*/
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

