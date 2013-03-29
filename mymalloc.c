/* 
 * File:   mymalloc.c
 * Author: Amjad Al-Rikabi
 * Student ID: 260143211
 *
 * Created on March 27, 2013, 11:20 AM
 */

#include "mymalloc.h"

#define DEBUG_MALLOC    0
#define DEBUG_FREE      0

#if (DEBUG_MALLOC||DEBUG_FREE)
#include <stdio.h>
#endif

//Global Variables
static MemoryBlock * MemBlkPtr;

void *my_malloc(int size)
{
    void * curr_break = sbrk(0);

#if DEBUG_MALLOC
    printf("The curr_break value is: \t%p\n", curr_break);
#endif

    MemBlkPtr = sbrk(size + 1);
    
#if DEBUG_MALLOC
    printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
#endif
    
    if(MemBlkPtr != curr_break)
    {
        my_malloc_error = "ERROR:\tCannot allocate using sbrk() successfully!\n";
        perror(my_malloc_error);
        return 0;
    }
    
    //Update current Break
    if (brk((MemBlkPtr+size+1)) != 0)
    {
        my_malloc_error = "ERROR:\tCannot update using brk() successfully!\n";
        perror(my_malloc_error);
        printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
        return 0;
    }
    
#if DEBUG_MALLOC
    printf("The sbrk(0) value is: \t%p\n", MemBlkPtr);
    printf("The size value is: \t%i\n", size);
    printf("The MemBlkPtr->FreeBlock value is: \t%X\n", &(MemBlkPtr->FreeBlock));
#endif
    
    //Update the number of Bytes in LengthOfBlock
    MemBlkPtr->lengthOfBlock = size;
    
    //Increment the number of butes allocated
    TotalNumBytesAllocated += size;
    
#if DEBUG_MALLOC
    printf("The MemBlkPtr->FreeBlock value is: \t%p\n", MemBlkPtr->FreeBlock);
#endif
    
    return (void *) &(MemBlkPtr->FreeBlock);
    
}

//Print Allocation Info
void my_mallinfo() 
{
    printf("Total Number of Bytes ALLOCATED is: \t%i\n", TotalNumBytesAllocated);
    printf("Total Number of Bytes in FREE SPACE is: \t%i\n", TotalFreeSpace );
    printf("Total Number of Bytes in LARGEST CONTIGUOUS BLOCK is: \t%i\n", LargestContiguousFreeSpace);
}

void my_free(void *ptr)
{
    //If the argument of my_free is NULL, then the call should not free anything
    if (ptr == NULL) 
    {
#if DEBUG_FREE
    printf("Null Pointer detected!\n");
#endif
        return;
    }
    
    int size;
    
    FreeMemoryBlock* FMBlk;
    FMBlk->previousFreeBlock = ptr;
    size = FMBlk->lengthOfBlock;
    
#if DEBUG_FREE
    printf("\nThe size of the freed Block is: \t%i\n", size);
    printf("Ptr is: \t%p\n", ptr);
    printf("FMBlk->lengthOfBlock is: \t%i\n", FMBlk->lengthOfBlock);
#endif
    
    //Update the number of bytes freed
    TotalFreeSpace += size;
    
}