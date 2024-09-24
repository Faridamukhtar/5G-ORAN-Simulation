# Output Filename
OUTPUT_FILE = "output.txt"
TRANSMISSION_MODE = "ORAN"

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I$(INC_DIR)
LDFLAGS = -lz  # Link with the zlib library

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
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# Object file rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Run the executable
run: $(TARGET)
	./$(TARGET) $(OUTPUT_FILE) $(TRANSMISSION_MODE)

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Include dependency files
-include $(DEPS)

.PHONY: all clean run