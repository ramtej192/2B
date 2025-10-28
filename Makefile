# Compiler
CC = gcc

# Compiler flags
# -Wall -Wextra: Enable all warnings
# -std=c99: Use the C99 standard (for long long and declarations)
# -g: Include debug symbols
# -O2: Optimization level
CFLAGS = -Wall -Wextra -std=c99 -g -O2

# Source file
SRCS = main.c

# Executable name
EXEC = program

# Default target
all: $(EXEC)

# Link the program
$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRCS)

# Run the program
run: $(EXEC)
	./$(EXEC)

# Clean up build files
clean:
	@if exist program.exe del /f program.exe

# Phony targets (targets that don't represent files)
.PHONY: all run clean