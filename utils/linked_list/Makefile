CFLAGS=-Wall -pedantic -std=c11 -I. -g
all:		listtest

list.o:		list.c list.h
		gcc $(CFLAGS) -c list.c

listtest.o:	listtest.c list.h
		gcc $(CFLAGS) -c listtest.c

listtest:	listtest.o list.o
		gcc $(CFLAGS) listtest.o list.o -o listtest

clean:
		rm -f *.o listtest
