CC = gcc                     
CFLAGS = -Wall -Werror -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)            
TARGET = build/gigaBinAnalyzer 

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p build
	$(CC) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) src/*.o
