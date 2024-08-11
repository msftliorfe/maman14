# Define the compiler and the flags
CC = gcc
CFLAGS = -g -ansi -pedantic -Wall

# List of source files
SRC = assembler.c actions.c assembler_manager.c data_manager.c direct_builder.c \
      file_manager.c first_line_builder.c immediate_builder.c macro_manager.c \
      number_manager.c operands.c register_builder.c strings_manager.c \
      symbols_manager.c error_manager.c

# List of header files
HEADERS = actions.h assembler_manager.h data_manager.h direct_builder.h \
          file_manager.h first_line_builder.h immediate_builder.h \
          macro_manager.h number_manager.h operands.h register_builder.h \
          strings_manager.h symbols_manager.h error_manager.h

# Output executable
TARGET = assembler

# Default target
all: $(TARGET)

# Compile the program
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean up object files and backup files
clean:
	rm -f $(TARGET) *~

