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
typedef int Align;

struct MemBlock {
    int size;
    struct MemBlock * next;
    struct MemBlock * prev;
    Align isFree;
//    int Padding[2];
   // char tag[1];
} MemBlock;

typedef struct MemBlock FreeMemoryBlock;

#define BLOCK_SIZE      24  //sizeof(FreeMemoryBlock)

//Helper Functions 
static FreeMemoryBlock* ExtendHeap(FreeMemoryBlock* last, int size);
static FreeMemoryBlock* FindMemBlk(FreeMemoryBlock** last, int size);
static FreeMemoryBlock* MergeMemBlk(FreeMemoryBlock* Blk);
static FreeMemoryBlock* GetMemBlk(void * Ptr);
static int isAddrValid(void * Ptr);

//Translation Unit variables
static int TotalNumBytesAllocated = 0;
static int TotalFreeSpace = 0;
static int LargestContiguousFreeSpace = 0;
static int Policy = FIRST_FIT;

#endif	/* MYMALLOC_H */

