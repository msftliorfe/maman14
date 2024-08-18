#include "operands.h"

/**
 * initialize_operands -
 * Initializes the register names for two sets of registers.
 *
 * @param registers Pointer to an array of `Registers` structures to be initialized.
 * @param registers_2 Pointer to an array of `Registers_2` structures to be initialized.
 */
void initialize_operands(Registers* registers, Registers_2* registers_2) {
	registers[0].register_name = "r0";
	registers[1].register_name = "r1";
	registers[2].register_name = "r2";
	registers[3].register_name = "r3";
	registers[4].register_name = "r4";
	registers[5].register_name = "r5";
	registers[6].register_name = "r6";
	registers[7].register_name = "r7";
	registers_2[0].register_name = "*r0";
	registers_2[1].register_name = "*r1";
	registers_2[2].register_name = "*r2";
	registers_2[3].register_name = "*r3";
	registers_2[4].register_name = "*r4";
	registers_2[5].register_name = "*r5";
	registers_2[6].register_name = "*r6";
	registers_2[7].register_name = "*r7";
}

/**
 * is_valid_register -
 * Checks if the given operand is a valid register.
 *
 * @param operand The register name to check.
 * @return FOUND (1) if the operand is a valid register, NOT_FOUND (0) otherwise.
 */
int is_valid_register(Registers* registers, const char* operand) {
	int i;
	for (i = 0; i < NUM_OF_REGISTERS; ++i) {
		if (strcmp(registers[i].register_name, operand) == 0) {
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
int is_valid_register_2(Registers_2* registers_2,const char* operand) {
	int i;
	for (i = 0; i < NUM_OF_REGISTERS; ++i) {
		if (strcmp(registers_2[i].register_name, operand) == 0) {
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
AddressingType get_addressing_type(Registers* registers, Registers_2* registers_2,const char* operand) {
	if (is_valid_register(registers,operand)) {
		return DirectRegister;
	}
	else if (is_valid_register_2(registers_2,operand)) {
		return IndirectRegister;
	}
	else if (operand[0] == '#') {
		return Immediate;
	}
	return Direct;
}


