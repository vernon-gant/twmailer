# Compiler
CC=g++

# Compiler Flags
CFLAGS=-g -Wall -Wextra -O -std=c++17 -pthread

# Source, Object, and Binary directories
SRC_DIR_CLIENT=client
SRC_DIR_SERVER=server
OBJ_DIR_CLIENT=$(OBJ_DIR)/client
OBJ_DIR_SERVER=$(OBJ_DIR)/server
OBJ_DIR=obj
BIN_DIR=bin

# Find source files recursively
SRCS_CLIENT=$(shell find $(SRC_DIR_CLIENT) -name '*.cpp')
SRCS_SERVER=$(shell find $(SRC_DIR_SERVER) -name '*.cpp')

# Create object file paths, preserving the directory structure
OBJS_CLIENT=$(SRCS_CLIENT:$(SRC_DIR_CLIENT)/%.cpp=$(OBJ_DIR_CLIENT)/%.o)
OBJS_SERVER=$(SRCS_SERVER:$(SRC_DIR_SERVER)/%.cpp=$(OBJ_DIR_SERVER)/%.o)

# Targets
.PHONY: all clean

all: $(BIN_DIR)/server $(BIN_DIR)/client

# Server binary
$(BIN_DIR)/server: $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $@ $^

# Client binary
$(BIN_DIR)/client: $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule for object files - server
$(OBJ_DIR_SERVER)/%.o: $(SRC_DIR_SERVER)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for object files - client
$(OBJ_DIR_CLIENT)/%.o: $(SRC_DIR_CLIENT)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	clear
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*
