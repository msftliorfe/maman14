#ifndef OPERANDS_H
#define OPERANDS_H

#include <string.h>

#include "constants.h"
#include "error_manager.h"

int is_valid_register(const char* operand);

typedef enum AddressingType {
	Direct,
	Immediate,
	DirectRegister,
	IndirectRegister
} AddressingType;

AddressingType get_addressing_type(const char* operand);


#endif /*OPERANDS_H*/
