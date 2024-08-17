#include "symbols_manager.h"

/**
 * createSymbolsManager -
 * Creates and initializes a new SymbolsManager instance.
 *
 * This function allocates memory for a new SymbolsManager structure and initializes
 * its fields, including arrays for symbols, external symbols, entry symbols,
 * and reference symbols. It sets initial sizes and allocates memory accordingly.
 * If any memory allocation fails, the function logs an error and returns.
 *
 * @return SymbolsManager* A pointer to the newly created and initialized SymbolsManager instance.
 */
SymbolsManager* createSymbolsManager() {
	SymbolsManager* manager = (SymbolsManager*)malloc(sizeof(SymbolsManager));
	if (manager == NULL) {
		log_error("createSymbolsManager", 17, "symbols_manager.c", "Failed to create SymbolsManager");
		return NULL;
	}

	/* Initialize Symbols array*/
	manager->has_symbols_errors = NOT_FOUND;
	manager->array = (Symbols*)malloc(5 * sizeof(Symbols)); /* Initial size of 5*/
	/*Failed to allocate memory for Symbols array*/
	if (manager->array == NULL) {
		log_error("createSymbolsManager", 26, "symbols_manager.c", "Failed to allocate memory for Symbols array array");
		free(manager);
		return NULL;
	}
	/*Memory allocation succeeded*/
	manager->used = 0;
	manager->size = 5;

	/* Initialize ext array*/
	manager->ext = (char**)malloc(5 * sizeof(char*)); /* Initial size of 5*/

	/*Failed to allocate memory for ext array*/
	if (manager->ext == NULL) {
		log_error("createSymbolsManager", 39, "symbols_manager.c", "Failed to allocate memory for ext array");

		free(manager->array);
		free(manager);
		return NULL;
	}
	manager->ext_used = 0;
	manager->ext_size = 5;

	/* Initialize ent array*/
	manager->ent = (char**)malloc(5 * sizeof(char*)); /* Initial size of 5*/
	/*Failed to allocate memory for ent array*/
	if (manager->ent == NULL) {
		log_error("createSymbolsManager", 52, "symbols_manager.c", "Failed to allocate memory for ent array");

		manager->has_symbols_errors = FOUND;
		free(manager->ext);
		free(manager->array);
		free(manager);
		return NULL;
	}
	manager->ent_used = 0;
	manager->ent_size = 5;

	/* Initialize ref_symbols array*/
	manager->ref_symbols = (ReferenceSymbol*)malloc(5 * sizeof(ReferenceSymbol)); /* Initial size of 5*/
	/*Failed to allocate memory for ref_symbols array*/
	if (manager->ref_symbols == NULL) {
		log_error("createSymbolsManager", 67, "symbols_manager.c", "Failed to allocate memory for ref_symbols array");

		free(manager->ent);
		free(manager->ext);
		free(manager->array);
		free(manager);
		return NULL;
	}
	manager->ref_used = 0;
	manager->ref_size = 5;

	return manager;
}

/**
 * addSymbol -
 * Adds a new symbol to the SymbolsManager if all validation checks pass.
 *
 * @param macroManager The MacroManager instance used to check if the symbol is a macro name.
 * @param manager The SymbolsManager instance where the symbol will be added.
 * @param symbol_name The name of the symbol to add.
 * @param symbol_location The location of the symbol.
 * @param is_data Flag indicating if the symbol is data.
 * @param actions Action instance used for additional symbol name validation.
 */
void addSymbol(MacroManager* macroManager, SymbolsManager* manager, const char* symbol_name, int symbol_location, int is_data, Action* actions) {
	if (is_symbol_exists(manager, symbol_name)) {
		log_error("addSymbol", 94, "symbols_manager.c", "symbol already exists");
		manager->has_symbols_errors = FOUND;
	}
	else if (is_macro_name(macroManager, symbol_name)) {
		label_error("addSymbol", 95, "symbols_manager.c", "symbol cannot be a macro name", symbol_name);
		manager->has_symbols_errors = FOUND;

	}
	else if (!is_valid_symbol_name(manager, symbol_name, actions)) {
		label_error("addSymbol", 100, "symbols_manager.c", "symbol isnt valid", symbol_name);
		manager->has_symbols_errors = FOUND;

	}
	else if (!is_first_char_a_letter(symbol_name)) {
		label_error("addSymbol", 105, "symbols_manager.c", "symbol isnt valid", symbol_name);
		manager->has_symbols_errors = FOUND;
	}
	else {
		if (manager->used == manager->size) {
			Symbols* new_array;
			manager->size *= 2;
			new_array = (Symbols*)realloc(manager->array, manager->size * sizeof(Symbols));
			if (new_array == NULL) {
				log_error("addSymbol", 117, "symbols_manager.c", "Failed to reallocate memory for Symbols array");
				manager->has_symbols_errors = FOUND;
				return;
			}
			manager->array = new_array;
		}
		manager->array[manager->used].symbol_name = duplicate_string(symbol_name); /* Make a copy of the string*/
		if (manager->array[manager->used].symbol_name == NULL) {
			log_error("addSymbol", 125, "symbols_manager.c", "Failed to duplicate");
			manager->has_symbols_errors = FOUND;
			free(manager->array);
			free(manager);
			return;
		}
		manager->array[manager->used].symbol_location = symbol_location;
		manager->array[manager->used].is_data = is_data;
		manager->used++;
	}

}

/**
 * printSymbols -
 * Prints the symbols managed by the SymbolsManager in a formatted table.
 * Used only during work. Unnecessary for final work.
 *
 * @param manager The SymbolsManager instance containing the symbols to be printed.
 */
void printSymbols(const SymbolsManager* manager) {
	int i;
	printf("\n\n");
	printf("manager->Symbols\n");
	printf("| %-20s | %-10s | %-5s |\n", "Symbol Name", "Location", "Data");
	printf("|----------------------|------------|-------|\n");

	for (i = 0; i < manager->used; i++) {
		printf("| %-20s | %-10d | %-5d |\n", manager->array[i].symbol_name, manager->array[i].symbol_location, manager->array[i].is_data);
	}
}

/**
 * getSymbolLocation -
 * Retrieves the location of a symbol by its name.
 *
 * @param manager The SymbolsManager instance containing the symbols.
 * @param symbol_name The name of the symbol to find.
 * @return The location of the symbol if found, or -1 if the symbol is not found.
 */
int getSymbolLocation(const SymbolsManager* manager, const char* symbol_name) {
	int i;
	for (i = 0; i < manager->used; i++) {
		if (strcmp(manager->array[i].symbol_name, symbol_name) == 0) {
			return manager->array[i].symbol_location;
		}
	}
	return -1; /* Indicate that the symbol was not found*/
}

/**
 * destroySymbolsManager -
 * Frees all allocated memory associated with the SymbolsManager.
 *
 * @param manager The SymbolsManager instance to destroy.
 */
void destroySymbolsManager(SymbolsManager* manager) {
	int i;
	for (i = 0; i < manager->used; i++) {
		free(manager->array[i].symbol_name); /* Free the copied strings*/
	}
	for (i = 0; i < manager->ext_used; i++) {
		free(manager->ext[i]); /* Free the ext strings*/
	}
	for (i = 0; i < manager->ent_used; i++) {
		free(manager->ent[i]); /* Free the ent strings*/
	}
	free(manager->array);
	free(manager->ext);
	free(manager->ent);
	free(manager);
}

/**
 * addExtEnt -
 * Adds a symbol to the external or entry symbol lists in the SymbolsManager.
 *
 * @param manager Pointer to the SymbolsManager structure which manages the symbol lists.
 * @param value The symbol to be added to the symbol list.
 * @param is_ext Flag indicating whether the symbol is an external symbol (if true) or an entry symbol (if false).
 *
 */
void addExtEnt(SymbolsManager* manager, const char* value, int is_ext) {
	if (is_ext) {
		/* Handle addition of an external symbol*/
		if (isRefExtSymbolExists(manager, value)) {
			/*symbol already exists*/
			log_error("addExtEnt", 212, "symbols_manager.c", "symbol already exists");
			manager->has_symbols_errors = FOUND;
		}
		else {
			if (manager->ext_used == manager->ext_size) {
				char** new_ext;
				manager->ext_size *= 2;
				new_ext = (char**)realloc(manager->ext, manager->ext_size * sizeof(char*));
				if (new_ext == NULL) {
					log_error("addExtEnt", 221, "symbols_manager.c", "Failed to reallocate memory for ext array");
					free(manager->ext);
					free(manager->array);
					free(manager->ent);
					free(manager);
					return;
				}
				manager->ext = new_ext;
			}
			/* Duplicate the symbol value and add it to the external symbols array*/
			manager->ext[manager->ext_used] = duplicate_string(value); /* Make a copy of the string*/
			if (manager->ext[manager->ext_used] == NULL) {
				perror("Failed to duplicate value");
				free(manager->ext);
				free(manager->array);
				free(manager->ent);
				free(manager);
				return;
			}
			manager->ext_used++;
		}

	}
	else {
		/* Handle addition of an entry symbol*/
		if (isRefEntSymbolExists(manager, value)) {
			/*symbol already exists*/
			log_error("addExtEnt", 248, "symbols_manager.c", "symbol already exists");
			manager->has_symbols_errors = FOUND;
		}
		else {
			if (manager->ent_used == manager->ent_size) {
				char** new_ent;

				/* Double the size of the entry symbols array*/
				manager->ent_size *= 2;
				new_ent = (char**)realloc(manager->ent, manager->ent_size * sizeof(char*));
				if (new_ent == NULL) {
					perror("Failed to reallocate memory for ent array");
					free(manager->ent);
					free(manager->array);
					free(manager->ext);
					free(manager);
					return;
				}
				manager->ent = new_ent;
			}

			/* Duplicate the symbol value and add it to the entry symbols array*/
			manager->ent[manager->ent_used] = duplicate_string(value); /* Make a copy of the string*/
			if (manager->ent[manager->ent_used] == NULL) {
				log_error("addExtEnt", 272, "symbols_manager.c", "Failed to duplicate value");
				free(manager->ent);
				free(manager->array);
				free(manager->ext);
				free(manager);
				return;
			}
			manager->ent_used++;
		}
	}
}

/**
 * updateSymbolsTable -
 * Updates the symbols table based on the provided line of assembly code.
 *
 * @param macroManager Pointer to the MacroManager structure, used to check if the symbol is a macro name.
 * @param symbolsManager Pointer to the SymbolsManager structure, used to manage symbols and symbol lists.
 * @param line Array of strings representing a line of assembly code, where line[0] is the directive or symbol
 *             name and line[1] is the associated value or type.
 * @param location The current location in the assembly code, used to determine the symbol's location.
 * @param actions Pointer to the Action structure, used to verify the validity of actions associated with symbols.
 *
 * @return None
 */
void updateSymbolsTable(MacroManager* macroManager, SymbolsManager* symbolsManager, char** line, int location, Action* actions) {
	if (strcmp(line[0], ".extern") == 0) {
		addExtEnt(symbolsManager, line[1], FOUND);
	}
	else if (strcmp(line[0], ".entry") == 0) {
		addExtEnt(symbolsManager, line[1], NOT_FOUND);
	}
	else {
		if (isSymbolPattern(line[0])) {
			char* symbol_name = strtrimlast(line[0]);
			if (symbol_name == NULL) {
				log_error("updateSymbolsTable", 308, "symbols_manager.c", "Failed to duplicate symbol_name");
				symbolsManager->has_symbols_errors = FOUND;
				return;
			}

			if (action_exists(actions, line[1])) {
				addSymbol(macroManager, symbolsManager, symbol_name, location, NOT_FOUND, actions);
			}
			else if (strcmp(line[1], ".string") == 0 || strcmp(line[1], ".data") == 0) {
				addSymbol(macroManager, symbolsManager, symbol_name, location, FOUND, actions);
			}

			free(symbol_name);
		}
	}
}

/**
 * printExt -
 * Prints the list of external symbols stored in the SymbolsManager.
 * Used only for work, unnecessary for final work

 * @param manager Pointer to the SymbolsManager structure
 */
void printExt(const SymbolsManager* manager) {
	int i;
	printf("\n\n");
	printf("manager->ext\n");
	printf("| %-20s |\n", "Ext Name");
	printf("|----------------------|\n");

	for (i = 0; i < manager->ext_used; i++) {
		printf("| %-20s |\n", manager->ext[i]);
	}
}

/**
 * printEnt -
 * Prints the list of entry symbols stored in the SymbolsManager.
 * Used only for work, unnecessary for final work
 *
 * @param manager Pointer to the SymbolsManager structure
 */
void printEnt(const SymbolsManager* manager) {
	int i;
	printf("\n\n");
	printf("manager->ent\n");
	printf("| %-20s |\n", "Ent Name");
	printf("|----------------------|\n");

	for (i = 0; i < manager->ent_used; i++) {
		printf("| %-20s |\n", manager->ent[i]);
	}
}

/**
 * isSymbolPattern -
 * Checks if a given string follows the symbol pattern
 *
 * @param word The string to check.
 *
 * @return Returns `1` (true) if the string ends with a colon (`:`), indicating it follows the symbol pattern.
 *         Returns `0` (false) if the string does not end with a colon.
 */
int isSymbolPattern(const char* word) {
	return word[(strlen(word) - 1)] == ':';
}

/**
 * isDataPattern -
 * Checks if a string matches ".data" or ".string".
 *
 * @param word The string to check.
 * @return 1 if the string matches ".data" or ".string", otherwise 0.
 */
int isDataPattern(const char* word) {
	return strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0;
}

/**
 * isReferencePattern -
 * Checks if a string is ".extern" or ".entry".
 *
 * @param word The string to check.
 * @return 1 if the string matches ".extern" or ".entry", otherwise 0.
 */
int isReferencePattern(const char* word) {
	return strcmp(word, ".extern") == 0 || strcmp(word, ".entry") == 0;
}

/**
 * updateDataSymbolsLocation -
 * Updates the location of data symbols in the SymbolsManager.
 *
 * This function updates the location of symbols in the `SymbolsManager`'s `array` by adding a
 * certain number of steps to each symbol's location. The update is conditional based on whether
 * the symbol represents data.
 * For data symbols, the function adds a base value (100) plus the
 * specified number of steps to the symbol's current location.
 *
 * @param manager A pointer to a SymbolsManager instance that contains the array of symbols to be updated.
 * If this pointer or the `array` is NULL, the function returns immediately without making any changes.
 *
 * @param steps An integer representing the number of steps to be added to the location of data symbols.
 */
void updateDataSymbolsLocation(const SymbolsManager* manager, int steps) {
	int i;
	/* Check if the manager or the symbols array is NULL.
	 * If either is NULL, there is nothing to update, so return immediately.
	 */
	if (manager == NULL || manager->array == NULL) {
		return;
	}

	/*manager and symbols array are not NULL*/
	/* Iterate over each symbol in the array */
	for (i = 0; i < manager->used; ++i) {
		/* Update the location of the symbol by adding 100 plus the steps if it is a data symbol. */
		manager->array[i].symbol_location += (FIRST_MEMORY_PLACE + (manager->array[i].is_data ? steps : 0));
	}
}

int is_symbol_exists(const SymbolsManager* manager, const char* symbol_name) {
	int i;
	for (i = 0; i < manager->used; i++) {
		if (strcmp(manager->array[i].symbol_name, symbol_name) == 0) {
			return FOUND;
		}
	}
	return NOT_FOUND; /* Indicate that the symbol was not found*/
}

/**
 * addReferenceSymbol -
 * Adds a reference symbol to the SymbolsManager.
 *
 * @param manager The SymbolsManager instance to update.
 * @param name The name of the reference symbol.
 * @param location The location of the reference symbol.
 * @param type The type of the reference symbol (e.g., external or entry).
 */
void addReferenceSymbol(SymbolsManager* manager, const char* name, int location, int type) {
	ReferenceSymbol* new_ref_symbols;
	if (manager->ref_used == manager->ref_size) {
		manager->ref_size *= 2;
		new_ref_symbols = (ReferenceSymbol*)realloc(manager->ref_symbols, manager->ref_size * sizeof(ReferenceSymbol));
		if (new_ref_symbols == NULL) {
			log_error("addReferenceSymbol", 455, "symbols_manager.c", "Failed to reallocate memory for ReferenceSymbol array");
			manager->has_symbols_errors = FOUND;
			free(manager->ref_symbols);
			free(manager->ent);
			free(manager->ext);
			free(manager->array);
			free(manager);
			return;
		}
		manager->ref_symbols = new_ref_symbols;
	}
	manager->ref_symbols[manager->ref_used].name = duplicate_string(name);
	if (manager->ref_symbols[manager->ref_used].name == NULL) {
		log_error("addReferenceSymbol", 468, "symbols_manager.c", "Failed to duplicate name");

		manager->has_symbols_errors = FOUND;
		free(manager->ref_symbols);
		free(manager->ent);
		free(manager->ext);
		free(manager->array);
		free(manager);
		return;
	}
	manager->ref_symbols[manager->ref_used].location = location;
	manager->ref_symbols[manager->ref_used].type = type;
	manager->ref_used++;
}

/**
 * printReferenceSymbols -
 * Prints the list of reference symbols.
 * Used only for work, unnecessary for final work
 *
 * @param manager The SymbolsManager instance containing the reference symbols to print.
 */
void printReferenceSymbols(const SymbolsManager* manager) {
	int i;
	printf("\n\n");
	printf("manager->ReferenceSymbols\n");
	printf("| %-20s | %-10s | %-5s |\n", "Name", "Location", "Type");
	printf("|----------------------|------------|-------|\n");

	for (i = 0; i < manager->ref_used; i++) {
		printf("| %-20s | %-10d | %-5d |\n", manager->ref_symbols[i].name, manager->ref_symbols[i].location, manager->ref_symbols[i].type);
	}
}

/**
 * isRefExtSymbolExists -
 * Checks if a symbol exists in the external symbols list.
 *
 * @param manager The SymbolsManager instance to search within.
 * @param symbol_name The symbol name to look for.
 * @return `FOUND` if the symbol is found, `NOT_FOUND` if it is not found or if input is invalid.
 */
int isRefExtSymbolExists(const SymbolsManager* manager, const char* symbol_name) {
	int i;
	if (manager == NULL || symbol_name == NULL) {
		return NOT_FOUND; /* Return 0 if the manager or symbol_name is NULL*/
	}

	for (i = 0; i < manager->ext_used; i++) {
		if (strcmp(manager->ext[i], symbol_name) == 0) {
			return FOUND; /* Return true if the symbol_name exists in the ext array*/
		}
	}

	return NOT_FOUND; /* Return false if the symbol_name does not exist in the ext array*/
}

/**
 * isRefEntSymbolExists -
 * Checks if a symbol exists in the entry symbols list.
 *
 * @param manager The SymbolsManager instance to search within.
 * @param symbol_name The symbol name to look for.
 * @return `FOUND` if the symbol is found, `NOT_FOUND` if it is not found or if input is invalid.
 */
int isRefEntSymbolExists(const SymbolsManager* manager, const char* symbol_name) {
	int i;

	if (manager == NULL || symbol_name == NULL) {
		return NOT_FOUND; /* Return 0 if the manager or symbol_name is NULL*/
	}

	for (i = 0; i < manager->ent_used; i++) {
		if (strcmp(manager->ent[i], symbol_name) == 0) {
			return FOUND; /* Return true if the symbol_name exists in the ext array*/
		}
	}

	return NOT_FOUND; /* Return false if the symbol_name does not exist in the ext array*/
}

/**
 * is_valid_symbol_name -
 * Validates a symbol name based on predefined criteria:
 * - It does not exceed the maximum allowed length (`MAX_SYMBOL_NAME_LENGTH`).
 * - It is not an existing action name.
 * - It is not a valid register name.
 *
 * @param manager The SymbolsManager instance (currently unused).
 * @param symbol_name The name of the symbol to validate.
 * @param actions The Action instance used to check for existing actions.
 * @return `FOUND` if the symbol name is valid, `NOT_FOUND` otherwise.
 */
int is_valid_symbol_name(const SymbolsManager* manager, char* symbol_name, Action* actions) {
	if (strlen(symbol_name) > MAX_SYMBOL_NAME_LENGTH) {
		return NOT_FOUND;
	}

	if (action_exists(actions, symbol_name)) {
		return NOT_FOUND;
	}
	if (is_valid_register(symbol_name)) {
		return NOT_FOUND;
	}
	return FOUND;
}




