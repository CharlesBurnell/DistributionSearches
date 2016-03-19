

all:
	gcc -Wall distributions.c -o distributions.o -lm

example: all
	./distributions.o
	./distributions.o -u -b 100 1 100
	./distributions.o -u -l 100 1 100
	./distributions.o -e -b 100 1 100 .76
	./distributions.o -e -l 100 1 100 .75
	./distributions.o -n -b 100 1 100 50 5
	./distributions.o -n -l 100 1 100 50 5
clean:
	rm distributions.o
