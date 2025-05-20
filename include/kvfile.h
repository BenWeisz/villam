#ifndef KVFILE_H
#define KVFILE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define KVFILE_MAX_TOKENS 1024

#define KVFILE_IS_WHITESPACE( x ) ( ( x ) == ' ' || ( x ) == '\t' || ( x ) == '\r' || ( x ) == '\n' || ( x ) == '=' || ( x ) == '\0' )

typedef struct 
{
    char** keys;
    char** values;
    unsigned int size;
} KVFILE;

KVFILE* KVFILE_read( const char* buf );
unsigned int KVFILE_tokenize( const char* buf, unsigned int* tokens, unsigned int* num_tokens );

const char* KVFILE_write( const KVFILE* kvfile );
void KVFILE_free( KVFILE* kvfile );

const char* KVFILE_get( const KVFILE* kvfile, const char* key );

#endif // KVFILE_H