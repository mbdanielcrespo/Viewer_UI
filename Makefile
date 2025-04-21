# Makefile for Hoist Management System

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -I./include -I/usr/local/include
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

# Raylib flags and libraries
RAYLIB_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Build directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Find all source files recursively
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
# Create corresponding object file paths
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Target executable
TARGET = $(BIN_DIR)/hoist_management_system

# Phony targets
.PHONY: all clean run re val

# Default target
all: prepare $(TARGET)

# Prepare directories
prepare:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)/ui

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(RAYLIB_LIBS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the application
run: all
	$(TARGET)

# Rebuild target
re: clean all

# Run with Valgrind
val: all
	valgrind $(VALGRIND_FLAGS) $(TARGET)
