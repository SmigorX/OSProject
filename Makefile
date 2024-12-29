# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I./src

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = build

# Source files and object files
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Executable name
EXEC = $(BIN_DIR)/dining_philosophers

# Default target
all: $(EXEC)

# Linking the executable
$(EXEC): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(EXEC)

# Compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	find $(OBJ_DIR) -type f -exec rm -f {} \;
	find $(BIN_DIR) -type f -exec rm -f {} \;

.PHONY: all clean

