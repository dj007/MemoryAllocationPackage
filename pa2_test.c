/*
 * =====================================================================================
 *
 *      Filename:  pa2_test.c
 *
 *   	Description:  Example of testing code of MyMalloc.
 *
 *      Version:  1.0
 *      Created:  18/03/2013 8:30:30 AM
 *   	Revision:  none
 *      Compiler:  gcc
 *
 *      Author:  Devarun Bhattacharya
 * =====================================================================================
 */
/* Includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Uncomment the following line -- this should be your header file
#include "mymalloc.h"	

// We assume you have defined the following two definitions
// If so, you should remove these..
// If not, move them to your mymalloc.h file
#define FIRST_FIT                         1
#define BEST_FIT                          2



// Comment out the following 4 lines
//#define my_malloc(X)                      malloc(X)
//#define my_free(X)                        free(X)
//#define my_mallopt(X)                     dummymallopt(X)
//#define my_mallinfo                       dummymallinfo


// No need to modify anything below.. unless you find a bug in the tester!
// Don't modify the tester to circumvent the bug in your code!

void dummymallopt()
{
  puts("No optimization");
}

void dummymallinfo()
{
  puts("No info");
}


int main(int argc, char *argv[])
{
	int i, count = 0;
	void *ptr, *limitafter = NULL, *limitbefore = NULL;
	char *c[32], *ct;
        
#if 0 //Amjad Debug test
        puts("Amjad Debug test:\n");
        char *str_temp = my_malloc(10);
        str_temp = "hello!";
        int * ptr1;
        printf("Ptr_address before my_malloc:\t %p\n", ptr1);
        printf("Brk address before my_malloc:\t %p\n", sbrk(0));
        ptr = my_malloc(8);
        printf("Ptr_address after my_malloc:\t %p\n", ptr1);
        printf("Brk address after my_malloc:\t %p\n", sbrk(0));
        printf("str_temp = %s, \tsize: %i\n", str_temp, sizeof(str_temp));
#endif    
#if 0 //Amjad Debug test
        puts("Amjad Debug test:\n");
        char *str_temp = my_malloc(13);
     //   str_temp = "hello!";
    //    int * ptr1;
        printf("str_temp address after my_malloc:\t %p\n", str_temp);
     //   printf("Ptr_address before my_malloc:\t %p\n", ptr1);
     //   printf("Brk address before my_malloc:\t %p\n", sbrk(0));
      //  ptr = my_malloc(8);
     //   printf("Ptr_address after my_malloc:\t %p\n", ptr1);
       // printf("Brk address after my_malloc:\t %p\n", sbrk(0));
       // printf("str_temp = %s, \tsize: %i\n", str_temp, sizeof(str_temp));
        return;
#endif        
#if 0 //Amjad Debug test
        puts("Amjad Debug test:\n");
        printf("The size of int is: \t%i\n", sizeof(int));
        printf("The size of Align is: \t%i\n", sizeof (Align));
        printf("The size of FreeMemoryBlock* is: \t%i\n", sizeof(FreeMemoryBlock*));
        my_mallinfo();
        char *str_temp;
        str_temp = (char *)my_malloc(15);
        printf("The value of str_temp is: \t%p\n", str_temp);
        printf("The address of str_temp is: \t%x\n", str_temp);
        my_free(str_temp);
        my_mallinfo();
        char * temp2 = (char *)my_malloc(34);
        printf("The value of temp2 is: \t%p\n", temp2);
        printf("The address of temp2 is: \t%x\n", temp2);
        my_free(temp2);
        my_mallinfo();
        char * temp3 = (char *)my_malloc(16);
        printf("The value of temp3 is: \t%p\n", temp3);
        printf("The address of temp3 is: \t%x\n", temp3);
        my_free(temp3);
        my_mallinfo();
        return;
#endif
	puts("Hole finding test....");

	// Allocating 32 kbytes of memory..
	for(i=0; i<32;i++)					
		c[i] = (char*)my_malloc(1024);
	// Now deallocating some of the slots ..to free
	for(i=10; i<18;i++)
		my_free(c[i]);
        my_mallinfo();
	// Allocate some storage .. this should go into the freed storage
	ct = (char*)my_malloc(5*1024);

	// First test, are you finding the available holes.
	if(ct < c[31])						
		puts("\t\t\t\t Passed\n");
	else
		puts("\t\t\t\t Failed\n");

	puts("Program break expansion test...");

	count = 0;
	for(i=1; i<40; i++) {
	  limitbefore = sbrk(0);
	  ptr = my_malloc(1024*32*i);
	  limitafter = sbrk(0);

	  if(limitafter > limitbefore)
	    count++;

	}

#ifndef my_malloc
	if (count > 0 && count < 40)
	  puts("\t\t\t\t Passed");
	else
	  puts("\t\t\t\t Failed");
#else
	puts("\t\t\t\t skipped");
#endif

	puts("Check for first fit algorithm.... ");
	my_mallopt(FIRST_FIT);


	// Allocating 512 kbytes of memory..
	for(i=0; i<32;i++)					
		c[i] = (char*)my_malloc(16*1024);

	// Now deallocating some of the slots ..to free
	my_free(c[31]);
	my_free(c[30]);
	my_free(c[29]);
	my_free(c[28]);
	my_free(c[27]);


	my_free(c[20]);
	my_free(c[19]);


	my_free(c[10]);
	my_free(c[9]);
	my_free(c[8]);


	char *cp2 = my_malloc(16*1024*2);

	if (cp2 == c[8] || cp2 == c[9])
	  puts("\t\t\t\t Passed");
	else
	  puts("\t\t\t\t Failed");

	
	my_free(cp2);
	// Now try the other method...

	puts("Check for best first algorithm.... ");
	my_mallopt(BEST_FIT);	

	char *cp3 = my_malloc(16*1024*2);

	if (cp3 == c[19])
	  puts("\t\t\t\t Passed");
	else
	  puts("\t\t\t\t Failed");

	puts("Print some information..");
	my_mallinfo();
        
	return 0;
}
