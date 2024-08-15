#include "strings_manager.h" 

/* is_null_or_empty - Checks if the given string is NULL or empty.
 * @param str Pointer to the string to be checked.
 * @return true if the string is NULL or empty, false otherwise.
 */
int is_null_or_empty(const char* str) {
	return (str == NULL || *str == '\0');
}

/**
 *split_string -
 * Splits a string into an array of tokens based on spaces and commas.
 *
 *@param str The input string to be split into tokens.
 *
 * @return A pointer to an array of strings, where each string is a token from the input
 *         string. The array is terminated with a NULL pointer.
 */
char** split_string(const char* str) {
	const char* end;
	const char* start;
	int index = 0;

	/* Allocate an initial size for the result array*/
	int initial_size = ARRAY_INITIAL_SIZE;
	char** result = (char**)malloc(initial_size * sizeof(char*));

	if (result == NULL) {
		LOG_ERROR("Memory allocation failed");
		return NULL;
	}

	start = str;
	while (*start) {
		/* Skip leading spaces and commas*/
		while ((*start == ' ' || *start == ',') && *start != '\0') start++;
		end = start;
		/* Find the end of the word*/
		while (*end != ' ' && *end != ',' && *end != '\0') end++;

		if (end > start) {
			int len = (int)(end - start);
			result[index] = (char*)malloc((len + 1) * sizeof(char));
			if (result[index] == NULL) {
				LOG_ERROR("Memory allocation failed");
				return NULL;
			}
			strncpy(result[index], start, len);
			result[index][len] = '\0';
			index++;

			/* Reallocate memory if the result array is full*/
			if (index >= initial_size) {
				char** temp;
				initial_size *= 2;
				temp = (char**)realloc(result, initial_size * sizeof(char*));
				if (temp == NULL) {
					LOG_ERROR("Memory reallocation failed");
					return NULL;
				}
				result = temp;
			}
		}
		start = end;
	}

	/* Add a NULL pointer at the end of the array to indicate the end*/
	result[index] = NULL;

	return result;
}

/* Function to free the allocated memory for the array of strings*/
void free_split_string(char** split_str) {
	if (split_str != NULL) {
		int i;
		for (i = 0; split_str[i] != NULL; i++) {
			free(split_str[i]);
		}
		free(split_str);
	}
}

char* letter_to_15bit_ascii(char letter) {
	char* result = (char*)malloc(16 * sizeof(char));
	int i;
	unsigned short ascii_value;

	if (result == NULL) {
		LOG_ERROR("Failed to allocate memory");
		return NULL;
	}

	/* Ensure the input is a lowercase English letter*/
	if (letter < 'a' || letter > 'z') {
		free(result);
		return NULL;
	}

	ascii_value = (unsigned short)letter;

	/* Convert ASCII value to a 15-bit binary string*/
	for (i = 14; i >= 0; i--) {
		result[14 - i] = (ascii_value & (1 << i)) ? '1' : '0';
	}
	result[15] = '\0';

	return result;
}

char* strtrimlast(const char* src) {
	int len = strlen(src);
	char* dest;
	/* Check if the string is empty*/
	if (len == 0) {
		return NULL; /* Return NULL for empty strings*/
	}

	/* Allocate memory for the new string excluding the last character plus the null terminator*/
	dest = (char*)malloc(len);
	if (dest == NULL) {
		return NULL; /* Return NULL if memory allocation fails*/
	}

	/* Copy all characters except the last one*/
	strncpy(dest, src, len - 1);

	/* Ensure the new string is null-terminated*/
	dest[len - 1] = '\0';

	return dest;
}

char* remove_first_last(const char* str) {
	int len = strlen(str);
	char* new_str;

	if (len <= 2) {
		/* If the string has two or fewer characters, return an empty string*/
		return duplicate_string("");
	}

	/* Allocate memory for the new string*/
	new_str = (char*)malloc(len - 1);
	if (new_str == NULL) {
		LOG_ERROR("Failed to allocate memory");
		return NULL;
	}

	/* Copy the substring excluding the first and last characters*/
	strncpy(new_str, str + 1, len - 2);
	new_str[len - 2] = '\0'; /* Null-terminate the new string*/

	return new_str;
}

char* clone_string(const char* input) {
	char* clone;

	if (input == NULL) {
		return NULL;
	}

	/* Allocate memory for the cloned string*/
	clone = (char*)malloc(strlen(input) + 1); /* +1 for the null terminator*/
	if (clone == NULL) {
		/* Handle memory allocation failure*/
		return NULL;
	}

	/* Copy the string*/
	strcpy(clone, input);

	return clone;
}


char* duplicate_string(const char* str) {
	int len;
	char* duplicate;

	if (str == NULL) {
		return NULL; /* Handle the case where the input is NULL*/
	}

	/* Allocate memory for the duplicate string*/
	len = strlen(str) + 1; /* +1 for the null terminator*/
	duplicate = (char*)malloc(len);

	if (duplicate == NULL) {
		return NULL; /* Handle memory allocation failure*/
	}

	/* Copy the string into the allocated memory*/
	strcpy(duplicate, str);

	return duplicate;
}

int is_first_char_a_letter(const char* str) {
	if ((str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z')) {
		return FOUND;
	}
	else {
		NOT_FOUND;
	}
}

int is_first_char_quotation(const char* str) {
	if (str[0] == '"') {
		return FOUND; /* Return true if the symbol_name exists in the ext array*/
	}
	return NOT_FOUND;
}


