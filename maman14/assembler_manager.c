#include "assembler_manager.h"

/**
 * createAssemblerManager -
 * Creates and initializes a new AssemblerManager instance.
 *
 * @return AssemblerManager* A pointer to the newly created and initialized AssemblerManager instance.
 */
AssemblerManager* createAssemblerManager() {
	AssemblerManager* manager = (AssemblerManager*)malloc(sizeof(AssemblerManager));
	/*Failed to create AssemblerManager*/
	if (manager == NULL) {
		LOG_ERROR("Failed to create AssemblerManager");
		return;
	}
	/*created AssemblerManager successfully*/
	manager->IC = 0;
	manager->DC = 0;
	manager->has_assembler_errors = 1;
	manager->dataItems = NULL;
	manager->dataItemCount = 0;
	manager->actionItems = NULL;
	manager->actionItemCount = 0;
	return manager;
}

void destroyAssemblerManager(AssemblerManager* manager) {
	free(manager->dataItems);
	free(manager->actionItems);
	free(manager);
}

/**
 * first_scan -
 * Performs the first scan of the file data and updates the symbol and assembler managers.
 *
 * This function iterates through each row of data in the FileManager's `post_macro` array and
 * performs processing based on the type of pattern detected in each row. It updates the SymbolsManager
 * with information about symbols, actions, data, and references and updates the AssemblerManager accordingly.
 *
 * @param fileManager A pointer to a FileManager instance containing the data to be scanned. The `post_macro` field
 * is expected to be an array of strings, each representing a line of the file.
 *
 * @param assemblerManager A pointer to an AssemblerManager instance that keeps track of the current instruction counter (IC)
 * and data counter (DC). This is used to update counters based on the type of data encountered.
 *
 * @param symbolsManager A pointer to a SymbolsManager instance used to manage and update symbol-related information
 * based on the patterns detected in the file data.
 */
void first_scan(MacroManager* macroManager, FileManager* fileManager, AssemblerManager* assemblerManager, SymbolsManager* symbolsManager) {
	int i;
	/* Iterate through each row of the file data */
	for (i = 0; i < fileManager->row_count; ++i) {
		char** line = fileManager->post_macro[i];
		/*If the line starts with ; it's a comment move to next line*/
		if (strcmp(line[0], ";") == 0 || strcmp(line[0], "file") == 0) {
			continue;
		}

		/* If the pattern is a reference, update the symbol table with a reference */
		if (isReferencePattern(line[0])) {
			updateSymbolsTable(macroManager, symbolsManager, line, -1);
		}
		/* If the pattern is a symbol and followed by data, update symbol table and process data */
		else if (isSymbolPattern(line[0])) {
			if (isDataPattern(line[1])) {
				updateSymbolsTable(macroManager, symbolsManager, line, assemblerManager->DC);
				processDataLine(line + 1, assemblerManager);
			}
			/* If the pattern is a symbol and followed by an action, update symbol table and process action */
			else if (action_exists(line[1])) {
				updateSymbolsTable(macroManager, symbolsManager, line, assemblerManager->IC);
				processActionLine(line + 1, assemblerManager);
			}
			else { /*action doesnt exists in allowed actions list*/
				LABEL_ERROR("This action doesn't exists", line[1]);
			}
		}
		/* If the pattern is an action, process the action line */
		else if (action_exists(line[0])) {
			processActionLine(line, assemblerManager);
		}
		/* If the pattern is data, process the data line */
		else if (isDataPattern(line[0])) {
			processDataLine(line, assemblerManager);
		}
		else {
			LABEL_ERROR("This action doesn't exists", line[1]);
		}
	}
}

void processActionLine(char** line, AssemblerManager* assemblerManager) {
	int reg_dest_was_handled = NOT_FOUND;
	char* first_line = process_first_line(line);
	char* action_name = clone_string(line[0]);
	char* source_operands = get_source_operands(action_name);
	char* destination_operands = get_destination_operands(action_name);
	int has_source_operands = strcmp(source_operands, "-1") != 0;
	int has_dest_operands = strcmp(destination_operands, "-1") != 0;
	int source_reg_num, dest_reg_num, number, location_of_current_operand;
	char* line_to_add;


	addActionItem(assemblerManager, line[0], assemblerManager->IC, first_line);
	if (has_source_operands) { /* there are source_operands for this action*/
		AddressingType	addressing_type_source = get_addressing_type(line[1]);
		if (addressing_type_source == DirectRegister || addressing_type_source == IndirectRegister) {
			source_reg_num = addressing_type_source == DirectRegister ? line[1][1] - '0' : line[1][2] - '0';
			if (has_dest_operands) {
				AddressingType	addressing_type_dest = get_addressing_type(line[2]);
				if (addressing_type_dest == DirectRegister || addressing_type_dest == IndirectRegister) {
					reg_dest_was_handled = FOUND;
					dest_reg_num = addressing_type_dest == DirectRegister ? line[2][1] - '0' : line[2][2] - '0';
					line_to_add = generate_combined_register_line(source_reg_num, dest_reg_num); /* pass the reg number without * sign*/
					addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);

				}
				else {/*DirectRegister or IndirectRegister in source and there is a dest but not DirectRegister or IndirectRegister in dest*/
					line_to_add = generate_single_register_line(source_reg_num, FOUND); /* pass the reg number without * sign*/
					addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
				}
			}
			else { /* DirectRegister or IndirectRegister in source but no dest operands at all*/
				line_to_add = generate_single_register_line(source_reg_num, FOUND); /* pass the reg number without * sign*/
				addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
			}
		}
		else { /* not DirectRegister or IndirectRegister in source only Immediate or Direct*/
			switch (addressing_type_source)
			{
			case Immediate: {
				number = atoi(line[1] + 1);
				line_to_add = generate_immediate_line(number);
				addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
				break;
			}
			case Direct: {
				addActionItem(assemblerManager, "LABEL", assemblerManager->IC, line[1]);
				break;
			}

			}
		}
	}
	if (has_dest_operands) { /* handle dest operands, but skip DirectRegister and IndirectRegister if already handled*/
		location_of_current_operand = has_source_operands ? 2 : 1;
		AddressingType	addressing_type_dest = get_addressing_type(line[location_of_current_operand]);
		switch (addressing_type_dest)
		{
		case Immediate: {
			number = atoi(line[location_of_current_operand] + 1);
			line_to_add = generate_immediate_line(number);
			addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
			break;
		}
		case Direct: {
			addActionItem(assemblerManager, "LABEL", assemblerManager->IC, line[location_of_current_operand]);
			break;
		}
		default: {
			if (!reg_dest_was_handled) { /* combined row was handled before*/
				dest_reg_num = addressing_type_dest == DirectRegister ? line[location_of_current_operand][1] - '0' : line[location_of_current_operand][2] - '0';
				line_to_add = generate_single_register_line(dest_reg_num, NOT_FOUND); /* pass the reg number without * sign*/
				addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
			}
		}
			   break;
		}
	}
}


void processDataLine(char** line, AssemblerManager* assemblerManager) {
	char** data_lines = generateDataLine(line);
	int count = 0;

	while (data_lines[count] != NULL) {
		addDataItem(assemblerManager, assemblerManager->DC, data_lines[count]);
		count++;
		assemblerManager->DC++;
	}
}

void addDataItem(AssemblerManager* manager, int location, const char* value) {
	manager->dataItems = (Item*)realloc(manager->dataItems, (manager->dataItemCount + 1) * sizeof(Item));
	if (manager->dataItems == NULL) {
		perror("Failed to add data item");
		exit(EXIT_FAILURE);
	}
	manager->dataItems[manager->dataItemCount].location = location;
	manager->dataItems[manager->dataItemCount].octal = bitStringToOctal(value);
	strncpy(manager->dataItems[manager->dataItemCount].value, value, WORD_SIZE_IN_BITS);
	manager->dataItems[manager->dataItemCount].value[WORD_SIZE_IN_BITS] = '\0';
	manager->dataItemCount++;
}

void addActionItem(AssemblerManager* manager, char* metadata, int location, const char* value) {
	manager->actionItems = (Item*)realloc(manager->actionItems, (manager->actionItemCount + 1) * sizeof(Item));
	if (manager->actionItems == NULL) {
		perror("Failed to add action item");
		exit(EXIT_FAILURE);
	}
	manager->actionItems[manager->actionItemCount].location = 100 + location;
	manager->actionItems[manager->actionItemCount].octal = bitStringToOctal(value);
	strncpy(manager->actionItems[manager->actionItemCount].value, value, WORD_SIZE_IN_BITS);
	manager->actionItems[manager->actionItemCount].value[WORD_SIZE_IN_BITS] = '\0';
	manager->actionItems[manager->actionItemCount].metadata = metadata;
	manager->actionItemCount++;
	manager->IC++;

}

void printItems(const Item* items, int itemCount, int includeMetadata) {
	int i;
	if (includeMetadata) {
		printf("| Location | Value           | Metadata    | Octal |\n");
		printf("|----------|-----------------|-------------|-------|\n");

		for (i = 0; i < itemCount; ++i) {
			printf("| %8d | %-15s | %-11s | %5s |\n", items[i].location, items[i].value, items[i].metadata ? items[i].metadata : "", items[i].octal);
		}
	}
	else {
		printf("| Location | Value           | Octal |\n");
		printf("|----------|-----------------|-------|\n");

		for (i = 0; i < itemCount; ++i) {
			printf("| %8d | %-15s | %5s |\n", items[i].location, items[i].value, items[i].octal);
		}
	}
}



void printDataItems(const AssemblerManager* manager) {
	printf("\n\n");

	printf("DataItems\n");
	printItems(manager->dataItems, manager->dataItemCount, NOT_FOUND);
}

void printActionItems(const AssemblerManager* manager) {
	printf("\n\n");

	printf("ActionItems\n");
	printItems(manager->actionItems, manager->actionItemCount, FOUND);
}

void updateLocationDataSymbols(const SymbolsManager* symbolsManager, const AssemblerManager* manager) {
	updateDataSymbolsLocation(symbolsManager, manager->IC);
}

/**
 * updateDataItemsLocation -
 * Updates the location of data items in the AssemblerManager.
 *
 * This function iterates through all data items managed by the AssemblerManager and updates their
 * location by adding a base offset of 100 plus the current instruction counter (IC). This helps in
 * adjusting the positions of data items relative to the current state of the assembler.
 *
 * @param manager A pointer to an AssemblerManager instance that contains the data items whose locations are to be updated.
 * The function assumes that `manager` and `manager->dataItems` are not NULL and that `dataItemCount`
 * correctly reflects the number of data items.
 */
void updateDataItemsLocation(const AssemblerManager* manager) {
	int i;
	/* Iterate over each data item in the manager's dataItems array */
	for (i = 0; i < manager->dataItemCount; ++i) {
		/* Update the location of each data item by adding 100 plus the current instruction counter (IC) */
		manager->dataItems[i].location += FIRST_MEMORY_PLACE + manager->IC;
	}
}
/**
 * second_scan -
 * Processes and updates action items and entry symbols during the second scan.
 *
 * This function performs a second scan over the action items in the AssemblerManager and updates
 * their metadata and values based on the symbols found in the SymbolsManager. It also processes
 * entry symbols and updates the reference symbols accordingly.
 *
 * @param assemblerManager A pointer to an AssemblerManager instance containing action items that need to be processed.
 * The function updates each action item's metadata and value based on its corresponding symbol.
 *
 * @param symbolsManager A pointer to a SymbolsManager instance containing symbol information used for updating action items
 * and handling entry symbols. The function uses this to check symbol existence and retrieve locations.
 */
void second_scan(AssemblerManager* assemblerManager, SymbolsManager* symbolsManager) {
	int i;
	/* Process each action item in the assemblerManager */
	for (i = 0; i < assemblerManager->actionItemCount; ++i) {
		Item* actionItem = &assemblerManager->actionItems[i];

		/* Check if the metadata indicates this action item is a label */
		if (strcmp(actionItem->metadata, "LABEL") == 0) {
			// Update metadata to be the value of the action item
			actionItem->metadata = duplicate_string(actionItem->value);  /* Update metadata to be the value*/

			/* Check if the value of the action item is an external symbol */
			if (isRefExtSymbolExists(symbolsManager, actionItem->value)) {/* this is an ext label*/
				// Convert the location to a 15-bit two's complement string
				char* location_str = int_to_15bit_twos_complement(1);

				// Add a new reference symbol to the SymbolsManager
				addReferenceSymbol(symbolsManager, actionItem->value, actionItem->location, FOUND); /* add new item to ref_symbols*/
				/* Copy the new string into the value array*/
				strncpy(actionItem->value, location_str, sizeof(actionItem->value) - 1);
				actionItem->value[sizeof(actionItem->value) - 1] = '\0';  /* Ensure null-termination*/

				// Convert the bit string to an octal representation
				actionItem->octal = bitStringToOctal(location_str);

				// Free the dynamically allocated string
				free(location_str);
			}
			else { /* this is ent symbol or just symbol - find its location in symbols table*/
				int symbol_location = getSymbolLocation(symbolsManager, actionItem->value);

				// Generate a direct line string representation of the symbol location
				char* location_str = generate_direct_line(symbol_location);
				/* Copy the new string into the value array*/
				strncpy(actionItem->value, location_str, sizeof(actionItem->value) - 1);

				// Convert the bit string to an octal representation
				actionItem->octal = bitStringToOctal(location_str);

				// Free the dynamically allocated string
				free(location_str);
			}
		}
	}

	/* Process each entry symbol */
	for (i = 0; i < symbolsManager->ent_used; ++i) {/* handle entry symbols*/
		char* entlItem = symbolsManager->ent[i]; /*Get the current entry symbol*/

		/* Find the location of the entry symbol in the symbols table*/
		int symbol_location = getSymbolLocation(symbolsManager, entlItem);

		/* Generate a direct line string representation of the symbol location*/
		char* location_str = generate_direct_line(symbol_location);

		/* Add a reference symbol for the entry item to the SymbolsManager*/
		addReferenceSymbol(symbolsManager, entlItem, symbol_location, NOT_FOUND); /* add new item to ref_symbols*/
		/* Free the dynamically allocated string*/
		free(location_str);
	}
}

void printObjToFile(char* file_name, const AssemblerManager* assemblerManager) {
	int i;
	int len;
	char* new_file_path;

	/*Concatenate extension string to the name of the file*/
	len = strlen(file_name) + strlen(OBJECTS_FILE_EXTENSION) + 1;
	new_file_path = malloc(len);

	if (new_file_path == NULL) {
		LOG_ERROR("Failed to allocate memory");
		return NOT_FOUND;
	}

	strcpy(new_file_path, file_name);
	strcat(new_file_path, OBJECTS_FILE_EXTENSION);
	FILE* file = fopen(new_file_path, "w");
	if (file == NULL) {
		perror("Failed to open file ps.obj");
		exit(EXIT_FAILURE);
	}

	/* Print the first line: IC tab_space DC*/
	fprintf(file, "%d\t%d\n", assemblerManager->IC, assemblerManager->DC);

	/* Print actionItems*/
	for (i = 0; i < assemblerManager->actionItemCount; ++i) {
		fprintf(file, "%d\t%s\n", assemblerManager->actionItems[i].location, assemblerManager->actionItems[i].octal);
	}

	/* Print dataItems*/
	for (i = 0; i < assemblerManager->dataItemCount; ++i) {
		fprintf(file, "%d\t%s\n", assemblerManager->dataItems[i].location, assemblerManager->dataItems[i].octal);
	}

	fclose(file);
}

void printReferenceSymbolsToFile(char* file_name, const SymbolsManager* manager) {
	int len;
	char* new_file_path;
	FILE* ent_file = NULL;
	FILE* ext_file = NULL;
	int ent_has_values = NOT_FOUND;
	int ext_has_values = NOT_FOUND;
	int i;

	for (i = 0; i < manager->ref_used; ++i) {
		ReferenceSymbol* ref_symbol = &manager->ref_symbols[i];
		if (ref_symbol->type) {
			if (!ext_has_values) {
				/*Concatenate extension string to the name of the file*/
				len = strlen(file_name) + strlen(EXTERNALS_FILE_EXTENSION) + 1;
				new_file_path = malloc(len);

				if (new_file_path == NULL) {
					LOG_ERROR("Failed to allocate memory");
					return NOT_FOUND;
				}

				strcpy(new_file_path, file_name);
				strcat(new_file_path, EXTERNALS_FILE_EXTENSION);
				ext_file = fopen(new_file_path, "w");
				if (ext_file == NULL) {
					perror("Failed to open file ps.ext");
					exit(EXIT_FAILURE);
				}
				ext_has_values = 1;
			}
			fprintf(ext_file, "%s\t%d\n", ref_symbol->name, ref_symbol->location);
		}
		else {
			if (!ent_has_values) {
				/*Concatenate extension string to the name of the file*/
				len = strlen(file_name) + strlen(ENTRY_FILE_EXTENSION) + 1;
				new_file_path = malloc(len);

				if (new_file_path == NULL) {
					LOG_ERROR("Failed to allocate memory");
					return NOT_FOUND;
				}

				strcpy(new_file_path, file_name);
				strcat(new_file_path, ENTRY_FILE_EXTENSION);
				ent_file = fopen(new_file_path, "w");
				if (ent_file == NULL) {
					perror("Failed to open file ps.ent");
					exit(EXIT_FAILURE);
				}
				ent_has_values = 1;
			}
			fprintf(ent_file, "%s\t%d\n", ref_symbol->name, ref_symbol->location);
		}
	}

	if (ext_file != NULL) {
		fclose(ext_file);
	}
	if (ent_file != NULL) {
		fclose(ent_file);
	}
}

