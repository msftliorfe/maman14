#include "error_manager.h"

/**
 * log_error Logs an error message with detailed information about the location of the error.
 *
 * @param function The name of the function where the error occurred.
 * @param line The line number in the source code where the error occurred.
 * @param file The name of the source code file where the error occurred.
 * @param message The error message describing the nature of the error.
 */
void log_error(const char* function, int line, const char* file, const char* message) {
   fprintf(stderr, "Error in function %s at line %d in file %s: %s\n", function, line, file, message);
}

/**
 * file_error - 
 * Logs an error message related to file operations with detailed information.
 *
 * @param function The name of the function where the error occurred.
 * @param line The line number in the source code where the error occurred.
 * @param file The name of the source code file where the error occurred.
 * @param message The error message describing the nature of the file error.
 * @param file_path The path of the file involved in the error, providing additional context.
 */
void file_error(const char* function, int line, const char* file, const char* message, const char* file_path) {
   fprintf(stderr, "Error in function %s at line %d in file %s: %s: %s\n", function, line, file, message, file_path);
}

/**
 * label_error -
 * Logs an error message related to label handling with detailed information.
 *
 * @param function The name of the function where the error occurred.
 * @param line The line number in the source code where the error occurred.
 * @param file The name of the source code file where the error occurred.
 * @param message The error message describing the nature of the label error.
 * @param label_name The name of the label involved in the error, providing additional context.
 */
void label_error(const char* function, int line, const char* file, const char* message, const char* label_name) {
	fprintf(stderr, "Error in function %s at line %d in file %s: %s: %s\n", function, line, file, message, label_name);
}



