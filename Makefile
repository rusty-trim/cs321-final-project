CXX        := g++
CC         := gcc
SRC_DIR    := src
VENDOR_DIR := vendor
BUILD_DIR  := build
TARGET     := game.out

# Automatically find ALL nested header directories under include/ and vendor/
INC_DIRS   := $(shell find include vendor -type d)
INC_FLAGS  := $(addprefix -I,$(INC_DIRS))

CXXFLAGS   := -std=c++17 -Wall -Wextra $(shell sdl2-config --cflags) $(INC_FLAGS)
CFLAGS     := $(shell sdl2-config --cflags) $(INC_FLAGS)
LIBS       := $(shell sdl2-config --libs) -lGL -ldl

MODE ?= debug
ifeq ($(MODE),debug)
    CXXFLAGS += -g -O0
    CFLAGS   += -g -O0
else
    CXXFLAGS += -O2 -DNDEBUG
    CFLAGS   += -O2 -DNDEBUG
endif

# 1. Find ALL C and C++ sources recursively inside src/ and vendor/
CPP_SRCS := $(shell find $(SRC_DIR) $(VENDOR_DIR) -type f -name "*.cpp" 2>/dev/null)
C_SRCS   := $(shell find $(SRC_DIR) $(VENDOR_DIR) -type f -name "*.c" 2>/dev/null)

# 2. Map source paths directly to build/ while preserving directory paths
#    e.g., src/world/chunk.cpp -> build/src/world/chunk.o
CPP_OBJS := $(CPP_SRCS:%=$(BUILD_DIR)/%.o)
C_OBJS   := $(C_SRCS:%=$(BUILD_DIR)/%.o)

OBJS     := $(CPP_OBJS) $(C_OBJS)
DEPS     := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

# Compile C++ sources (mkdir -p $(dir $@) creates nested subdirectories inside build/)
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Compile C sources
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
