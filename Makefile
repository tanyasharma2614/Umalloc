CC = gcc
CFLAGS = -g -c
AR = ar -rc
RANLIB = ranlib


all: umalloc.a memgrind

memgrind:
	$(CC) -g -w -o memgrind memgrind.c -L./ -lumalloc

umalloc.a: umalloc.o
	$(AR) libumalloc.a umalloc.o
	$(RANLIB) libumalloc.a

umalloc.o: umalloc.h
	$(CC) $(CFLAGS) umalloc.c

clean:
	rm -rf *.o *.a memgrind
