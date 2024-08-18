#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "actions.h"
#include "file_manager.h"
#include "symbols_manager.h"
#include "number_manager.h"
#include "macro_manager.h"
#include "assembler_manager.h"
#include "constants.h"
#include "error_manager.h"
#include "operands.h"


/*
This program is an assembler for the assembly language.
The program receives an input file containing a program written in assembly language. The role of the assembler is to build from this a file containing machine code.
@param int argc
@param char** argv
@return int 0 if OK 1 otherwise
*/
int main(int argc, char** argv) {

	FileManager fileManager;
	MacroManager macroManager;
	Action actions[NUM_OF_ACTIONS];
	Registers* registers = (Registers*)malloc(NUM_OF_REGISTERS * sizeof(Registers));
	Registers_2* registers_2 = (Registers_2*)malloc(NUM_OF_REGISTERS * sizeof(Registers_2));



	/*There isn't any file name*/
	if (argc == 1)
	{
		log_error("main", 36, "assembler.c", "There isn't any file name as input");
		return !OK;
	}


	intialize_actions_array(actions);
	initialize_operands(registers, registers_2);
	/*There is at least 1 file name. Start reading*/
	while (--argc > 0)
	{
		/*Initialize a FileManager*/
		initialize_file_manager(&fileManager);

		/*Initialize a MacroManager*/
		init_macro_manager(&macroManager);

		/*Check legality of file name*/
		/*Process files provided by the user*/
		if (input_process(&fileManager, &macroManager, *++argv))
		{

			/*Only if reading the file and creating the post-macro file worked, then continue*/

			/*print_post_macro(&fileManager);*//*Use only for work, asked only to print to file*/

			if (printPostMacroToFile(*argv, &fileManager)) {

				/*Create assemblerManager*/
				AssemblerManager* assemblerManager = createAssemblerManager();
				if (assemblerManager != NULL)
				{
					/*Create symbolsManager*/
					SymbolsManager* symbolsManager = createSymbolsManager();
					if (symbolsManager != NULL)
					{
						first_scan(&macroManager, &fileManager, assemblerManager, symbolsManager, actions, registers, registers_2);
						updateLocationDataSymbols(symbolsManager, assemblerManager);
						updateDataItemsLocation(assemblerManager);


						second_scan(assemblerManager, symbolsManager);
						if (assemblerManager->has_assembler_errors == NOT_FOUND && symbolsManager->has_symbols_errors == NOT_FOUND)
						{
							/*Can print output files*/
							printObjToFile(*argv, assemblerManager);
							printReferenceSymbolsToFile(*argv, symbolsManager);
						}
					}
				}
			}
		}
		free_file_manager(&fileManager);
	}
	return OK;

}


