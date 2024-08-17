#include "file_manager.h"

/**
 * initialize_file_manager -
 * Initializes the FileManager structure.
 *
 * @param manager Pointer to the FileManager structure to be initialized.
 */
void initialize_file_manager(FileManager* manager) {
	/* Set the post_macro pointer to NULL, indicating no macro has been processed yet. */
	manager->post_macro = NULL;
	/* Initialize the row count to 0, meaning no rows have been processed yet. */
	manager->row_count = 0;

}

/**
 * free_file_manager -
 * Frees the memory allocated for the FileManager structure.
 *
 * @param manager Pointer to the FileManager structure whose memory is to be freed.
 */
void free_file_manager(FileManager* manager) {
	int i, j;
	/* Iterate over each row in the post_macro array. */
	for (i = 0; i < manager->row_count; ++i) {
		/* Iterate over each string in the current row and free it. */
		for (j = 0; manager->post_macro[i][j] != NULL; ++j) {
			free(manager->post_macro[i][j]);
		}
		/* Free the memory allocated for the current row. */
		free(manager->post_macro[i]);
	}
	/* Free the memory allocated for the post_macro array itself. */
	free(manager->post_macro);
}

/**
 *input_process -
 *Processes the input file and updates the FileManager with the processed lines.
 *
 *@param fileManager A pointer to the FileManager structure that will be updated with
 *                     the processed data from the input file.
 * @param macroManager A pointer to the MacroManager structure used to check for macros
 *                      and retrieve their content.
 * @param file_path The path to the file to be processed. This is a string representing
 *                  the file's location in the filesystem.
 * @return int 0 if the macro file was created successfully otherwise 1
 */
int input_process(FileManager* fileManager, MacroManager* macroManager, char* file_path) {
	int i, split_count, len;
	char** split_line;
	char line[MAX_LINE_LENGTH];
	char* new_file_path;

	/*Concatenate extension string to the name of the file*/
	len = strlen(file_path) + strlen(INPUT_FILE_EXTENSION) + 1;
	new_file_path = malloc(len);

	if (new_file_path == NULL) {
		log_error("input_process", 61, "file_manager.c", "Memory allocation failed");
		return NOT_FOUND;
	}

	strcpy(new_file_path, file_path);
	strcat(new_file_path, INPUT_FILE_EXTENSION);

	/* Open the specified file for reading*/
	FILE* file = fopen(new_file_path, "r");
	if (!file) {
		/*Failed to open file*/
		file_error("input_process", 72, "file_manager.c", "Failed to open file", new_file_path);
		return NOT_FOUND;
	}

	/*File opened*/
	while (fgets(line, sizeof(line), file)) {
		/* Remove newline character from the end of the line*/
		line[strcspn(line, "\n")] = '\0';

		/* Split the line into tokens using whitespace as delimiters */
		split_line = split_string(line);

		/* Count the number of tokens in the split line */
		split_count = 0;
		while (split_line[split_count] != NULL) split_count++;

		/* Check if the first token is a macro name */
		if (is_macro_name(macroManager, *split_line))
		{
			/* Retrieve the content of the macro associated with the macro name */
			char*** processed_lines = get_macro_content(macroManager, *split_line);

			/* Iterate over each row of the processed macro content */
			for (i = 0; processed_lines[i] != NULL; i++) {
				char** row = processed_lines[i];  /* Each row is an array of strings (char**) */

				/* Reallocate memory for the post_macro array to include the new row */
				fileManager->post_macro = realloc(fileManager->post_macro, (fileManager->row_count + 1) * sizeof(char**));
				if (fileManager->post_macro == NULL) {
					/* Handle memory allocation failure */
					log_error("input_process", 102, "file_manager.c", "Memory allocation failed");
					return NOT_FOUND;
				}
				/* Add the new row to the post_macro array */
				fileManager->post_macro[fileManager->row_count] = row;
				/* Increment the row count */
				fileManager->row_count++;
			}

		}
		else {
			/* Process the line normally if it is not a macro name */
			char** processed_line = process_file_line(macroManager, split_line, split_count);
			if (processed_line != NULL) {

				/* Reallocate memory for the post_macro array to include the new line */
				fileManager->post_macro = realloc(fileManager->post_macro, (fileManager->row_count + 1) * sizeof(char**));
				if (fileManager->post_macro == NULL) {
					/* Handle memory allocation failure by logging an error message and exit */
					log_error("input_process", 121, "file_manager.c", "Memory allocation failed");
					return NOT_FOUND;
				}

				/* Add the new line to the post_macro array */
				fileManager->post_macro[fileManager->row_count] = processed_line;
				/* Increment the row count */
				fileManager->row_count++;
			}

		}


		/* Free the memory allocated for the split line tokens */
		for (i = 0; split_line[i] != NULL; ++i) {
			free(split_line[i]);
		}
		free(split_line);
	}
	/* Close the file after processing */
	fclose(file);
	return FOUND;
}

/**
 *print_post_macro -
 *Prints the contents of the `post_macro` table from a FileManager structure.
 *@param manager A pointer to a FileManager structure containing the data to be printed.
 */
void print_post_macro(FileManager* manager) {
	int i, j, col, max_columns;
	printf("post_macro\n");
	if (manager->row_count == 0) {
		printf("No data to display.\n");
		return;
	}

	/* Determine the maximum number of columns for proper formatting*/
	/* depende on input file*/
	max_columns = 0;
	for (i = 0; i < manager->row_count; ++i) {
		j = 0;
		while (manager->post_macro[i][j] != NULL) j++;
		if (j > max_columns) {
			max_columns = j;
		}
	}

	/* Print the table*/
	printf("+");
	for (col = 0; col < max_columns; ++col) {
		printf("------------+"); /* Adjust the width of columns as needed*/
	}
	printf("\n");

	for (i = 0; i < manager->row_count; ++i) {
		printf("|");
		for (j = 0; manager->post_macro[i][j] != NULL; ++j) {
			printf(" %-10s |", manager->post_macro[i][j]); /* Adjust the width of columns as needed*/
		}

		/* Fill the remaining columns with empty spaces if the current row has fewer columns*/
		for (j = 0; j < max_columns; ++j) {
			if (manager->post_macro[i][j] == NULL) {
				printf(" %-10s |", "");
			}
		}
		printf("\n");

		/* Print the row separator*/
		printf("+");
		for (col = 0; col < max_columns; ++col) {
			printf("------------+"); /* Adjust the width of columns as needed*/
		}
		printf("\n");
	}
}

/**
 * printPostMacroToFile -
 * Writes the content of the `post_macro` table from a FileManager structure
 * to a specified file. The data is formatted into a table, with columns
 * adjusted to fit the content.
 * @param file_name The base name of the file where the data will be written
 * @param fileManager A pointer to a FileManager structure containing the data to be printed
 * @return int 0 if the content was written successfully in the macro file otherwise 1
 */
int printPostMacroToFile(char* file_name, const FileManager* fileManager) {
	int len;
	int i, j, col, max_columns;
	char* new_file_path;

	/*Concatenate extension string to the name of the file*/
	len = strlen(file_name) + strlen(POST_MACRO_FILE_EXTENSION) + 1;
	new_file_path = malloc(len);

	/*Failed to allocate memory*/
	if (new_file_path == NULL) {
		log_error("printPostMacroToFile", 219, "file_manager.c", "Memory allocation failed");
		return NOT_FOUND;
	}

	strcpy(new_file_path, file_name);
	strcat(new_file_path, POST_MACRO_FILE_EXTENSION);

	/*Open file to write*/
	FILE* file = fopen(new_file_path, "w");

	/*failed to open file*/
	if (file == NULL) {
		strcpy(new_file_path, file_name);
		file_error("printPostMacroToFile", 501, "file_manager.c", "Failed to open file", new_file_path);
		return NOT_FOUND;
	}

	/*file opened start writing*/
	fprintf(file, "post_macro\n");
	/*Nothing to write*/
	if (fileManager->row_count == 0) {
		fprintf(file, "No data to display.\n");
		return;
	}
	/*There is data to write*/
	/* Determine the maximum number of columns for proper formatting*/
	/* depende on input file*/
	max_columns = 0;
	for (i = 0; i < fileManager->row_count; ++i) {
		j = 0;
		while (fileManager->post_macro[i][j] != NULL) j++;
		if (j > max_columns) {
			max_columns = j;
		}
	}

	/* Print the table*/
	fprintf(file, "+");
	for (col = 0; col < max_columns; ++col) {
		fprintf(file, "------------+"); /* Adjust the width of columns as needed*/
	}
	fprintf(file, "\n");

	for (i = 0; i < fileManager->row_count; ++i) {
		fprintf(file, "|");
		for (j = 0; fileManager->post_macro[i][j] != NULL; ++j) {
			fprintf(file, " %-10s |", fileManager->post_macro[i][j]); /* Adjust the width of columns as needed*/
		}

		/* Fill the remaining columns with empty spaces if the current row has fewer columns*/
		for (j = 0; j < max_columns; ++j) {
			if (fileManager->post_macro[i][j] == NULL) {
				fprintf(file, " %-10s |", "");
			}
		}
		fprintf(file, "\n");

		/* Print the row separator*/
		fprintf(file, "+");
		for (col = 0; col < max_columns; ++col) {
			fprintf(file, "------------+"); /* Adjust the width of columns as needed*/
		}
		fprintf(file, "\n");
	}
	return FOUND;
}


