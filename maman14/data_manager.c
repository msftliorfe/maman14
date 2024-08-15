#include "data_manager.h"

char** handle_numbers(char** number_strings) {
	char** result;
	int count = 0;
	int i;

	/* Calculate the number of strings in the array */
	while (number_strings[count] != NULL) {
		count++;
	}

	/* Allocate memory for the result array, including space for the null terminator */
	result = (char**)malloc((count + 1) * sizeof(char*));
	if (result == NULL) {
		LOG_ERROR("Memory allocation failed\n");
		return NULL;
	}

	for (i = 0; i < count; i++) {
		int number = atoi(number_strings[i]);
		result[i] = int_to_15bit_twos_complement(number);
	}

	/* Null-terminate the result array */
	result[count] = NULL;

	return result;
}


char** handle_strings(const char* input_string) {
	if (!is_first_char_quotation(input_string)) {
		LABEL_ERROR("string is not valid", input_string);
		return "";
	}
	else {
		char* trimmed = remove_first_last(input_string);
		if (trimmed == NULL) {
			LABEL_ERROR("string is not valid", input_string);
			return "";
		}
		int length, i;
		char** result;
		const int extra_length = 1;  /* Additional slot for the 15 "0"s string*/
		char* zero_string;

		length = strlen(trimmed);
		result = (char**)malloc((length + extra_length) * sizeof(char*));
		if (result == NULL) {
			LOG_ERROR("Memory allocation failed\n");
			return NULL;
		}

		for (i = 0; i < length; i++) {
			result[i] = letter_to_15bit_ascii(trimmed[i]);
		}

		/* Add the "000000000000000" string to the end of the result*/
		zero_string = (char*)malloc((WORD_SIZE_IN_BITS + 1) * sizeof(char));  /* 15 "0"s + NULL terminator*/
		if (zero_string == NULL) {
			LOG_ERROR("Memory allocation failed");
			exit(EXIT_FAILURE);
		}
		memset(zero_string, '0', WORD_SIZE_IN_BITS);  /* Fill with "0"s*/
		zero_string[WORD_SIZE_IN_BITS] = '\0';        /* Null-terminate the string*/
		result[length] = zero_string;

		result[length + 1] = NULL;  /* NULL-terminate the array*/

		free(trimmed);  /* Don't forget to free the trimmed string if it was dynamically allocated*/
		return result;
	}
}



char** generateDataLine(const char** input_array) {
	if (strcmp(input_array[0], ".data") == 0) {
		return handle_numbers(input_array + 1);
	}
	else {
		return handle_strings(input_array[1]);

	}
}
