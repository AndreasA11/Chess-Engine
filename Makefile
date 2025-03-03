SRC_DIR = src
BUILD_DIR = build
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = chessEngine
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -L/opt/homebrew/Cellar/sdl2/2.30.8/lib/ -L/opt/homebrew/Cellar/sdl2_image/2.8.2_2/lib
COMPILER_FLAGS = -std=c++17 -Wall

LINKER_FLAGS = -lsdl2

all: COMPILER_FLAGS += -O0
all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

debug: COMPILER_FLAGS += -g3 -DDEBUG -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG
debug: 
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)_debug
.PHONY: debug


release: COMPILER_FLAGS += -O3 -DNDEBUG	
release: 
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)_release
.PHONY: release



# make clean - remove .o files, executables
clean:
	rm -Rf $(BUILD_DIR)/*.dSYM
	rm -f $(BUILD_DIR)/$(OBJ_NAME) $(BUILD_DIR)/$(OBJ_NAME)_release $(BUILD_DIR)/$(OBJ_NAME)_debug
.PHONY: clean


