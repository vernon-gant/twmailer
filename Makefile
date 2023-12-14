# Compiler and flags
CC = g++
CFLAGS = -g -Wall -Wextra -O -pthread -std=c++17

# Include directories
INCLUDES = -I./client -I./server -I/usr/include

# Source files
SOURCES_CLIENT = $(shell find client/ -name '*.cpp')
SOURCES_SERVER = $(shell find server/ -name '*.cpp')

# Object files directory
OBJ_DIR = bin/obj

# Object files
OBJS_CLIENT = $(SOURCES_CLIENT:%.cpp=$(OBJ_DIR)/%.o)
OBJS_SERVER = $(SOURCES_SERVER:%.cpp=$(OBJ_DIR)/%.o)

# Output directory for binaries
BIN_DIR = bin

# Output binaries
BIN_CLIENT = $(BIN_DIR)/client
BIN_SERVER = $(BIN_DIR)/server

# LDAP Libraries
LDAP_LIBS = -lldap -llber

# Build targets
.PHONY: all prebuild clean

all: prebuild $(BIN_CLIENT) $(BIN_SERVER)

prebuild:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

$(BIN_CLIENT): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $(BIN_CLIENT) $(OBJS_CLIENT) $(LDAP_LIBS)

$(BIN_SERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $(BIN_SERVER) $(OBJS_SERVER) $(LDAP_LIBS)

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_CLIENT) $(BIN_SERVER)
