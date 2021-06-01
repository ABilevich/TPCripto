CFLAGS= -Wall -Wextra -pedantic -pedantic-errors -O3 -std=gnu99 -Wno-unused-parameter -Wno-unused-result -Wno-unused-variable 
CC=gcc

all: main test

main: src/main.o src/images.o src/galois_8.o
	$(CC) src/main.c src/images.c src/galois_8.c $(CFLAGS) -o main

test: 
	cd tests; make all;

.PHONY: clean

clean:
	 rm src/*.o main; cd tests; make clean