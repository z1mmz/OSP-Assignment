CC=gcc
CFLAGS= -Wall -pedantic -g -pthread

all: central442t.o external442t.o

central442t.o: central442t.c central442t.h
	$(CC) $(CFLAGS) central442t.c central442t.h -o central442t

external442t.o: external442t.c external442t.h
	$(CC) $(CFLAGS) external442t.c external442t.h -o external442t
