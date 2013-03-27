/* 
 * File:   mymalloc.h
 * Author: Amjad Al-Rikabi
 * Student ID: 260143211
 *
 * Created on March 25, 2013, 4:37 PM
 */

#ifndef MYMALLOC_H
#define	MYMALLOC_H

//Function declarations
void * my_malloc(int size);
extern char *my_malloc_error;
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();

#endif	/* MYMALLOC_H */

