TARGET = main

CC = gcc
CC_FLAGS = -std=c99 -pedantic -Wall -Wextra

SRC_DIR = .
MAIN_C = main.c
FILES_C := $(wildcard $(SRC_DIR)/*.c)
FILES_H := $(wildcard $(SRC_DIR)/*.h)
CORE_FILES_C := $(filter-out $(SRC_DIR)/$(MAIN_C), $(FILES_C))

all = $(TARGET) run

$(TARGET): $(FILES_C) $(FILES_H)
	$(CC) $(CC_FLAGS) -O0 -DDEBUG $(FILES_C) -o $(TARGET)

debug: $(FILES_C) $(FILES_H)
	$(CC) $(CC_FLAGS) -g -DDEBUG -O0 $(FILES_C) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

gdb: debug
	gdb --ex "tty /dev/null" ./$(TARGET)

valgrind:
	valgrind --leak-check=full ./$(TARGET)

