#ifndef MACRO_MANAGER_H
#define MACRO_MANAGER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

#include "constants.h"
#include "error_manager.h"


#define MAX_STRING_LENGTH 100
#define MAX_MACROS 100


typedef struct {
    char*** commands;  /* Matrix of strings (commands)*/
    int row_count;  /* Number of rows in the matrix*/
} Macro;

typedef struct {
    Macro macros[MAX_MACROS];
    char* macro_names[MAX_MACROS];
    int macro_count;
    int is_macro_context;
    char current_macro_name[MAX_STRING_LENGTH];
} MacroManager;

void init_macro_manager(MacroManager* manager);
char** process_file_line(MacroManager* manager, char** input, int input_count);
void free_macro_manager(MacroManager* manager);
char*** get_macro_content(MacroManager* manager, const char* macro_name);
int is_macro_name(MacroManager* manager, const char* name);

#endif /*MACRO_MANAGER_H*/ 

