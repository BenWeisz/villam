#include "glist.h"

GLIST_BLOCK* GLIST_BLOCK_create(const unsigned int capacity)
{
    GLIST_BLOCK* block = (GLIST_BLOCK*)malloc(sizeof(GLIST_BLOCK));
    if (block == NULL)
    {
        printf("ERROR: Failed to allocate memory for GLIST_BLOCK.\n");
        return NULL;
    }

    block->data = (BENCE*)malloc(sizeof(BENCE) * capacity);
    if (block->data == NULL)
    {
        printf("ERROR: Failed to allocate memory for GLIST_BLOCK data.\n");
        free(block);
        return NULL;
    }

    block->size = 0;
    block->capacity = capacity;
    block->next = NULL;

    return block;
}

GLIST_BLOCK* GLIST_BLOCK_destroy(GLIST_BLOCK* block)
{
    block->size = 0;
    block->capacity = 0;
    free(block->data);
    block->data = NULL;

    GLIST_BLOCK* next = block->next;
    block->next = NULL;
    free(block);
    return next;
}

GLIST* GLIST_create()
{
    GLIST* glist = (GLIST*)malloc(sizeof(GLIST));
    if (glist == NULL)
    {
        printf("ERROR: Failed to allocate memory for GLIST.\n");
        return NULL;
    }

    glist->size = 0;
    glist->head = NULL;
    glist->tail = NULL;

    return glist;
}

void GLIST_destroy(GLIST* glist)
{
    GLIST_BLOCK* curr_block = glist->head;
    while (curr_block != NULL)
    {
        curr_block = GLIST_BLOCK_destroy(curr_block);
    }

    glist->size = 0;
    glist->head = NULL;
    glist->tail = NULL;
    free(glist);
}

unsigned int GLIST_add(GLIST* glist, BENCE e)
{
    GLIST_BLOCK* block = glist->tail;
    
    // First block
    if (block == NULL)
    {
        block = GLIST_BLOCK_create(GLIST_MIN_BLOCK_CAPACITY);
        if (block == NULL) return 0;

        glist->size = 0;
        glist->head = block;
        glist->tail = block;
    }

    // Full block
    if (block->size == block->capacity)
    {
        if (block->next != NULL)
        {
            printf("ERROR: Leaking GLIST_BLOCK memory, aborting\n");
            return 0;
        }

        block->next = GLIST_BLOCK_create(block->capacity * 2);
        if (block->next == NULL) return 0;

        block = block->next;
        glist->tail = block;
    }

    block->data[block->size++] = e;
    glist->size++;
    return 1;
}

GLIST_ITERATOR GLIST_iterator(GLIST* glist)
{
    GLIST_ITERATOR iter;
    iter.block_e_i = 0;
    iter.block = glist->head;
    return iter;
}

BENCE* GLIST_next(GLIST_ITERATOR* iter)
{
    if (iter->block == NULL) return NULL;

    BENCE* e = &(iter->block->data[iter->block_e_i++]);
    
    if (iter->block_e_i == iter->block->size)
    {
        iter->block_e_i = 0;
        iter->block = iter->block->next;
    }

    return e;
}