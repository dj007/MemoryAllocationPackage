/* 
 * File:   mymalloc.h
 * Author: Amjad Al-Rikabi
 * Student ID: 260143211
 *
 * Created on March 25, 2013, 4:37 PM
 */

#ifndef MYMALLOC_H
#define	MYMALLOC_H

//Includes
#include <unistd.h>

//Function declarations
void * my_malloc(int size);
extern char *my_malloc_error;
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();

//Data structures
typedef struct MemoryBlock {
    int lengthOfBlock;
//    void * previousFreeBlock;
//    void * nextFreeBlock;
    void * FreeBlock; 
} MemoryBlock;

typedef struct FreeDLL{
    MemoryBlock * head;
} FreeDLL;

//Translation Unit variables
static int TotalNumBytesAllocated = 0 ;
static int TotalFreeSpace = 0;
static int LargestContiguousFreeSpace = 0;

#endif	/* MYMALLOC_H */

