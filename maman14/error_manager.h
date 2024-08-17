#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "constants.h"

void log_error(const char* function, int line, const char* file, const char* message);
void file_error(const char* function, int line, const char* file, const char* message, const char* file_path);
void label_error(const char* function, int line, const char* file, const char* message, const char* label_name);
#endif /* ERROR_MANAGER_H*/
