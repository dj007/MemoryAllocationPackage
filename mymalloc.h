/* 
 * File:   mymalloc.h
 * Author: Amjad Al-Rikabi
 * Student ID: 260143211
 *
 * Created on March 25, 2013, 4:37 PM
 */

#ifndef MYMALLOC_H
#define	MYMALLOC_H

//Defines:
#define FIRST_FIT                         1
#define BEST_FIT                          2 

//Includes
#include <unistd.h>
#include "myerror.c"

//Function declarations
void * my_malloc(int size);
extern char *my_malloc_error;
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();

//Data structures
typedef struct MemoryBlock_struct {
    int lengthOfBlock;
    union MemoryBlock * FreeBlock; 
} MemoryBlock;

typedef struct FreeMemoryBlock_struct {
    int lengthOfBlock;
    union FreeMemoryBlock * previousFreeBlock;
    union FreeMemoryBlock * nextFreeBlock;
    void * FreeBlock; 
} FreeMemoryBlock;

typedef struct FreeDLL{
    FreeMemoryBlock * head;
    FreeMemoryBlock * tail;
} FreeDLL;

//Helper Functions for FreeDLL


//Translation Unit variables
static int TotalNumBytesAllocated = 0 ;
static int TotalFreeSpace = 0;
static int LargestContiguousFreeSpace = 0;

#endif	/* MYMALLOC_H */

