CC = gcc                     
CFLAGS = -Wall -Werror -Iinclude

SRC = $(filter-out src/main.c, $(wildcard src/*.c))
TEST_SRC = tests/tests.c $(SRC)
HEADERS = include/StoreDump.h include/LoadDump.h include/SortDump.h

OBJ = $(SRC:.c=.o)            
TARGET = build/gigaBinAnalyzer 
TEST_TARGET = build/tests

.PHONY: all clean

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJ) src/main.o
	@mkdir -p build
	$(CC) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf build
