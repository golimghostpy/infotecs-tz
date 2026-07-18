CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC
INCLUDES = -Iinclude

BUILD_DIR = build
LIB_NAME = liblogger.so
LIB_SRC = src/Logger.cpp
LIB_OBJ = $(BUILD_DIR)/Logger.o
LIB_DEPS = include/logger/Logger.hpp include/logger/LogLevel.hpp

APP_NAME = logger_app
APP_SRC = src/app.cpp
APP_DEPS = include/logger/Logger.hpp include/logger/LogLevel.hpp

TEST_NAME = test_runner
TEST_SRC = src/test.cpp
TEST_DEPS = include/logger/Logger.hpp include/logger/LogLevel.hpp

.PHONY: all clean test

all: $(LIB_NAME) $(APP_NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_OBJ): $(LIB_SRC) $(LIB_DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(LIB_NAME): $(LIB_OBJ)
	$(CXX) -shared -o $@ $<

$(APP_NAME): $(APP_SRC) $(LIB_NAME) $(APP_DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -pthread -o $@ $< -L. -llogger -Wl,-rpath,.

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME) $(APP_NAME) $(TEST_NAME)

test: $(LIB_NAME) $(TEST_NAME)
	./$(TEST_NAME)

$(TEST_NAME): $(TEST_SRC) $(LIB_NAME) $(TEST_DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< -L. -llogger -Wl,-rpath,.
