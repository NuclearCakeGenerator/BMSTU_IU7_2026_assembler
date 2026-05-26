CXX := aarch64-linux-gnu-g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

TARGET := array_add.exe
SRC_DIR := src
INCLUDES := -I$(SRC_DIR)

all: $(TARGET)

$(TARGET): $(SRC_DIR)/main.cpp $(SRC_DIR)/types/index.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean