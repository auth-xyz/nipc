CXX = g++
CXXFLAGS = -std=c++20 -Wall -lcurl

ROOT_DIR = .
LIBS_DIR = $(ROOT_DIR)/libs
SRC_DIR = $(ROOT_DIR)/src
HEADERS = ./headers
BUILD = dist/compiled
DIST = dist/build

EXEC = $(DIST)/nip

LIBS := $(wildcard $(LIBS_DIR)/*.cpp)
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(LIBS_DIR)/%.cpp, $(BUILD)/%.o, $(LIBS)) $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/%.o, $(SRCS))

# Targets
all: create_dirs $(EXEC)

create_dirs:
	mkdir -p $(BUILD) && mkdir -p $(DIST)

clean:
	rm -rf dist/

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

$(BUILD)/%.o: $(LIBS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c $< -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c $< -o $@

# Check and distcheck targets
check:
	$(CXX) $(CXXFLAGS) -I$(HEADERS) $(SRCS) -o /dev/null
	@echo "Check complete: No errors found."

dist: create_dirs
	tar -czvf $(DIST)/project.tar.gz $(SRC_DIR) $(HEADERS) Makefile

distcheck: dist
	mkdir -p $(DIST)/temp
	tar -xzvf $(DIST)/project.tar.gz -C $(DIST)/temp
	cd $(DIST)/temp && $(MAKE)
	cd $(DIST)/temp && $(MAKE) check
	rm -rf $(DIST)/temp

