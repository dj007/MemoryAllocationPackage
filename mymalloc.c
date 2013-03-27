/* 
 * File:   mymalloc.c
 * Author: Amjad Al-Rikabi
 * Student ID: 260143211
 *
 * Created on March 27, 2013, 11:20 AM
 */

#include "mymalloc.h"

#define DEBUG_MALLOC 0

#if DEBUG_MALLOC
#include <stdio.h>
#endif

void *my_malloc(int size)
{
    MemoryBlock * MemBlkPtr;
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
        printf("ERROR:\tCannot allocate using sbrk() successfully!\n");
        return 0;
    }
    
    //Update current Break
    if (brk((MemBlkPtr+size+1)) != 0)
    {
        printf("ERROR:\tCannot update using brk() successfully!\n");
        printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
        return 0;
    }
    
#if DEBUG_MALLOC
    printf("The MemBlkPtr->FreeBlock value is: \t%p\n", MemBlkPtr->FreeBlock);
    printf("The size value is: \t%i\n", size);
    printf("The sbrk(0) value is: \t%p\n", MemBlkPtr);
#endif
    
    return MemBlkPtr->FreeBlock;
}