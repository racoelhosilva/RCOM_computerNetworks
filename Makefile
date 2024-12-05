CC = gcc
CFLAGS = -Wall -Werror -pedantic
BIN = bin

all: download

download: download.o
	$(CC) $(CFLAGS) -o $(BIN)/download $(BIN)/download.o

download.o: download.c
	$(CC) $(CFLAGS) -c download.c -o $(BIN)/download.o

clean:
	rm -f $(BIN)/*

.PHONY: all clean