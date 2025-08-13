CXX = gcc
FLAGS = -Wall -Werror -o3 -g -fsanitize=address

BUILD_FOLDER = build
SRC_FOLDER = src

SRC_FILES = $(wildcard $(SRC_FOLDER)/*.c)
BUILD_FILES = $(patsubst $(SRC_FOLDER)/%.c, $(BUILD_FOLDER)/%.o, $(SRC_FILES))

OUTPUT = $(BUILD_FOLDER)/SHELL


all : $(OUTPUT)

$(OUTPUT) : $(SRC_FILES)
	$(CXX) $(SRC_FILES) -o $(OUTPUT)

#$(OUTPUT): $(BUILD_FILES)
#	$(CXX) $(FLAGS)  $(BUILD_FILES) -o $(OUTPUT)

$(BUILD_FOLDER)/%.o : $(SRC_FOLDER)/%.c
	$(CXX) $(FLAGS) -c $^ -o $@

clean :
	rm -rf $(BUILD_FOLDER)/*
