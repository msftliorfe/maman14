#include "number_manager.h"

/**
 * intTo12BitUnsignedString - 
 * Converts an unsigned integer to a 12-bit binary string representation.
 *
 * @param num The unsigned integer to convert.
 * @return A pointer to a null-terminated string containing the binary representation.
 *         Returns NULL if the input number is out of range or memory allocation fails.
 */
char* intTo12BitUnsignedString(unsigned int num) {
	unsigned int mask;
	int i;
	char* binaryString;
	if (num > 4095) {
		return NULL;
	}
	binaryString = (char*)malloc(BITS + 1);
	if (!binaryString) {
		return NULL;
	}
	binaryString[BITS] = '\0';
	mask = 1 << (BITS - 1);
	for (i = 0; i < BITS; i++) {
		binaryString[i] = (num & mask) ? '1' : '0';
		mask >>= 1;
	}
	return binaryString;
}

/**
 * intTo12Bit2ComplementString -
 * Converts an integer to a 12-bit two's complement binary string representation.
 *
 * @param num The integer to convert.
 * @return A pointer to a null-terminated string containing the binary representation.
 *         Returns NULL if the input number is out of range or memory allocation fails.
 */
char* intTo12Bit2ComplementString(int num) {
	int i;
	unsigned int mask;	
	char* binaryString; 	
	if (num < -2048 || num > 2047) {
		return NULL;
	}
	binaryString = (char*)malloc(BITS + 1);
	if (!binaryString) {
		return NULL;
	}
	binaryString[BITS] = '\0';
	mask = 1 << (BITS - 1);
	for (i = 0; i < BITS; i++) {
		binaryString[i] = (num & mask) ? '1' : '0';
		mask >>= 1;
	}
	return binaryString;
}

/**
 * intToThreeBitBinary -
 * Converts an integer to a 3-bit binary string representation.
 *
 * @param num The integer to convert (must be between 0 and 7 inclusive).
 * @return A pointer to a null-terminated string containing the binary representation.
 *         Returns NULL if the input number is out of range or memory allocation fails.
 */
char* intToThreeBitBinary(int num) {
	char* binaryString;
	if (num < 0 || num > 7) {
		return NULL;
	}
	binaryString = (char*)malloc(4);
	if (!binaryString) {
		return NULL;
	}
	binaryString[0] = (num & 4) ? '1' : '0';
	binaryString[1] = (num & 2) ? '1' : '0';
	binaryString[2] = (num & 1) ? '1' : '0';
	binaryString[3] = '\0';
	return binaryString;
}

/**
 * intTo4BitString -
 * Converts an integer to a 4-bit binary string representation.
 *
 * @param number The integer to convert (must be between 0 and 15 inclusive).
 * @return A pointer to a null-terminated string containing the 4-bit binary representation.
 *         Returns NULL if the input number is out of range or memory allocation fails.
 */
char* intTo4BitString(int number) {
	int i;
	char* bitString; 
	if (number < 0 || number > WORD_SIZE_IN_BITS) {
		return NULL;
	}
	bitString = (char*)malloc(5);
	if (!bitString) {
		return NULL;
	}
	for (i = 3; i >= 0; i--) {
		bitString[3 - i] = (number & (1 << i)) ? '1' : '0';
	}
	bitString[4] = '\0';
	return bitString;
}

/**
 * int_to_15bit_twos_complement -
 * Converts an integer to a 15-bit two's complement binary string representation.
 *
 * @param number The integer to convert (must be between -16384 and 16383 inclusive).
 * @return A pointer to a null-terminated string containing the 15-bit two's complement representation.
 *         Returns NULL if memory allocation fails or if the number is out of range.
 */
char* int_to_15bit_twos_complement(int number) {
	char* result = (char*)malloc((WORD_SIZE_IN_BITS+1) * sizeof(char));
	int i;
	unsigned short twos_complement;

	if (result == NULL) {
		log_error("int_to_15bit_twos_complement", 122, "number_manager.c", "Memory allocation failed");
				return NULL;
	}

	if (number < -16384 || number > 16383) {
		free(result);
		return NULL;
	}

	if (number < 0) {
		twos_complement = (unsigned short)((~(-number) + 1) & 0x7FFF);
	}
	else {
		twos_complement = (unsigned short)(number & 0x7FFF);
	}

	for (i = (WORD_SIZE_IN_BITS-1); i >= 0; i--) {
		result[(WORD_SIZE_IN_BITS-1) - i] = (twos_complement & (1 << i)) ? '1' : '0';
	}
	result[WORD_SIZE_IN_BITS] = '\0';

	return result;
}

/**
 * calc_array_length -
 * Calculates the number of non-null elements in a null-terminated array of strings.
 *
 * @param array A pointer to a null-terminated array of strings.
 * @return The number of non-null elements in the array. Returns 0 if the input array is NULL.
 */
int calc_array_length(char** array) {
	int arraySize = 0;
	while (array[arraySize] != NULL) {
		arraySize++;
	}
	return arraySize;
}

/**
 * bitStringToOctal -
 * Converts a binary string to its octal representation.
 *
 * @param bitString A null-terminated string representing a binary number.
 * @return A pointer to a null-terminated string containing the octal representation.
 *         Returns NULL if memory allocation fails.
 */
char* bitStringToOctal(const char* bitString) {
	unsigned int decimal = 0;
	unsigned int octal = 0, place = 1;
	char* octalString = (char*)malloc(6 * sizeof(char)); /* 5 chars + 1 null terminator*/
	int i, j;

	if (octalString == NULL) {
		return NULL;  /* Return NULL if memory allocation fails*/
	}

	/* Convert the bit string to a decimal number*/
	for (i = 0; i < WORD_SIZE_IN_BITS; i++) {
		if (bitString[i] == '1') {
			decimal += (1U << ((WORD_SIZE_IN_BITS-1) - i));
		}
	}

	/* Convert the decimal number to octal*/
	while (decimal > 0) {
		octal += (decimal % 8) * place;
		decimal /= 8;
		place *= 10;
	}

	/* Convert the octal number to a string*/
	for (i = 4; i >= 0; i--) {
		octalString[i] = '0' + (octal % 10);
		octal /= 10;
	}
	octalString[5] = '\0';

	/* If octal number is less than 5 digits, pad with leading zeros*/
	for (j = 0; j < 5; j++) {
		if (octalString[j] != '0') {
			break;
		}
		octalString[j] = '0';
	}

	return octalString;
}
