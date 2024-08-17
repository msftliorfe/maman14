#include "direct_builder.h"

/**
 * direct_are - 
 * turns the ARE (Addressing Relocation Entry) bits for a label.
 *
 * The function returns a 3-bit binary string representing the ARE bits based on the given number.
 * The ARE bits indicate how the address of the label should be handled:
 * - "001" indicates the label is external (i.e., located outside the current file/module).
 * - "010" indicates the label is not external (i.e., internal or relocatable within the current file/module).
 *
 * @param num The number representing the label's type or location.
 *            - A value of 0 indicates the label is external.
 *            - Any other value indicates the label is internal or relocatable.
 *
 * @return A string representing the 3-bit ARE binary code ("001" for external, "010" for internal/relocatable).
 */
char* direct_are(int num) {/* the num is a location of a label based on the LabelsManager*/
	if (num == 0) { /* external labal*/
		return "001";
	}
	return "010";
}

/**
 * generate_direct_line -
 * Generates a direct line string representing a label's location in binary format.
 *
 * @param num The location of the label, which is used to generate the binary representation.
 * @return A string that combines the 12-bit binary representation and the static direct addressing string.
 *         Returns NULL if memory allocation fails or if any intermediate step fails.
 */
char* generate_direct_line(int num) { /* the num is a location of a label based on the LabelsManager*/
	int combinedLength;
	char* combinedString;
	char* binaryString = intTo12BitUnsignedString(num);
	char* staticString = direct_are(num);

	if (!binaryString || !staticString) {
		if (binaryString) free(binaryString); /* Free memory if only one allocation succeeded*/
		return NULL; /* Return NULL if any string is NULL*/
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
