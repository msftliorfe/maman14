#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "strings_manager.h"
#include "macro_manager.h"
#include "constants.h"
#include "error_manager.h"


#define MAX_LINE_LENGTH 1024
#define INPUT_DIR 

typedef struct {
	char*** post_macro;
	int row_count;
} FileManager;

void initialize_file_manager(FileManager* manager);
void free_file_manager(FileManager* manager);
int input_process(FileManager* fileManager, MacroManager* macroManager, char* file_path);
void print_post_macro(FileManager* manager);
int printPostMacroToFile(char* file_name, const FileManager* fileManager);

#endif /* FILE_MANAGER_H*/



