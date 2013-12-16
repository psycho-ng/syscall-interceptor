CFLAGS=-g3 -O2 -Wall
CC=cc

all:
	$(CC) $(CFLAGS) main.c handle.c pex.c child.c -o ./a.out
