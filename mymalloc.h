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
typedef long long Align;

//union MemoryBlock_union {
//
//    struct {
//        int lengthOfBlock;
//        union MemoryBlock_union * FreeBlock;
//    } MemoryBlock_struct;
//    Align data; //This should force alignment of blocks to 64bit machine
//};
//typedef MemoryBlock_union MemoryBlock;
//
//union FreeMemoryBlock_union {
//
//    struct {
//        int lengthOfBlock;
//        union FreeMemoryBlock_union * previousFreeBlock;
//        union FreeMemoryBlock_union * nextFreeBlock;
//        union FreeMemoryBlock_union * FreeBlock;
//    } FreeMemoryBlock_struct;
//    Align data; //This should force alignment of blocks to 64bit machine
//};
//typedef FreeMemoryBlock_union FreeMemoryBlock;



struct MemBlock {
    int size;
    struct MemBlock * next;
    struct MemBlock * prev;
    Align Free;
    char tag[1];
} MemBlock;

typedef struct MemBlock FreeMemoryBlock;

typedef struct FreeDLL {
    FreeMemoryBlock * head;
    FreeMemoryBlock * tail;
} FreeDLL;

//Helper Functions 
static FreeMemoryBlock * ExtendHeap(FreeMemoryBlock* last, int size);
static FreeMemoryBlock * FindMemBlk(FreeMemoryBlock** last, int size);



//Translation Unit variables
static int TotalNumBytesAllocated = 0;
static int TotalFreeSpace = 0;
static int LargestContiguousFreeSpace = 0;

#endif	/* MYMALLOC_H */

