CC=gcc
CFLAGS = -lcheck -lrt -lm -lsubunit -std=gnu99 -pthread

all: tests

tests: galois_8_test 

galois_8_test: galois_8_test.o ../src/galois_8.c
	$(CC) galois_8_test.c -o galois_8_test.out $(CFLAGS)

.PHONY: clean

clean: 
	rm -rf *.o *.out