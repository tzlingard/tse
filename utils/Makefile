CFLAGS=-Wall -pedantic -std=c11 -I. -g
OFILES=queue.o hash.o webpage.o pageio.o indexio.o lqueue.o lhash.o
all: $(OFILES)
	ar cr ~/ENGS50/tse/lib/libutils.a $(OFILES)

hash.o:	hash.c hash.h queue.h
	gcc $(CFLAGS) -c hash.c

lhash.o: hash.c hash.h queue.h lhash.h
	gcc $(CFLAGS) -c lhash.c

hashtest.o:	hashtest.c hash.h queue.h
	gcc $(CFLAGS) -c hashtest.c

queue.o:	queue.c queue.h
	gcc $(CFLAGS) -c queue.c

lqueue.o:	lqueue.c lqueue.h queue.h
	gcc $(CFLAGS) -c lqueue.c

webpage.o:	webpage.c webpage.h
	gcc $(CFLAGS) -c webpage.c

pageio.o:  pageio.c pageio.h
	gcc $(CFLAGS) -c pageio.c 

hashtest: hash.o hashtest.o queue.o
	gcc $(CFLAGS) hash.o  hashtest.o queue.o -o hashtest

pageiotest.o:  pageiotest.c pageio.h webpage.h
	gcc $(CFLAGS) -c pageiotest.c 

pageiotest: $(OFILES)
	gcc $(CFLAGS) pageiotest.o -o pageiotest

indexio.o:  indexio.h indexio.c queue.h hash.h webpage.h
	gcc $(CFLAGS) -c indexio.c 

indexiotest.o: indexiotest.c indexio.h hash.h queue.h
	gcc $(CFLAGS) -c indexiotest.c

clean:
	rm -f *.o hashtest pageiotest indexiotest
