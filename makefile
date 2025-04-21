# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g -std=c++17 -I./src

# Target executables
MAIN_TARGET = xmlparser_main
TEST_TARGET = xmlparser_tests

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Source files
MAIN_SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/xmlParser.cpp $(SRC_DIR)/xmlUtils.cpp
TEST_SRCS = $(TEST_DIR)/test_parser.cpp $(SRC_DIR)/xmlParser.cpp $(SRC_DIR)/xmlUtils.cpp

# Object files (placed in build/)
MAIN_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(MAIN_SRCS)))
TEST_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(TEST_SRCS)))

# Default rule: build and run
all: $(MAIN_TARGET) $(TEST_TARGET) run

# Build main app
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJS)

# Build test runner
$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS)

# Ensure build/ exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run rule
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(MAIN_TARGET) $(TEST_TARGET)

.PHONY: all run clean
