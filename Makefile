CC = gcc
FLAGS = -Wall -Werror -o3

BUILD_FOLDER = build
SRC_FOLDER = src

SRC_FILES = $(wildcard $(SRC_FOLDER)/*.c)
BUILD_FILES = $(patsubst $(SRC_FOLDER)/%.c, $(BUILD_FOLDER)/%.o, $(SRC_FILES))
BUILD_FILES_DEB =  $(patsubst $(SRC_FOLDER)/%.c, $(BUILD_FOLDER)/deb_%.o, $(SRC_FILES))


OUTPUT = $(BUILD_FOLDER)/SHELL
OUTPUT_DEBUG = $(BUILD_FOLDER)/SHELL_D

default : $(OUTPUT)

debug : $(OUTPUT_DEBUG)

$(OUTPUT_DEBUG) : $(BUILD_FILES_DEB)
	$(CC) $(FLAGS) -g $(BUILD_FILES_DEB) -o $(OUTPUT_DEBUG)

$(BUILD_FOLDER)/deb_%.o : $(SRC_FOLDER)/%.c
	$(CC) $(FLAGS) -g -c $^ -o $@

$(OUTPUT): $(BUILD_FILES)
	$(CC) $(FLAGS)  $(BUILD_FILES) -o $(OUTPUT)

$(BUILD_FOLDER)/%.o : $(SRC_FOLDER)/%.c
	$(CC) $(FLAGS) -c $^ -o $@

clean :
	rm -rf $(BUILD_FOLDER)/*
