CFLAGS=-Wall
LDFLAGS=-lserialport
TSTFLAGS=-L. -lklug $(LDFLAGS)

all: lib test

.PHONY: test
test: test.c
	$(CC) $(CFLAGS) test.c $(TSTFLAGS) -o test
	LD_LIBRARY_PATH=. ./test

lib: 
	$(CC) $(CFLAGS) -c -fPIC src/port.c $(LDFLAGS) -o src/port.o
	$(CC) -shared src/port.o -o libklug.so

.PHONY: how2
how2:
	$(MAKE) -C how2

.PHONY: clean
clean:
	$(RM) src/*.o libklug.so test