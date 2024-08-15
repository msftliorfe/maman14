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
		LOG_ERROR("Failed to create SymbolsManager");
		return NULL;
	}

	/* Initialize Symbols array*/
	manager->has_symbols_errors = NOT_FOUND;
	manager->array = (Symbols*)malloc(5 * sizeof(Symbols)); /* Initial size of 5*/
	/*Failed to allocate memory for Symbols array*/
	if (manager->array == NULL) {
		LOG_ERROR("Failed to allocate memory for Symbols array");
		free(manager);
		return NULL;
	}
	/*Memory allocation succeeded*/
	manager->has_symbols_errors = FOUND;
	manager->used = 0;
	manager->size = 5;

	/* Initialize ext array*/
	manager->ext = (char**)malloc(5 * sizeof(char*)); /* Initial size of 5*/

	/*Failed to allocate memory for ext array*/
	if (manager->ext == NULL) {
		LOG_ERROR("Failed to allocate memory for ext array");
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
		LOG_ERROR("Failed to allocate memory for ent array");
		manager->has_symbols_errors = NOT_FOUND;
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
		LOG_ERROR("Failed to allocate memory for ref_symbols array");
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


void addSymbol(MacroManager* macroManager, SymbolsManager* manager, const char* symbol_name, int symbol_location, int is_data) {
	if (is_symbol_exists(manager, symbol_name)) {
		LOG_ERROR("symbol already exists");
	}
	if (is_macro_name(macroManager, symbol_name)) {
		LABEL_ERROR("symbol cannot be a macro name",symbol_name);
	}
	else {
		if (manager->used == manager->size) {
			Symbols* new_array;
			manager->size *= 2;
			new_array = (Symbols*)realloc(manager->array, manager->size * sizeof(Symbols));
			if (new_array == NULL) {
				perror("Failed to reallocate memory for Symbols array");
				free(manager->array);
				free(manager);
				exit(EXIT_FAILURE);
			}
			manager->array = new_array;
		}
		manager->array[manager->used].symbol_name = duplicate_string(symbol_name); /* Make a copy of the string*/
		if (manager->array[manager->used].symbol_name == NULL) {
			perror("Failed to duplicate symbol_name");
			free(manager->array);
			free(manager);
			exit(EXIT_FAILURE);
		}
		manager->array[manager->used].symbol_location = symbol_location;
		manager->array[manager->used].is_data = is_data;
		manager->used++;
	}

}

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

int getSymbolLocation(const SymbolsManager* manager, const char* symbol_name) {
	int i;
	for (i = 0; i < manager->used; i++) {
		if (strcmp(manager->array[i].symbol_name, symbol_name) == 0) {
			return manager->array[i].symbol_location;
		}
	}
	return -1; /* Indicate that the symbol was not found*/
}

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

void addExtEnt(SymbolsManager* manager, const char* value, int is_ext) {
	if (is_ext) {
		if (isRefExtSymbolExists(manager, value)) {
			LOG_ERROR("symbol already exists");
			manager->has_symbols_errors = NOT_FOUND;
		}
		else {
			if (manager->ext_used == manager->ext_size) {
				char** new_ext;
				manager->ext_size *= 2;
				new_ext = (char**)realloc(manager->ext, manager->ext_size * sizeof(char*));
				if (new_ext == NULL) {
					LOG_ERROR("Failed to reallocate memory for ext array");
					free(manager->ext);
					free(manager->array);
					free(manager->ent);
					free(manager);
					return;
				}
				manager->ext = new_ext;
			}
			manager->ext[manager->ext_used] = duplicate_string(value); /* Make a copy of the string*/
			if (manager->ext[manager->ext_used] == NULL) {
				perror("Failed to duplicate value");
				free(manager->ext);
				free(manager->array);
				free(manager->ent);
				free(manager);
				exit(EXIT_FAILURE);
			}
			manager->ext_used++;
		}

	}
	else {
		if (isRefEntSymbolExists(manager, value)) {
			LOG_ERROR("symbol already exists");
			manager->has_symbols_errors = NOT_FOUND;
		}
		else {
			if (manager->ent_used == manager->ent_size) {
				char** new_ent;
				manager->ent_size *= 2;
				new_ent = (char**)realloc(manager->ent, manager->ent_size * sizeof(char*));
				if (new_ent == NULL) {
					perror("Failed to reallocate memory for ent array");
					free(manager->ent);
					free(manager->array);
					free(manager->ext);
					free(manager);
					exit(EXIT_FAILURE);
				}
				manager->ent = new_ent;
			}
			manager->ent[manager->ent_used] = duplicate_string(value); /* Make a copy of the string*/
			if (manager->ent[manager->ent_used] == NULL) {
				perror("Failed to duplicate value");
				free(manager->ent);
				free(manager->array);
				free(manager->ext);
				free(manager);
				exit(EXIT_FAILURE);
			}
			manager->ent_used++;
		}

	}
}


void updateSymbolsTable(MacroManager* macroManager, SymbolsManager* symbolsManager, char** line, int location) {
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
				perror("Failed to duplicate symbol_name");
				exit(EXIT_FAILURE);
			}

			if (action_exists(line[1])) {
				addSymbol(macroManager, symbolsManager, symbol_name, location, NOT_FOUND);
			}
			else if (strcmp(line[1], ".string") == 0 || strcmp(line[1], ".data") == 0) {
				addSymbol(macroManager, symbolsManager, symbol_name, location, FOUND);
			}

			free(symbol_name);
		}
	}
}

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

int isSymbolPattern(const char* word) {
	return word[(strlen(word) - 1)] == ':';
}

int isDataPattern(const char* word) {
	return strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0;
}

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

/* Adding the function to add a reference symbol*/
void addReferenceSymbol(SymbolsManager* manager, const char* name, int location, int type) {
	ReferenceSymbol* new_ref_symbols;
	if (manager->ref_used == manager->ref_size) {
		manager->ref_size *= 2;
		new_ref_symbols = (ReferenceSymbol*)realloc(manager->ref_symbols, manager->ref_size * sizeof(ReferenceSymbol));
		if (new_ref_symbols == NULL) {
			perror("Failed to reallocate memory for ReferenceSymbol array");
			free(manager->ref_symbols);
			free(manager->ent);
			free(manager->ext);
			free(manager->array);
			free(manager);
			exit(EXIT_FAILURE);
		}
		manager->ref_symbols = new_ref_symbols;
	}
	manager->ref_symbols[manager->ref_used].name = duplicate_string(name);
	if (manager->ref_symbols[manager->ref_used].name == NULL) {
		perror("Failed to duplicate name");
		free(manager->ref_symbols);
		free(manager->ent);
		free(manager->ext);
		free(manager->array);
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->ref_symbols[manager->ref_used].location = location;
	manager->ref_symbols[manager->ref_used].type = type;
	manager->ref_used++;
}

/* Adding the function to print all reference symbols*/
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




