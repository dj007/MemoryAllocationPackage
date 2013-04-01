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
#define MEMBLOCKSIZE    sizeof(struct MemBlock)

//MACROS:
#define ALIGN8(x) (((((x)-1)>>3)<<3)+8)

#if (DEBUG_MALLOC||DEBUG_FREE)
#include <stdio.h>
#endif

//Global Variables
static FreeMemoryBlock * Base = NULL;
//static FreeMemoryBlock * MemBlkPtr = NULL;
static FreeMemoryBlock * FreeMemBlkPtr;
static FreeDLL FreeMemBlkDLL;

void *my_malloc(int size) {
    FreeMemoryBlock * Blk, *last;
    int s;

    s = ALIGN8(size);

#if DEBUG_MALLOC
    printf("The size of FreeMemoryBlock is: \t%i\n", sizeof (FreeMemoryBlock));
    printf("The size of Pointer is:     \t%i\n", sizeof (Base));
    printf("The value of s is:          \t%i\n", s);
    printf("The Base pointer value is: \t%p\n", Base);
#endif
    //MemoryBlock *ptr, *prev_ptr;
    //unsigned numUnits;

    //numUnits = (size + sizeof(FreeMemoryBlock) - 1);
    //First check if there is no FreeDLL, then create one
    //if ((FreeMemBlkDLL.head = FreeMemBlkDLL.tail) == NULL)
    // {
    //     FreeMemoryBlock FreeMemBlk;
    //    FreeMemBlkDLL.head = FreeMemBlkDLL.tail = &(FreeMemBlk);
    // }

    if (Base) //DLL already exists
    {
#if DEBUG_MALLOC
        printf("There is a FreeDLL memblk \n");
#endif
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
    TotalNumBytesAllocated += size;

#if DEBUG_MALLOC
    printf("The Blk->Free value is: \t%i\n", Blk->Free);
#endif

    return (Blk->Free);

}

//Print Allocation Info

void my_mallinfo() {
    int NumOfElem = 0;
    FreeMemoryBlock * iterator = &(FreeMemBlkDLL.head);
    if ((FreeMemBlkDLL.head = FreeMemBlkDLL.tail) != NULL) NumOfElem++;
    while (iterator->next) {
        NumOfElem++;
        iterator = iterator->next;
    }

    printf("Total Number of Bytes ALLOCATED is: \t\t\t%i\n", TotalNumBytesAllocated);
    printf("Total Number of Bytes in FREE SPACE is: \t\t%i\n", TotalFreeSpace);
    printf("Total Number of Bytes in LARGEST CONTIGUOUS BLOCK is: \t%i\n", LargestContiguousFreeSpace);
    printf("Total Number of FREE BLOCKS is: \t\t\t%i\n", NumOfElem);
}

void my_free(void *ptr) {
    //If the argument of my_free is NULL, then the call should not free anything
    if (ptr == NULL) {
#if DEBUG_FREE
        printf("Null Pointer detected!\n");
#endif
        return;
    }

    //    FreeMemBlkPtr->previousFreeBlock = FreeMemBlkDLL.tail;
    //    FreeMemBlkPtr->next = NULL;
    //    FreeMemBlkDLL.tail->next=ptr;
    //    FreeMemBlkDLL.tail = ptr;

    FreeMemoryBlock FMBlk;
    FreeMemoryBlock *FMBlk_ptr;

    FMBlk_ptr = (FreeMemoryBlock*) ptr - 1;
    //    FMBlk_ptr->previousFreeBlock = FreeMemBlkDLL.tail;
    FMBlk_ptr->next = NULL;
    //FMBlk_ptr = &FMBlk;

    //    FMBlk_ptr->previousFreeBlock = ptr;
    //    FMBlk_ptr->previousFreeBlock = FreeMemBlkDLL.tail;
    //    FMBlk_ptr->next = NULL;
    //    FreeMemBlkDLL.tail = FMBlk_ptr;

#if DEBUG_FREE
    printf("\nThe value of ptr is: \t%p\n", ptr);
    printf("The size of INT is: \t%i\n", sizeof (int));
    printf("The size of the freed Block is: \t%i\n", FMBlk_ptr->lengthOfBlock);
    printf("FMBlk->lengthOfBlock address is: \t%p\n\n", FMBlk_ptr->lengthOfBlock);
    printf("FMBlk->lengthOfBlock is: \t%i\n\n", FMBlk_ptr->lengthOfBlock);
#endif

    //Update the number of bytes freed
    TotalFreeSpace += FMBlk_ptr->size;
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
    void * curr_break = sbrk(0);

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