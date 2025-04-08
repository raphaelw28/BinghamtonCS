all: cache

cache: cache.o
	gcc -o cache cache.o

cache.o: cache.c
	gcc -c cache.c

clean:
	rm -f cache.o cache
