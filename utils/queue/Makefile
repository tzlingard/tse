CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: queuetest

queue.o:	queue.c queue.h
	gcc $(CFLAGS) -c queue.c

queuetest.o:	queuetest.c queue.h
	gcc $(CFLAGS) -c queuetest.c

queuetest: queue.o queuetest.o
	gcc $(CFLAGS) queue.o  queuetest.o -o queuetest

clean:
	rm -f *.o queuetest
