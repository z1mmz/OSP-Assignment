CC=gcc
CFLAGS= -Wall -pedantic -g

all: central44.o external44.o

central44.o: central44.c central44.h
	$(CC) $(CFLAGS) central44.c central44.h -o central44

external44.o: external44.c external44.h
	$(CC) $(CFLAGS) external44.c external44.h -o external44
