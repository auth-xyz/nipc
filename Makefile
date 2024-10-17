
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
LDFLAGS = 


SRC_DIR = src
HEADER_DIR = include
BUILD_DIR = comp
DIST_DIR = dist


SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))


TARGET = $(DIST_DIR)/my_program


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)


all: $(TARGET)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(HEADER_DIR) -c $< -o $@

$(TARGET): $(OBJS) | $(DIST_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)

rebuild: clean all

.PHONY: all clean rebuild

