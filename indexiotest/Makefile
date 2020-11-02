CFLAGS=-Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl

all: indexiotest

indexiotest:	
	gcc $(CFLAGS) indexiotest.c $(LIBS) -o $@

clean:
	rm indexiotest test_index
