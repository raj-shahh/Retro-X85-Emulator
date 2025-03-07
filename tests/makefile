# Compiler
CXX := g++
CXXFLAGS := -std=c++17 -I../include -I./TestHeaders
LDFLAGS := -lgtest -lgtest_main -pthread

# Directories
SRC_DIR := ../src
TEST_DIR := .
BUILD_DIR := ../build_tests
BUILD_SRC_DIR := $(BUILD_DIR)/src
BUILD_TEST_DIR := $(BUILD_DIR)/tests

# Find all .cpp files in src/ and tests/, while preserving paths
SRC_SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp" ! -name "main.cpp")
TEST_SRCS := $(shell find $(TEST_DIR) -type f -name "*.cpp")

# Generate object file paths, maintaining directory structure
SRC_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_SRC_DIR)/%.o, $(SRC_SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_TEST_DIR)/%.o, $(TEST_SRCS))

# Target executable
TEST_TARGET := $(BUILD_DIR)/runTests

# Default target: Build and run tests
test: $(TEST_TARGET)
	@echo "Running Tests..."
	$(TEST_TARGET)

# Link test executable
$(TEST_TARGET): $(TEST_OBJS) $(SRC_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files (preserving subdirectory structure)
$(BUILD_SRC_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compile test files (preserving subdirectory structure)
$(BUILD_TEST_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Ensure build directories exist
$(BUILD_DIR) $(BUILD_SRC_DIR) $(BUILD_TEST_DIR):
	@mkdir -p $@

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean test
