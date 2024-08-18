#ifndef OPERANDS_H
#define OPERANDS_H

#include <string.h>

#include "constants.h"
#include "error_manager.h"


typedef enum AddressingType {
	Direct,
	Immediate,
	DirectRegister,
	IndirectRegister
} AddressingType;

typedef struct {
	char* register_name;
} Registers;

typedef struct {
	char* register_name;
} Registers_2;

AddressingType get_addressing_type(Registers* registers, Registers_2* registers_2, const char* operand);

void initialize_operands(Registers* registers, Registers_2* registers_2);
int is_valid_register(Registers* registers, const char* operand);
int is_valid_register_2(Registers_2* registers_2, const char* operand);
#endif /*OPERANDS_H*/
