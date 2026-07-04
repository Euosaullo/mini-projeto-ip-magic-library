# Build commands for the Magic Library project.

CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -Iinclude

SRC = src/main.c src/library.c src/files.c src/encryption.c
OUT = biblioteca

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	./$(OUT) data/biblioteca.txt

clean:
	rm -f $(OUT) *.o
