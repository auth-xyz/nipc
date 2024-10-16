# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
LDFLAGS = 

# Directories
SRC_DIR = src
HEADER_DIR = include
BUILD_DIR = comp
DIST_DIR = dist

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Executable name
TARGET = $(DIST_DIR)/my_program

# Create the build and dist directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

# Default target: build the project
all: $(TARGET)

# Rule to compile .cpp files into .o object files in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule to link object files and generate the final executable in the dist directory
$(TARGET): $(OBJS) | $(DIST_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Clean build directory and output
clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)

# Rebuild the project
rebuild: clean all

.PHONY: all clean rebuild

