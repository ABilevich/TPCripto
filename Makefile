CFLAGS= -Wall -Wextra -pedantic -pedantic-errors -O3 -std=gnu99 -Wno-unused-parameter -Wno-unused-result -Wno-unused-variable 
CC=gcc

all: main

main: src/main.o src/images.o src/galois_8.o
	$(CC) src/main.c src/images.c src/galois_8.c $(CFLAGS) -o main

clean:
	 rm src/main.o src/images.o src/galois_8.o main