#include "error_manager.h"

void log_error(const char* function, int line, const char* file, const char* message) {
   fprintf(stderr, "Error in function %s at line %d in file %s: %s\n", function, line, file, message);
}

void file_error(const char* function, int line, const char* file, const char* message, const char* file_path) {
   fprintf(stderr, "Error in function %s at line %d in file %s: %s: %s\n", function, line, file, message, file_path);
}
void label_error(const char* function, int line, const char* file, const char* message, const char* label_name) {
	fprintf(stderr, "Error in function %s at line %d in file %s: %s: %s\n", function, line, file, message, label_name);
}



