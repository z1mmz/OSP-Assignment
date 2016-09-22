CC=gcc
CFLAGS= -Wall -pedantic -g

all: central8.o external8.o

central8.o: central8.c central8.h
	$(CC) $(CFLAGS) central8.c central8.h -o central8

external8.o: external8.c external8.h
	$(CC) $(CFLAGS) external8.c external8.h -o external8