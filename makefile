# Compiler
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude
LDFLAGS := -lX11 -lGL -lpthread -lpng -lstdc++fs

# Directories
SRC_DIR := src
BUILD_DIR := build

# Find all .cpp files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Target executable
TARGET := $(BUILD_DIR)/emu

default: release

# Release build
release: CXXFLAGS += -O2
release: $(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create build directory if not exists
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean build
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean debug release
