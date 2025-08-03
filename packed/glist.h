#ifndef GLIST_H
#define GLIST_H

/* 
    GLIST - Grow List 
    A list that is only meant to be added to, iterated over, then deallocated.
    
    Instantiation macro version: specify a TYPE and a NAME prefix to get a typed list.
    MACRO-ized by ChatGPT.
*/

#include <stdlib.h>
#include <stdio.h>

#define GLIST_MIN_BLOCK_CAPACITY 8

/* Declaration macro: in headers */
#define GLIST_DECLARE(TYPE, NAME)                                                \
                                                                                 \
    typedef struct NAME##_BLOCK                                                  \
    {                                                                            \
        unsigned int size;                                                       \
        unsigned int capacity;                                                   \
        TYPE* data;                                                              \
        struct NAME##_BLOCK* next;                                               \
    } NAME##_BLOCK;                                                              \
                                                                                 \
    typedef struct NAME                                                          \
    {                                                                            \
        unsigned int size;                                                       \
        NAME##_BLOCK* head;                                                      \
        NAME##_BLOCK* tail;                                                      \
    } NAME;                                                                      \
                                                                                 \
    typedef struct NAME##_ITERATOR                                               \
    {                                                                            \
        unsigned int block_e_i; /* index within current block */                 \
        NAME##_BLOCK* block;                                                     \
    } NAME##_ITERATOR;                                                           \
                                                                                 \
    /* API */                                                                    \
    NAME* NAME##_create(void);                                                   \
    void NAME##_destroy(NAME* glist);                                            \
    unsigned int NAME##_add(NAME* glist, TYPE e);                                \
    NAME##_ITERATOR NAME##_iterator(NAME* glist);                                \
    TYPE* NAME##_next(NAME##_ITERATOR* iter);

/* Definition macro: in one .c per instantiation */
#define GLIST_DEFINE(TYPE, NAME)                                                 \
                                                                                 \
    static NAME##_BLOCK* NAME##_BLOCK_create(const unsigned int capacity)        \
    {                                                                            \
        NAME##_BLOCK* block = (NAME##_BLOCK*)malloc(sizeof(NAME##_BLOCK));       \
        if (block == NULL)                                                       \
        {                                                                        \
            printf("ERROR: Failed to allocate memory for " #NAME "_BLOCK.\n");   \
            return NULL;                                                         \
        }                                                                        \
                                                                                 \
        block->data = (TYPE*)malloc(sizeof(TYPE) * capacity);                    \
        if (block->data == NULL)                                                 \
        {                                                                        \
            printf("ERROR: Failed to allocate memory for " #NAME " data.\n");    \
            free(block);                                                         \
            return NULL;                                                         \
        }                                                                        \
                                                                                 \
        block->size = 0;                                                         \
        block->capacity = capacity;                                              \
        block->next = NULL;                                                      \
                                                                                 \
        return block;                                                            \
    }                                                                            \
                                                                                 \
    static NAME##_BLOCK* NAME##_BLOCK_destroy(NAME##_BLOCK* block)               \
    {                                                                            \
        block->size = 0;                                                         \
        block->capacity = 0;                                                     \
        free(block->data);                                                       \
        block->data = NULL;                                                      \
                                                                                 \
        NAME##_BLOCK* next = block->next;                                        \
        block->next = NULL;                                                      \
        free(block);                                                             \
        return next;                                                             \
    }                                                                            \
                                                                                 \
    NAME* NAME##_create(void)                                                    \
    {                                                                            \
        NAME* glist = (NAME*)malloc(sizeof(NAME));                               \
        if (glist == NULL)                                                       \
        {                                                                        \
            printf("ERROR: Failed to allocate memory for " #NAME ".\n");         \
            return NULL;                                                         \
        }                                                                        \
                                                                                 \
        glist->size = 0;                                                         \
        glist->head = NULL;                                                      \
        glist->tail = NULL;                                                      \
                                                                                 \
        return glist;                                                            \
    }                                                                            \
                                                                                 \
    void NAME##_destroy(NAME* glist)                                             \
    {                                                                            \
        NAME##_BLOCK* curr_block = glist->head;                                  \
        while (curr_block != NULL)                                               \
        {                                                                        \
            curr_block = NAME##_BLOCK_destroy(curr_block);                       \
        }                                                                        \
        glist->size = 0;                                                         \
        glist->head = NULL;                                                      \
        glist->tail = NULL;                                                      \
        free(glist);                                                             \
    }                                                                            \
                                                                                 \
    unsigned int NAME##_add(NAME* glist, TYPE e)                                 \
    {                                                                            \
        NAME##_BLOCK* block = glist->tail;                                       \
                                                                                 \
        if (block == NULL)                                                       \
        {                                                                        \
            block = NAME##_BLOCK_create(GLIST_MIN_BLOCK_CAPACITY);               \
            if (block == NULL) return 0;                                         \
                                                                                 \
            glist->size = 0;                                                     \
            glist->head = block;                                                 \
            glist->tail = block;                                                 \
        }                                                                        \
                                                                                 \
        if (block->size == block->capacity)                                      \
        {                                                                        \
            if (block->next != NULL)                                             \
            {                                                                    \
                printf("ERROR: Leaking " #NAME "_BLOCK memory, aborting\n");     \
                return 0;                                                        \
            }                                                                    \
            block->next = NAME##_BLOCK_create(block->capacity * 2);              \
            if (block->next == NULL) return 0;                                   \
            block = block->next;                                                 \
            glist->tail = block;                                                 \
        }                                                                        \
                                                                                 \
        block->data[block->size++] = e;                                          \
        glist->size++;                                                           \
        return 1;                                                                \
    }                                                                            \
                                                                                 \
    NAME##_ITERATOR NAME##_iterator(NAME* glist)                                 \
    {                                                                            \
        NAME##_ITERATOR iter;                                                    \
        iter.block_e_i = 0;                                                      \
        iter.block = glist->head;                                                \
        return iter;                                                             \
    }                                                                            \
                                                                                 \
    TYPE* NAME##_next(NAME##_ITERATOR* iter)                                     \
    {                                                                            \
        if (iter->block == NULL) return NULL;                                    \
        TYPE* e = &(iter->block->data[iter->block_e_i++]);                       \
        if (iter->block_e_i == iter->block->size)                                \
        {                                                                        \
            iter->block_e_i = 0;                                                 \
            iter->block = iter->block->next;                                     \
        }                                                                        \
        return e;                                                                \
    }

#endif // GLIST_H
