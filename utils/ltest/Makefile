CFLAGS=-Wall -pedantic -pthread -std=c11 -I ../utils -L ../../lib -g
LIBS=-lutils -lcurl

all: lqueuetest lqueuetest_multi testH testQ

lqueuetest:	
	gcc $(CFLAGS) lqueuetest.c $(LIBS) -o $@

lqueuetest_multi:	
	gcc $(CFLAGS) lqueuetest_multi.c $(LIBS) -o $@

testH: #../queue.o ../lqueue.o ../hash.o ../lhash.o testH.o
	gcc $(CFLAGS) testH.o $(LIBS) -o testH

testQ: #../queue.o ../lqueue.o ../hash.o ../lhash.o testQ.o
	gcc $(CFLAGS) testQ.o $(LIBS) -o testQ

clean:
	rm lqueuetest lqueuetest_multi testH testQ
