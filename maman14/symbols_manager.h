#ifndef SYMBOLS_MANAGER_H
#define SYMBOLS_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "actions.h"
#include "strings_manager.h"
#include "constants.h"
#include "error_manager.h"
#include "macro_manager.h"

typedef struct {
	char* symbol_name;
	int symbol_location;
	int is_data;
} Symbols;


/* New structure for reference symbols*/
typedef struct {
	char* name;
	int location;
	int type; /* true for ext, false for ent*/
} ReferenceSymbol;


struct SymbolsManager {
	Symbols* array;
	int has_symbols_errors;
	int used;
	int size;
	char** ext;
	int ext_used;
	int ext_size;
	char** ent;
	int ent_used;
	int ent_size;
	ReferenceSymbol* ref_symbols;
	int ref_used;
	int ref_size;
};

typedef struct SymbolsManager SymbolsManager;





/* Function to add a reference symbol to the manager*/
void addReferenceSymbol(SymbolsManager* manager, const char* name, int location, int type);

/* Function to print all reference symbols*/
void printReferenceSymbols(const SymbolsManager* manager);


/* Function to create a SymbolsManager instance*/
SymbolsManager* createSymbolsManager(void);

/* Function to add a symbol to the manager*/
void addSymbol(MacroManager* macroManager, SymbolsManager* manager, const char* symbol_name, int symbol_location, int is_data);

/* Function to print all symbols*/
void printSymbols(const SymbolsManager* manager);

/* Function to get the location of a symbol by its name*/
int getSymbolLocation(const SymbolsManager* manager, const char* symbol_name);
int isRefExtSymbolExists(const SymbolsManager* manager, const char* symbol_name);
int isRefEntSymbolExists(const SymbolsManager* manager, const char* symbol_name);

/* Function to destroy a SymbolsManager instance and free resources*/
void destroySymbolsManager(SymbolsManager* manager);

/* Function to add a value to ext or ent*/
void addExtEnt(SymbolsManager* manager, const char* value, int is_ext);

/* Function to update the symbols table*/
void updateSymbolsTable(MacroManager* macroManager, SymbolsManager* manager, char** line, int location);

/* Function to check if an action exists*/
void printExt(const SymbolsManager* manager);
void printEnt(const SymbolsManager* manager);

int isSymbolPattern(const char* action_name);


int isDataPattern(const char* word);

int isReferencePattern(const char* word);

void updateDataSymbolsLocation(const SymbolsManager* manager, int steps);

int is_symbol_exists(const SymbolsManager* manager, const char* symbol_name);

#endif /* SYMBOLS_MANAGER_H*/

