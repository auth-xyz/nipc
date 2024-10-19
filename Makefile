CXX = g++
CXXFLAGS = -std=c++20 -Wall -lcurl

ROOT_DIR = .
LIBS_DIR = $(ROOT_DIR)/libs
SRC_DIR = $(ROOT_DIR)/src
HEADERS = ./headers
BUILD = dist/compiled
DIST = dist/build

EXEC = $(DIST)/nip

# Collect .cpp files
LIBS := $(wildcard $(LIBS_DIR)/*.cpp)
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(LIBS_DIR)/%.cpp, $(BUILD)/%.o, $(LIBS)) $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/%.o, $(SRCS))

# Targets
all: create_dirs $(EXEC)

create_dirs:
	mkdir -p $(BUILD) && mkdir -p $(DIST)

clean:
	rm -rf dist/

# Build the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# Compile source files into object files, with or without headers
$(BUILD)/%.o: $(LIBS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c $< -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c $< -o $@

