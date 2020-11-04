CFLAGS=-Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl

all: querier q

querier:
	gcc $(CFLAGS) query.c $(LIBS) -o $@

q:
	gcc $(CFLAGS) q.c $(LIBS) -o $@

clean:
	rm querier
	rm q
