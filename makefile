# Compiler
CXX := g++
CXXFLAGS := -std=c++20 -Iinclude
LDFLAGS := -lX11 -lGL -lpthread -lpng -lstdc++fs

# Directories
SRC_DIR := src
BUILD_DIR := build
DEB_PKG_DIR := Deb_pkg/Retro_X85_Emu
BIN_DIR := $(DEB_PKG_DIR)/Emu_x85/bin

# Find all .cpp files recursively
SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Target executable
TARGET := $(BUILD_DIR)/emu

default: release
all: $(TARGET)

# Release build
release: CXXFLAGS += -O2
release: $(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG -O0
debug: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create build directory if not exists
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Debian Package Build
debian: $(TARGET)
	@mkdir -p $(BIN_DIR)
	mv $(TARGET) $(BIN_DIR)/
	cd Deb_pkg && dpkg-deb --build Retro_X85_Emu

# Clean build
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean debug release debian
