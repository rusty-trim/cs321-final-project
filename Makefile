CXX      := g++
CC       := gcc
SRC_DIR  := src
BUILD_DIR:= build
TARGET   := game.out

CXXFLAGS := -std=c++17 -Wall -Wextra $(shell sdl2-config --cflags) -Iinclude
CFLAGS   := $(shell sdl2-config --cflags) -Iinclude
LIBS     := $(shell sdl2-config --libs) -lGL -ldl

MODE ?= debug
ifeq ($(MODE),debug)
    CXXFLAGS += -g -O0
    CFLAGS   += -g -O0
else
    CXXFLAGS += -O2 -DNDEBUG
    CFLAGS   += -O2 -DNDEBUG
endif

CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(CPP_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) \
            $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS     := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

-include $(DEPS)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
