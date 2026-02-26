CC=gcc
CFLAGS=-Wall
LDFLAGS=-lserialport

all: list.c info.c
	$(CC) $(CFLAGS) list.c $(LDFLAGS) -o list
	$(CC) $(CFLAGS) info.c $(LDFLAGS) -o info
	./list