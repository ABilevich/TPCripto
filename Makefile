CFLAGS= -Wall -Wextra -pedantic -pedantic-errors -O3 -std=gnu99 -Wno-unused-result
CC=gcc

debug: CFLAGS += -g -fsanitize=address
debug: main

all: main test

main: src/main.o src/images.o src/galois_8.o src/interpolation.o
	$(CC) src/main.c src/images.c src/galois_8.c src/interpolation.c $(CFLAGS) -o main

test: 
	cd tests; make all;

.PHONY: clean

clean:
	 rm src/*.o main; cd tests; make clean