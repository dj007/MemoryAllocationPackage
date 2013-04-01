/* 
 * File:   mymalloc.c
 * Author: Amjad Al-Rikabi
 * Student ID: 260143211
 *
 * Created on March 27, 2013, 11:20 AM
 */

#include "mymalloc.h"

#define DEBUG_MALLOC    1
#define DEBUG_FREE      1
#define MEMBLOCKSIZE    sizeof(struct MemBlock)

//MACROS:
#define ALIGN(x) (((((x)-1)>>3)<<3)+8) //For 64-bit system

#if (DEBUG_MALLOC||DEBUG_FREE)
#include <stdio.h>
#endif

//Global Variables
static FreeMemoryBlock * Base = NULL;
//static FreeDLL FreeMemBlkDLL;

void *my_malloc(int size) {
    FreeMemoryBlock * Blk, *last;
    int s;

    s = ALIGN(size);

#if DEBUG_MALLOC
    printf("The size of FreeMemoryBlock is: \t%i\n", sizeof (FreeMemoryBlock));
    printf("The size of Pointer is:     \t%i\n", sizeof (Base));
    printf("The value of s is:          \t%i\n", s);
    printf("The Base pointer value is: \t%p\n", Base);
#endif

    if (Base) //DLL already exists
    {
#if DEBUG_MALLOC
        printf("There is a FreeDLL memblk \n");
#endif
        last = Base;
        Blk = FindMemBlk(last, s);
        if (Blk) {
            Blk->Free = 0;
        } else {
            Blk = ExtendHeap(last, s);
            if (!Blk) return NULL;
        }
    } else //There is no FreeDLL
    {
#if DEBUG_MALLOC
        printf("There is no FreeDLL memblk \n");
#endif
        Blk = ExtendHeap(last, s);
        if (!Blk) return NULL;
        Base = Blk;
#if DEBUG_MALLOC
        printf("The Base pointer value is: \t%p\n", Base);
#endif
    }

    //Increment the number of butes allocated
    TotalNumBytesAllocated += Blk->size;

#if DEBUG_MALLOC
    printf("The Blk->Free value is: \t%i\n", Blk->Free);
    printf("The Blk->size value is: \t%i\n", Blk->size);
#endif
    
    return &(Blk->Free);

}

/**
 *Print Allocation Info 
 */
void my_mallinfo() {
    int NumOfElem = 0;
    LargestContiguousFreeSpace = 0;
    FreeMemoryBlock * iterator = Base;
    if (Base) {
        NumOfElem++;
        LargestContiguousFreeSpace = Base ->size;
    }

    while ((Base)&&(iterator->next)) {
        NumOfElem++;
        if (iterator->next->size > LargestContiguousFreeSpace)
            LargestContiguousFreeSpace = iterator->next->size;
        iterator = iterator->next;
    }

    printf("Total Number of Bytes ALLOCATED is: \t\t\t%i\n", TotalNumBytesAllocated);
    printf("Total Number of Bytes in FREE SPACE is: \t\t%i\n", TotalFreeSpace);
    printf("Total Number of Bytes in LARGEST CONTIGUOUS BLOCK is: \t%i\n", LargestContiguousFreeSpace);
    printf("Total Number of FREE BLOCKS is: \t\t\t%i\n", NumOfElem);
}

void my_free(void *Ptr) {
    //If the argument of my_free is NULL, then the call should not free anything
    if (Ptr == NULL) {
#if DEBUG_FREE
        printf("Null Pointer detected!\n");
#endif
        return;
    }

    FreeMemoryBlock* Blk;

#if DEBUG_FREE
        printf("The value of ptr is: \t%p\n", Ptr);
        printf("The value of sbrk(0) is: \t%p\n", sbrk(0));
        printf("The value of Base is: \t%p\n", Base);
        printf("The value of isAddrValid(Ptr) is: \t%i\n", isAddrValid(Ptr));
#endif
    
    if (isAddrValid(Ptr)) {
        Blk = GetMemBlk(Ptr);
#if DEBUG_FREE
        printf("The value of ptr is: \t%p\n", Ptr);
        printf("The value of Blk is: \t%p\n", Blk);
        printf("The value of Blk->size is: \t%i\n", Blk->size);
        printf("The value of Blk->prev is: \t%p\n", Blk->prev);
        printf("The value of Blk->next is: \t%p\n", Blk->next);
#endif
        Blk->Free = 1;
        //Merge with previous if possible
        if (Blk->prev && Blk->prev->Free) {
#if DEBUG_FREE
            printf("Merge with previous if possible\n");
#endif
            Blk = MergeMemBlk(Blk->prev);
        }
        //Merge with the next
        if (Blk->next) {
#if DEBUG_FREE
            printf("Merge with the next\n");
#endif
            MergeMemBlk(Blk);
        } else {
            //Free the end of the heap
            if (Blk->prev) {
#if DEBUG_FREE
                printf("Free the end of the heap\n");
#endif
                Blk->prev->next = NULL;
            } else {
#if DEBUG_FREE
                printf("No more block\n");
#endif      
                //No more block
                Base = NULL;
            }
#if DEBUG_FREE
            printf("The size of the freed Block is: \t%i\n", Blk->size);
#endif
            //Update the number of bytes freed
            TotalFreeSpace += Blk->size;
            //Update brk()
            brk(Blk);
        }
    }
#if DEBUG_FREE
    else
    {        
        printf("INVALID ADDRESS!\n");
    }
#endif
        
#if DEBUG_FREE
//    printf("\nThe value of ptr is: \t%p\n", Ptr);
//    printf("\nThe value of Blk is: \t%p\n", Blk);
#endif

    //Update the number of bytes freed
    //TotalFreeSpace += Blk->size;
}

/**
 * This function aims to Extend the Heap using OS help using brk() & sbrk()
 * @param last
 * @param size
 * @return 
 */
static FreeMemoryBlock* ExtendHeap(FreeMemoryBlock* last, int size) {
    FreeMemoryBlock * MemBlkPtr;
    //Allocate Space from OS:
    void* curr_break = sbrk(0);

#if DEBUG_MALLOC
    printf("The curr_break value is: \t%p\n", curr_break);
#endif

    MemBlkPtr = sbrk(size + MEMBLOCKSIZE);

#if DEBUG_MALLOC
    printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
#endif

    if (MemBlkPtr != curr_break) {
        my_malloc_error = "ERROR:\tCannot allocate using sbrk() successfully!\n";
        perror(my_malloc_error);
        return 0;
    }

    //Update the number of Bytes in LengthOfBlock
    MemBlkPtr->size = size;
    MemBlkPtr->next = NULL;

    //Update current Break
    if (brk((MemBlkPtr + size + MEMBLOCKSIZE)) != 0) {
        my_malloc_error = "ERROR:\tCannot update using brk() successfully!\n";
        perror(my_malloc_error);
        printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
        return 0;
    }

#if DEBUG_MALLOC
    printf("The sbrk(0) value is: \t%p\n", MemBlkPtr);
    printf("The size value is: \t%i\n", size);
    printf("The MemBlkPtr->Free value is: \t%X\n", &(MemBlkPtr->Free));
#endif

    if (last)
        last->next = MemBlkPtr;

    MemBlkPtr->Free = 0;

    return MemBlkPtr;
}

static FreeMemoryBlock* FindMemBlk(FreeMemoryBlock** last, int size) {
    FreeMemoryBlock * iter = Base;
    while (iter && !(iter->Free && iter->size >= size)) {
        *last = iter;
        iter = iter->next;
    }

    return (iter);
}

static FreeMemoryBlock* MergeMemBlk(FreeMemoryBlock * Blk) {
    if (Blk->next && Blk->next->Free) {
        Blk->size += BLOCK_SIZE + Blk->next->size;
        Blk->next = Blk->next->next;
        if (Blk->next)
            Blk->next->prev = Blk;
    }
    return (Blk);
}

static FreeMemoryBlock* GetMemBlk(void * Ptr) {
    char *tmp;
    tmp = Ptr;
    return (Ptr = tmp -= BLOCK_SIZE);
}

static int isAddrValid(void * Ptr) {
    if (Base) {
        if (Ptr > Base && Ptr < sbrk(0))
            return 1;
    }
    return 0;
}