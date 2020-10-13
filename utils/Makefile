CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o
all: $(OFILES)
	ar cr ~/ENGS50/tse/lib/libutils.a $(OFILES)

hash.o:	hash.c hash.h queue.h
	gcc $(CFLAGS) -c hash.c

hashtest.o:	hashtest.c hash.h queue.h
	gcc $(CFLAGS) -c hashtest.c

queue.o:	queue.c queue.h
	gcc $(CFLAGS) -c queue.c

webpage.o:	webpage.c webpage.h
	gcc $(CFLAGS) -c webpage.c

hashtest: hash.o hashtest.o queue.o
	gcc $(CFLAGS) hash.o  hashtest.o queue.o -o hashtest

clean:
	rm -f *.o hashtest
