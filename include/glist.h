#ifndef GLIST_H
#define GLIST_H

/* 
    GLIST - Grow List 
    A list that is only meant to be added to, iterated over, then deallocated.
*/

#include <stdlib.h>
#include <stdio.h>

typedef int BENCE;

#define GLIST_MIN_BLOCK_CAPACITY 8

typedef struct GLIST_BLOCK
{
    unsigned int size;
    unsigned int capacity;
    BENCE* data;
    struct GLIST_BLOCK* next;
} GLIST_BLOCK;

GLIST_BLOCK* GLIST_BLOCK_create(const unsigned int capacity);
GLIST_BLOCK* GLIST_BLOCK_destroy(GLIST_BLOCK* block);

typedef struct GLIST
{
    unsigned int size;
    GLIST_BLOCK* head;
    GLIST_BLOCK* tail;
} GLIST;

GLIST* GLIST_create();
void GLIST_destroy(GLIST* glist);

unsigned int GLIST_add(GLIST* glist, BENCE e); 

typedef struct GLIST_ITERATOR
{
    // Index of next element in the block
    unsigned int block_e_i;
    GLIST_BLOCK* block;
} GLIST_ITERATOR;

GLIST_ITERATOR GLIST_iterator(GLIST* glist);
BENCE* GLIST_next(GLIST_ITERATOR* iter);

#endif // GLIST_H