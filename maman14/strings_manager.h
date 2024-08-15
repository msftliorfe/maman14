#ifndef STRINGS_H
#define STRINGS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "constants.h"
#include "error_manager.h"

#define ARRAY_INITIAL_SIZE 10

/* Function prototype*/
int is_null_or_empty(const char* str);

/* Function to split a string by spaces and return an array of strings*/
char** split_string(const char* str);

/* Function to free the allocated memory for the array of strings*/
void free_split_string(char** split_str);
char* letter_to_15bit_ascii(char letter);
char* strtrimlast(const char* src);
char* remove_first_last(const char* str);

char* clone_string(const char* input);
char* duplicate_string(const char* str);
int is_first_char_a_letter(const char* str);
#endif /* STRINGS_H*/
