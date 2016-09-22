CC=gcc
CFLAGS= -Wall -pedantic -g

all: central.o external.o

central.o: central.c central.h
	$(CC) $(CFLAGS) central.c central.h -o central

external.o: external.c external.h
	$(CC) $(CFLAGS) external.c external.h -o external