# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I./src
LDFLAGS = -lncurses  # Add this line to link ncurses

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

# Ensure obj directory exists before compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  # Create obj directory if it does not exist
	$(CC) $(CFLAGS) -c $< -o $@

# Linking the executable
$(EXEC): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(EXEC) $(LDFLAGS)

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)  # Recreate the obj directory after cleaning

.PHONY: all clean

