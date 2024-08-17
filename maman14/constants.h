#ifndef CONSTANTS_H
#define CONSTANTS_H

#define OK 0
#define LOG_ERROR(message) log_error(__func__, __LINE__, __FILE__, message)
//#define file_error(message, file_path) file_error(__func__, __LINE__, __FILE__, message, file_path)
//#define label_error(message, file_path) label_error(__func__, __LINE__, __FILE__, message, file_path)
#define FOUND 1 /*Instead of bool true*/
#define NOT_FOUND 0 /*Instead of bool false*/
#define INPUT_FILE_EXTENSION ".as"
#define POST_MACRO_FILE_EXTENSION ".am"
#define OBJECTS_FILE_EXTENSION ".ob"
#define EXTERNALS_FILE_EXTENSION ".ext"
#define ENTRY_FILE_EXTENSION ".ent"
#define WORD_SIZE_IN_BITS 15
#define NUM_OF_ACTIONS 16
#define FIRST_MEMORY_PLACE 100
#define MAX_SYMBOL_NAME_LENGTH 31


#endif /*CONSTANTS_H*/
