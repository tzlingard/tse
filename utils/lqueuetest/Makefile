CFLAGS=-Wall -pedantic -pthread -std=c11 -I ../utils -L ../../lib -g
LIBS=-lutils -lcurl

all: lqueuetest lqueuetest_multi

lqueuetest:	
	gcc $(CFLAGS) lqueuetest.c $(LIBS) -o $@

lqueuetest_multi:	
	gcc $(CFLAGS) lqueuetest_multi.c $(LIBS) -o $@

clean:
	rm lqueuetest lqueuetest_multi
