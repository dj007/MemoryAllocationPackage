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
#define MEMBLOCKSIZE    24 //sizeof(struct MemBlock)

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
#if 1//DEBUG_MALLOC
        printf("There is a FreeDLL memblk \n");
#endif
        last = Base;
        Blk = FindMemBlk(last, s);
        if (Blk) {
#if 1//DEBUG_MALLOC
            printf("We found a FREE memblk in DLL! \n");
            printf("The Base pointer value is: \t%p\n", Base);
            printf("The last pointer value is: \t%p\n", last);
#endif
            Blk->isFree = 1;
        } else {
            Blk = ExtendHeap(last, s);
            if (!Blk) return NULL;
        }
    } else //There is no FreeDLL
    {
#if 1//DEBUG_MALLOC
        printf("CREATING A NEW MEMBLK \n");
#endif
        Blk = ExtendHeap(last, s);
#if DEBUG_MALLOC
        printf("The Blk pointer value is: \t%p\n", Blk);
#endif
        if (!Blk) return NULL;
        Base = Blk;
#if DEBUG_MALLOC
        printf("The Base pointer value is: \t%p\n", Base);
#endif
    }

    //Increment the number of butes allocated
    TotalNumBytesAllocated += Blk->size;

#if 1//DEBUG_MALLOC
    printf("The Blk->isFree value is: \t%i\n", Blk->isFree);
    printf("The Blk->size value is: \t%i\n", Blk->size);
#endif

    return &(Blk->isFree);
}

/**
 *Print Allocation Info 
 */
void my_mallinfo() {
    puts("\n==========MALLINFO===========\n");
    int NumOfElem = 0;
    LargestContiguousFreeSpace = 0;
    TotalFreeSpace = 0;
    FreeMemoryBlock * iterator = Base;
    if (Base) {
        NumOfElem++;
        LargestContiguousFreeSpace = Base ->size;
        TotalFreeSpace += Base ->size;
        printf("The Value of Base is: \t%p\n", Base);
        printf("The Size of Base is: \t%i\n", Base->size);
        printf("The Value of Base->next is: \t%p\n", Base->next);
        printf("The Value of Base->prev is: \t%p\n", Base->prev);
        printf("The value of Base->isFree is: \t%i\n", Base->isFree);
    }

    while ((Base)&&(iterator->next)) {
        NumOfElem++;
        printf("The Value of FreeElem[%i] is: \t%p\n", NumOfElem - 1, iterator->next);
        printf("The Size of FreeElem[%i] is: \t%i\n", NumOfElem - 1, iterator->size);
        printf("The Value of FreeElem[%i]->next is: \t%p\n", NumOfElem - 1, iterator->next->next);
        printf("The Value of FreeElem[%i]->prev is: \t%p\n", NumOfElem - 1, iterator->next->prev);
        printf("The value of FreeElem[%i]->isFree is: \t%i\n", NumOfElem - 1, iterator->next->isFree);
        if ((iterator->next) && (iterator->next->size > LargestContiguousFreeSpace))
            LargestContiguousFreeSpace = iterator->next->size;
        //Update the number of bytes freed
        TotalFreeSpace += iterator->next->size;
        if (isAddrValid(iterator->next)) {
            iterator = iterator->next;
        } else {
            break;
        }
    }

    printf("Total Number of Bytes ALLOCATED is: \t\t\t%i\n", TotalNumBytesAllocated);
    printf("Total Number of Bytes in FREE SPACE is: \t\t%i\n", TotalFreeSpace);
    printf("Total Number of Bytes in LARGEST CONTIGUOUS BLOCK is: \t%i\n", LargestContiguousFreeSpace);
    printf("Total Number of FREE BLOCKS is: \t\t\t%i\n", NumOfElem);
    printf("The Base pointer value is: \t%p\n", Base);
    printf("=========COMPLETED MALLINFO!==============\n");
}

void my_free(void *Ptr) {
#if DEBUG_FREE
    printf("INSIDE MY_FREE()!\n");
#endif
    //If the argument of my_free is NULL, then the call should not free anything
    if (Ptr == NULL) {
        printf("Null Pointer detected!\n");
        return;
    }
#if DEBUG_FREE
    printf("The Base pointer value is: \t%p\n", Base);
    printf("The value of ptr is: \t%p\n", Ptr);
    printf("The value of sbrk(0) is: \t%p\n", sbrk(0));
    printf("The value of Base is: \t%p\n", Base);
    printf("The value of isAddrValid(Ptr) is: \t%i\n", isAddrValid(Ptr));
#endif

    FreeMemoryBlock* Blk;

    if (isAddrValid(Ptr)) {
        Blk = GetMemBlk(Ptr);
#if DEBUG_FREE
        printf("The value of Blk->size is: \t%i\n", Blk->size);
        printf("The value of Blk->prev is: \t%p\n", Blk->prev);
        printf("The value of Blk->next is: \t%p\n", Blk->next);
#endif
        Blk->isFree = 0;
        //Merge with previous if possible
        if (Blk->prev && !(Blk->prev->isFree)) {
#if DEBUG_FREE
            printf("Merge with previous!\n");
#endif
            Blk = MergeMemBlk(Blk->prev);
        }
        //Merge with the next
        if (Blk->next) {
#if DEBUG_FREE
            printf("Merge with next\n");
#endif
            MergeMemBlk(Blk);
        }
        //        else {
        //            //isFree the end of the heap
        //            if (Blk->prev) {
        //#if DEBUG_FREE
        //                printf("isFree the end of the heap\n");
        //#endif
        //                Blk->prev->next = NULL;
        //            } else {
        //#if DEBUG_FREE
        //                printf("No more block!\n");
        //#endif      
        //                //No more block!
        //                Base = NULL;
        //            }
        //#if DEBUG_FREE
        //            printf("The size of the freed Block is: \t%i\n", Blk->size);
        //#endif
        //            //Update the number of bytes freed
        //            TotalFreeSpace += Blk->size;
        //            //Update brk()
        //            brk(Blk);
        //        }
    }
#if DEBUG_FREE
    else {
        printf("INVALID ADDRESS!\n");
        return NULL;
    }

    printf("COMPLETED FREEMEM!\n");
#endif
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
    MemBlkPtr = sbrk(0);

#if DEBUG_MALLOC
    printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
#endif

    MemBlkPtr = sbrk(size + MEMBLOCKSIZE);

#if DEBUG_MALLOC
    printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
#endif

    if (MemBlkPtr == (void*) - 1) {
        my_malloc_error = "ERROR:\tCannot allocate using sbrk() successfully!\n";
        perror(my_malloc_error);
        return 0;
    }

    //Update the number of Bytes in LengthOfBlock
    MemBlkPtr->size = size;
    MemBlkPtr->next = NULL;

    //Update current Break
    //    if (brk((MemBlkPtr + size + MEMBLOCKSIZE)) != 0) {
    //        my_malloc_error = "ERROR:\tCannot update using brk() successfully!\n";
    //        perror(my_malloc_error);
    //        printf("The MemBlkPtr value is: \t%p\n", MemBlkPtr);
    //        return 0;
    //        }

#if DEBUG_MALLOC
    printf("The sbrk(0) value is: \t%p\n", MemBlkPtr);
    printf("The size value is: \t%i\n", size);
    printf("The MemBlkPtr->isFree value is: \t%X\n", &(MemBlkPtr->isFree));
#endif

    if (last)
        last->next = MemBlkPtr;

    MemBlkPtr->isFree = 1;

    return MemBlkPtr;
}

void my_mallopt(int policy) {
    if ((policy == BEST_FIT) || (policy == FIRST_FIT)) {
        Policy = policy;
        char * str_tmp;
        str_tmp = (Policy == BEST_FIT) ? "Best Fit" : "First Fit";
        printf("The current allocation policy has been set to: \t%s\n", str_tmp);
        return;
    }
    perror("UNRECOGNIZED POLICY\n");
}

static FreeMemoryBlock* FindMemBlk(FreeMemoryBlock** last, int size) {
    FreeMemoryBlock * iter = Base;

    if (Policy == FIRST_FIT) {
        while (iter && !(!(iter->isFree) && iter->size >= size)) {
            *last = iter;
            iter = iter->next;
        }

        return (iter);
    } else if (Policy == BEST_FIT) {
        FreeMemoryBlock * best = iter;
        while (iter && !(!(iter->isFree) && iter->size >= size)) {
            *last = iter;
            iter = iter->next;
        }

    }
}

static FreeMemoryBlock* MergeMemBlk(FreeMemoryBlock * Blk) {
    if (Blk->next && !(Blk->next->isFree)) {
        Blk->size += BLOCK_SIZE + Blk->next->size;
        Blk->next = Blk->next->next;
        if (Blk->next)
            Blk->next->prev = Blk;
    }
#if DEBUG_FREE
    printf("\nCOMPLETED MERGE: The value of Blk is: \t%p\n", Blk);
    //    printf("\nThe value of Blk is: \t%p\n", Blk);
#endif    
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