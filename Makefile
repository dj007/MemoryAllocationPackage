## COMP 310 - Programming Assignment 2
## Amjad Al-Rikabi
## ID: 260143211

pa2_test : pa2_test.o mymalloc.o mymalloc.h myerror.o
	gcc -o pa2_test pa2_test.o mymalloc.o myerror.o

pa2_test.o : pa2_test.c mymalloc.h mymalloc.o
	gcc -c -Wall pa2_test.c mymalloc.o

mymalloc.o : mymalloc.h mymalloc.c
	gcc -c -Wall mymalloc.h mymalloc.c myerror.c
	
myerror.o : myerror.c
	gcc -c -Wall myerror.c

clean:
	rm *.o 

