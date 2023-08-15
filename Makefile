CC = gcc
CFLAGS = -Wall -Wextra

all: practica3

practica3: practica3.o
	$(CC) $(CFLAGS) -o $@ $^

practica3.o: practica3.c estructura.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f practica3 *.o
