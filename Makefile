CFLAGS= -Wall -Wextra -pedantic -pedantic-errors -O3 -std=c99 -Wno-unused-parameter -Wno-unused-result -Wno-unused-variable 
CC=gcc

all: main

main: src/main.o src/images.o
	$(CC) src/main.c src/images.c $(CFLAGS) -o main

clean:
	 rm src/main.o src/images.o main