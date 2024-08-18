#ifndef ASSEMBLER_MANAGER_H
#define ASSEMBLER_MANAGER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

#include "symbols_manager.h"
#include "file_manager.h"
#include "actions.h"
#include "data_manager.h"
#include "first_line_builder.h"
#include "strings_manager.h"
#include "operands.h"
#include "immediate_builder.h"
#include "register_builder.h"
#include "number_manager.h"
#include "direct_builder.h"
#include "constants.h"
#include "error_manager.h"
#include "macro_manager.h"

typedef struct {
	int location;
	char* metadata;
	char value[16];
	char* octal;
} Item;


typedef struct {
	int has_assembler_errors;
	int IC;
	int DC;
	Item* dataItems;
	int dataItemCount;
	Item* actionItems;
	int actionItemCount;
} AssemblerManager;

AssemblerManager* createAssemblerManager();
void destroyAssemblerManager(AssemblerManager* manager);
void first_scan(MacroManager* macroManager, FileManager* fileManager, AssemblerManager* assemblerManager, SymbolsManager* symbolsManager, Action* actions, Registers* registers, Registers_2* registers_2);
void processActionLine(Action* actions, char** line, AssemblerManager* assemblerManager, Registers* registers, Registers_2* registers_2);
void processDataLine(char** line, AssemblerManager* assemblerManager);
void addDataItem(AssemblerManager* manager, int location, const char* value);
void addActionItem(AssemblerManager* manager, char* metadata, int location, const char* value);
void printDataItems(const AssemblerManager* manager);
void printActionItems(const AssemblerManager* manager);
void updateLocationDataSymbols(const SymbolsManager* symbolsManager, const AssemblerManager* manager);
void updateDataItemsLocation(const AssemblerManager* manager);
void second_scan(AssemblerManager* assemblerManager, SymbolsManager* symbolsManager);
void printObjToFile(char* file_name, const AssemblerManager* assemblerManager);
void printReferenceSymbolsToFile(char* file_name, const SymbolsManager* manager);

#endif /*ASSEMBLER_MANAGER_H*/

