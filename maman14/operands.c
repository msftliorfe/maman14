#include "operands.h"

/* Array of valid operands*/
static const char* valid_registers[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
static const char* valid_registers_2[] = { "*r0", "*r1", "*r2", "*r3", "*r4", "*r5", "*r6", "*r7" };
static const int num_registers = sizeof(valid_registers) / sizeof(valid_registers[0]);

/**
 * is_valid_register -
 * Checks if the given operand is a valid register.
 *
 * @param operand The register name to check.
 * @return FOUND (1) if the operand is a valid register, NOT_FOUND (0) otherwise.
 */
int is_valid_register(const char* operand) {
	int i;
	for (i = 0; i < num_registers; ++i) {
		if (strcmp(valid_registers[i], operand) == 0) {
			return FOUND;
		}
	}
	return NOT_FOUND;
}

/**
 * is_valid_register_2 - 
 * Checks if the given operand is a valid register in the valid_registers_2 list.
 *
 * @param operand The register name to check.
 * @return FOUND (1) if the operand is a valid register, NOT_FOUND (0) otherwise.
 */
int is_valid_register_2(const char* operand) {
	int i;
	for (i = 0; i < num_registers; ++i) {
		if (strcmp(valid_registers_2[i], operand) == 0) {
			return FOUND;
		}
	}
	return NOT_FOUND;
}

/**
 * get_addressing_type -
 * Determines the addressing type based on the operand.
 *
 * @param operand The operand whose addressing type is to be determined.
 * @return The AddressingType enum value corresponding to the operand's addressing type.
 */
AddressingType get_addressing_type(const char* operand) {
	if (is_valid_register(operand)) {
		return DirectRegister;
	}
	else if (is_valid_register_2(operand)) {
		return IndirectRegister;
	}
	else if (operand[0] == '#') {
		return Immediate;
	}
	return Direct;
}


