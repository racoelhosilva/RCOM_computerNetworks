CC = gcc
CFLAGS = -Wall -Werror -pedantic
TARGET = download

all: $(TARGET)

$(TARGET): download.o
	$(CC) $(CFLAGS) -o $(TARGET) download.o

download.o: download.c
	$(CC) $(CFLAGS) -c download.c

clean:
	rm -f $(TARGET) download.o

.PHONY: all clean