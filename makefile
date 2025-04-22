# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g -std=c++17 -I./src

# Target executables
MAIN_TARGET = xmlparser_main
TEST_PARSER_TARGET = xmlparser_tests_parser
TEST_UTILS_TARGET = xmlparser_tests_utils
TEST_XMLPARSER_TARGET = xmlparser_tests_xmlParser

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Source files
MAIN_SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/xmlParser.cpp $(SRC_DIR)/xmlUtils.cpp
TEST_PARSER_SRCS = $(TEST_DIR)/test_parser.cpp $(SRC_DIR)/xmlParser.cpp $(SRC_DIR)/xmlUtils.cpp
TEST_UTILS_SRCS = $(TEST_DIR)/test_xmlUtils.cpp $(SRC_DIR)/xmlParser.cpp $(SRC_DIR)/xmlUtils.cpp
TEST_XMLPARSER_SRCS = $(TEST_DIR)/test_xmlParser.cpp $(SRC_DIR)/xmlParser.cpp $(SRC_DIR)/xmlUtils.cpp

# Object files
MAIN_OBJS = $(MAIN_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_PARSER_OBJS = $(TEST_PARSER_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_PARSER_OBJS := $(TEST_PARSER_OBJS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_UTILS_OBJS = $(TEST_UTILS_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_UTILS_OBJS := $(TEST_UTILS_OBJS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_XMLPARSER_OBJS = $(TEST_XMLPARSER_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_XMLPARSER_OBJS := $(TEST_XMLPARSER_OBJS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default rule: build all
all: $(MAIN_TARGET) $(TEST_PARSER_TARGET) $(TEST_UTILS_TARGET) $(TEST_XMLPARSER_TARGET)

# Build main app
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJS)

# Build test runners
$(TEST_PARSER_TARGET): $(TEST_PARSER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_PARSER_OBJS)

$(TEST_UTILS_TARGET): $(TEST_UTILS_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_UTILS_OBJS)

$(TEST_XMLPARSER_TARGET): $(TEST_XMLPARSER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_XMLPARSER_OBJS)

# Ensure build/ exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile rules for source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run main app
main: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Run all tests
test: $(TEST_PARSER_TARGET) $(TEST_UTILS_TARGET) $(TEST_XMLPARSER_TARGET)
	./$(TEST_PARSER_TARGET)
	./$(TEST_UTILS_TARGET)
	./$(TEST_XMLPARSER_TARGET)

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(MAIN_TARGET) $(TEST_PARSER_TARGET) $(TEST_UTILS_TARGET) $(TEST_XMLPARSER_TARGET)

.PHONY: all clean main test
