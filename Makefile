#Input/Output Filenames
INPUT_FILE = "config.txt"
OUTPUT_FILE = "output.txt"

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I$(INC_DIR)

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Source, object, and dependency files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) 
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Target executable
TARGET = main

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Object file rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Run the executable
run: $(TARGET)
	./$(TARGET) $(INPUT_FILE) $(OUTPUT_FILE)

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Include dependency files
-include $(DEPS)

.PHONY: all clean