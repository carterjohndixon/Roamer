SRC_DIR = src
ECS_DIR = ECS
BUILD_DIR = build/debug
RELEASE_DIR = build/release
CC = g++
SRC_FILES = $(wildcard  $(SRC_DIR)/*.cpp)
ECS_FILES = $(wildcard  $(SRC_DIR)/$(ECS_DIR)/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib
COMPILER_FLAGS = -std=c++20 -Wall -O0 -g
RELEASE_FLAGS = -std=c++20 -Wall -O3 -g
LINKER_FLAGS = -lsdl2 -lsdl2_image -lsdl2_ttf

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) $(ECS_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)
run:
	./$(BUILD_DIR)/$(OBJ_NAME)
release:
	$(CC) $(RELEASE_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(RELEASE_DIR)/$(OBJ_NAME)
release_run:
	./$(RELEASE_DIR)/$(OBJ_NAME)