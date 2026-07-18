CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC
INCLUDES = -Iinclude

BUILD_DIR = build
LIB_NAME = liblogger.so
LIB_SRC = src/Logger.cpp
LIB_OBJ = $(BUILD_DIR)/Logger.o
LIB_DEPS = include/logger/Logger.hpp include/logger/LogLevel.hpp

.PHONY: all clean

all: $(LIB_NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_OBJ): $(LIB_SRC) $(LIB_DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(LIB_NAME): $(LIB_OBJ)
	$(CXX) -shared -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME)
