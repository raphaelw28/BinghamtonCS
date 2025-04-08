all: predictors

predictors: predictors.o
	gcc -o predictors predictors.o

predictors.o: predictors.c
	gcc -c predictors.c

clean:
	rm -f predictors.o predictors