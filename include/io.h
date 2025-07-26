#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <stdio.h>

typedef enum IO_MODE
{
    READ,
    WRITE,
    APPEND
} IO_MODE;

typedef struct IO_FILE
{
    FILE* file;
    IO_MODE mode;
} IO_FILE;

IO_FILE IO_open(const char* path, IO_MODE mode);
void IO_close(const IO_FILE file);

unsigned char* IO_read_internal(IO_FILE file, unsigned int* size, const unsigned int padding);

unsigned char* IO_read_uchar(IO_FILE file, unsigned int* size);

char* IO_read_char(IO_FILE file);

char* IO_read_char_sized(IO_FILE, unsigned int* size);

unsigned int IO_write(IO_FILE, const unsigned char* buf, const unsigned int size);

#endif // IO_H