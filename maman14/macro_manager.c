#include "macro_manager.h"

void init_macro_manager(MacroManager* manager) {
	manager->macro_count = 0;
	manager->is_macro_context = 0;
}


/**
 *process_file_line - 
 * Processes a line of input, checking for macro definitions and expansions.
 *
 * @param manager A pointer to the MacroManager structure used to manage macro definitions and expansions.
 * @param input A pointer to an array of strings (char**) representing the tokens of the input line.
 * @param input_count The number of tokens in the input line.
 *
 * @return A processed line as an array of strings (char**). If the line defines or is part of a macro, the function returns NULL.
 */
char** process_file_line(MacroManager* manager, char** input, int input_count) {
	int i, j;
	char** processed_line;

	/* If the input line is empty, return NULL */
	if (input_count == 0) return NULL;

    /* If the input line ends a macro definition, update the macro context and return NULL */
	if (strcmp(input[0], "endmacr") == 0) {
		manager->is_macro_context = 0;
		return NULL;
	}

    /* If in the middle of a macro definition, add the line to the current macro's command list */
	if (manager->is_macro_context) {
		for (i = 0; i < manager->macro_count; ++i) {
			if (strcmp(manager->macro_names[i], manager->current_macro_name) == 0) {
				Macro* macro = &manager->macros[i];
				macro->commands = realloc(macro->commands, (macro->row_count + 1) * sizeof(char**));
				if (macro->commands == NULL) {
					/* Handle allocation failure*/
					LOG_ERROR("Memory allocation failed");
					return NULL;
				}
				macro->commands[macro->row_count] = malloc((input_count + 1) * sizeof(char*));
				if (macro->commands[macro->row_count] == NULL) {
					/* Handle allocation failure*/
					LOG_ERROR("Memory allocation failed");
					return NULL;
				}
				for (j = 0; j < input_count; ++j) {
					macro->commands[macro->row_count][j] = malloc((strlen(input[j]) + 1) * sizeof(char));
					if (macro->commands[macro->row_count][j] == NULL) {
						/* Handle allocation failure*/
						LOG_ERROR("Memory allocation failed");
						return NULL;
					}
					strcpy(macro->commands[macro->row_count][j], input[j]);
				}
				macro->commands[macro->row_count][input_count] = NULL; /* Null-terminate the row*/
				macro->row_count++;
				return NULL;
			}
		}
	}

    /* If the input line defines a new macro, initialize it in the MacroManager */
	if (strcmp(input[0], "macr") == 0) {
		manager->is_macro_context = 1;
		strcpy(manager->current_macro_name, input[1]);
		manager->macro_names[manager->macro_count] = malloc((strlen(input[1]) + 1) * sizeof(char));
		if (manager->macro_names[manager->macro_count] == NULL) {
			/* Handle allocation failure*/
			LOG_ERROR("Memory allocation failed");
			return NULL;
		}
		strcpy(manager->macro_names[manager->macro_count], input[1]);
		manager->macros[manager->macro_count].commands = NULL;
		manager->macros[manager->macro_count].row_count = 0;
		manager->macro_count++;
		return NULL;
	}

	for (i = 0; i < manager->macro_count; ++i) {
		if (strcmp(manager->macro_names[i], input[0]) == 0) {
			Macro* macro = &manager->macros[i];
			char*** result = malloc((macro->row_count + 1) * sizeof(char**));
			if (result == NULL) {
				/* Handle allocation failure*/
				LOG_ERROR("Memory allocation failed");
				return NULL;
			}
			for (j = 0; j < macro->row_count; ++j) {
				int k, row_length = 0;
				for (k = 0; macro->commands[j][k] != NULL; ++k) {
					row_length++;
				}
				result[j] = malloc((row_length + 1) * sizeof(char*));
				if (result[j] == NULL) {
					/* Handle allocation failure*/
					LOG_ERROR("Memory allocation failed");	
					return NULL;
				}
				for (k = 0; macro->commands[j][k] != NULL; ++k) {
					result[j][k] = malloc((strlen(macro->commands[j][k]) + 1) * sizeof(char));
					if (result[j][k] == NULL) {
						/* Handle allocation failure*/
						LOG_ERROR("Memory allocation failed");
						return NULL;
					}
					strcpy(result[j][k], macro->commands[j][k]);
				}
				result[j][row_length] = NULL; /* Null-terminate the row*/
			}
			result[macro->row_count] = NULL; /* Null-terminate the result array*/
			return (char**)result;
		}
	}


	/* Allocate new memory for processed_line*/
	processed_line = malloc((input_count + 1) * sizeof(char*));
	if (processed_line == NULL) {
		LOG_ERROR("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < input_count; ++i) {
		processed_line[i] = malloc((strlen(input[i]) + 1) * sizeof(char));
		if (processed_line[i] == NULL) {
			LOG_ERROR("Memory allocation failed");
			exit(EXIT_FAILURE);
		}
		strcpy(processed_line[i], input[i]);
	}
	processed_line[input_count] = NULL; /* Null-terminate the processed line*/

	return processed_line;
}

/**
 *is_macro_name - 
 * Checks if the given name is a macro name managed by the MacroManager.
 *
 * @param manager A pointer to the MacroManager structure that contains macro names.
 * @param name The name to be checked against the list of macro names.
 *
 * @return 1 if the name matches a macro name in the MacroManager, 0 otherwise.
 */
int is_macro_name(MacroManager* manager, const char* name) {
	int i;
	for (i = 0; i < manager->macro_count; ++i) {
		if (strcmp(manager->macro_names[i], name) == 0) {
			return FOUND; /* Name matches a macro name*/
		}
	}
	return NOT_FOUND; /*Name does not match any macro name*/
}

char*** get_macro_content(MacroManager* manager, const char* macro_name) {
	int i, j;
	for (i = 0; i < manager->macro_count; ++i) {
		if (strcmp(manager->macro_names[i], macro_name) == 0) {
			Macro* macro = &manager->macros[i];
			char*** result = malloc((macro->row_count + 1) * sizeof(char**));
			if (result == NULL) {
				/* Handle allocation failure*/
				return NULL;
			}
			for (j = 0; j < macro->row_count; ++j) {
				int k, row_length = 0;
				for (k = 0; macro->commands[j][k] != NULL; ++k) {
					row_length++;
				}
				result[j] = malloc((row_length + 1) * sizeof(char*));
				if (result[j] == NULL) {
					/* Handle allocation failure*/
					return NULL;
				}
				for (k = 0; macro->commands[j][k] != NULL; ++k) {
					result[j][k] = malloc((strlen(macro->commands[j][k]) + 1) * sizeof(char));
					if (result[j][k] == NULL) {
						/* Handle allocation failure*/
						return NULL;
					}
					strcpy(result[j][k], macro->commands[j][k]);
				}
				result[j][row_length] = NULL; /* Null-terminate the row*/
			}
			result[macro->row_count] = NULL; /* Null-terminate the result array*/
			return result;
		}
	}
 /* Return NULL if the macro_name was not found */
    return NULL;
}


void free_macro_manager(MacroManager* manager) {
	int i, j, k;
	for (i = 0; i < manager->macro_count; ++i) {
		for (j = 0; j < manager->macros[i].row_count; ++j) {
			for (k = 0; manager->macros[i].commands[j][k] != NULL; ++k) {
				free(manager->macros[i].commands[j][k]);
			}
			free(manager->macros[i].commands[j]);
		}
		free(manager->macros[i].commands);
		free(manager->macro_names[i]);
	}
}

