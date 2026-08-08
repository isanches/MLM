# Project Structure
# MLM/
# ├── bin/          <-- Created automatically upon typing 'make'
# │   └── mlm.exe
# ├── source/
# │   └── main.c
# └── Makefile      <-- This file here
#   
# Compiler and flags
CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -pedantic

# Directories
SRC_DIR := source
INC_DIR := include
BIN_DIR := bin

# Target executable name
TARGET  := $(BIN_DIR)/mlm.exe

# Default rule
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(SRC_DIR)/main.c $(INC_DIR)/mlm.h
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Clean rule to remove the generated binary folder
clean:
	rm -rf $(BIN_DIR) *~

.PHONY: all clean
