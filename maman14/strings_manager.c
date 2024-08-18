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
		log_error("split_string", 30, "strings_manager.c", "Memory allocation failed");
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
				log_error("split_string", 46, "strings_manager.c", "Memory allocation failed");
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
					log_error("split_string", 59, "strings_manager.c", "Memory allocation failed");
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

/**
 * free_split_string -
 * Frees the memory allocated for an array of strings.
 *
 * @param split_str A pointer to an array of strings, where each string is dynamically allocated.
 */
void free_split_string(char** split_str) {
	if (split_str != NULL) {
		int i;
		for (i = 0; split_str[i] != NULL; i++) {
			free(split_str[i]);
		}
		free(split_str);
	}
}

/**
 * letter_to_15bit_ascii -
 * Converts a lowercase English letter to a 15-bit ASCII binary string.
 *
 * @param letter The lowercase English letter to convert (from 'a' to 'z').
 * @return A string representing the letter in 15-bit binary format, or NULL if memory allocation fails or input is invalid.
 */
char* letter_to_15bit_ascii(char letter) {
	char* result = (char*)malloc(16 * sizeof(char));
	int i;
	unsigned short ascii_value;

	if (result == NULL) {
		log_error("letter_to_15bit_ascii", 103, "strings_manager.c", "Memory allocation failed");
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

/**
 * strtrimlast -
 * Creates a new string that is a copy of the input string with the last character removed.
 *
 * @param src The input string from which the last character will be removed.
 * @return A new string without the last character, or NULL if the input string is empty or memory allocation fails.
 */
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

/**
 * remove_first_last -
 * Creates a duplicate of a string.
 *
 * @param str The string to duplicate.
 * @return A newly allocated duplicate of the input string, or NULL if memory allocation fails.
 */
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
		log_error("remove_first_last", 173, "strings_manager.c", "Memory allocation failed");
		return NULL;
	}

	/* Copy the substring excluding the first and last characters*/
	strncpy(new_str, str + 1, len - 2);
	new_str[len - 2] = '\0'; /* Null-terminate the new string*/

	return new_str;
}

/**
 * clone_string -
 * Clones a string by allocating memory and copying its contents.
 *
 * @param input The string to clone.
 * @return A new string that is a duplicate of the input string, or NULL if the input is NULL or memory allocation fails.
 */
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

/**
 * duplicate_string -
 * Duplicates a string by allocating memory and copying its contents.
 *
 * @param str The string to duplicate.
 * @return A new string that is a duplicate of the input string, or NULL if the input is NULL or memory allocation fails.
 */
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

/**
 * is_first_char_a_letter -
 * Checks if the first character of a string is a letter.
 *
 * @param str The input string to check.
 * @return FOUND (1) if the first character is a letter, NOT_FOUND (0) otherwise.
 */
int is_first_char_a_letter(const char* str) {
	if ((str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z')) {
		return FOUND;
	}
	else {
		return	NOT_FOUND;
	}
}

/**
 * is_first_char_quotation -
 * Checks if the first character of a string is a double quotation mark.
 *
 * @param str The input string to check.
 * @return FOUND (1) if the first character is a double quotation mark, NOT_FOUND (0) otherwise.
 */
int is_first_char_quotation(const char* str) {
	int length = strlen(str);
	if (str[0] == '"' && str[length - 1] == '"') {
		return FOUND; /* Return true if the symbol_name exists in the ext array*/
	}
	return NOT_FOUND;
}


