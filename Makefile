# Compiler
CC := gcc

# Directories
SRC_DIR := src
BUILD_DIR := build

# Source files
SRC := ./src/main.c ./src/**/*.c ./src/algorithms/heuristics/**/*.c ./src/algorithms/metaheuristics/**/*.c

# Object files
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# Compiler flags
CFLAGS := -Wall -Wextra -Wno-unknown-pragmas -O3 -fopenmp #  -lubsan -fsanitize=undefined -fsanitize=address -Wno-unused-parameter

# Build target
TARGET := $(BUILD_DIR)/out2

# Default target
all: $(TARGET)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -g

# Link object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -lm -o $@

# Clean build directory
clean:
	rm $(BUILD_DIR)/out2

.PHONY: all clean
