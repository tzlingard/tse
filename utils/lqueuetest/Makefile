CFLAGS=-Wall -pedantic -pthread -std=c11 -I ../utils -L ../../lib -g
LIBS=-lutils -lcurl

all: lqueuetest

lqueuetest:	
	gcc $(CFLAGS) lqueuetest.c $(LIBS) -o $@

clean:
	rm lqueuetest
