#include "kvfile.h"

KVFILE* KVFILE_read( const char* buf )
{
    KVFILE* kvfile = ( KVFILE* )malloc( sizeof( KVFILE ) );
    if ( kvfile == NULL )
    {
        printf( "\e[1;31mERROR\e[0m | [%s:%d] : malloc\n", __FILE__, __LINE__ );
        return NULL;
    }

    // Tokenize the buffer
    unsigned int tokens[KVFILE_MAX_TOKENS];
    unsigned int num_tokens = KVFILE_MAX_TOKENS;
    unsigned int r = KVFILE_tokenize( buf, tokens, &num_tokens );
    if ( r == 0 || num_tokens % 2 == 1 )
    {
        printf( "\e[1;31mERROR\e[0m | [%s:%d] : KVFILE_tokenize\n", __FILE__, __LINE__ );
        free( kvfile );
        return NULL;
    }

    // Allocate space for the key value pairs
    kvfile->keys = ( char** )malloc( sizeof( char* ) * num_tokens / 2 );
    if ( kvfile->keys == NULL )
    {
        printf( "\e[1;31mERROR\e[0m | [%s:%d] : malloc\n", __FILE__, __LINE__ );
        free( kvfile );
        return NULL;
    }

    kvfile->values = ( char** )malloc( sizeof( char* ) * num_tokens / 2 );
    if ( kvfile->values == NULL )
    {
        printf( "\e[1;31mERROR\e[0m | [%s:%d] : malloc\n", __FILE__, __LINE__ );
        free( kvfile->keys );
        free( kvfile );
        return NULL;
    }

    kvfile->size = num_tokens / 2;

    // Copy in the strings
    unsigned int token_i;
    for ( token_i = 0; token_i < num_tokens; token_i++ )
    {
        unsigned int token_start = tokens[token_i];
        const char* c = &( buf[token_start] );
        
        int size = 0;
        while ( !KVFILE_IS_WHITESPACE( *( c + size ) ) ) size++;
        
        char* token_buf = ( char * )malloc( sizeof( char ) * ( size + 1 ) );
        if ( token_buf == NULL )
        {
            printf( "\e[1;31mERROR\e[0m | [%s:%d] : malloc\n", __FILE__, __LINE__ );
            goto partial_cleanup;
        }

        token_buf[size--] = '\0';
        for ( ; size >= 0; size-- ) token_buf[size] = *( c + size );

        if ( token_i % 2 == 0 ) kvfile->keys[token_i / 2] = token_buf;
        else                    kvfile->values[token_i / 2] = token_buf;
    }

    return kvfile;

partial_cleanup:
    for ( unsigned int delete_token_i = 0; delete_token_i < token_i; delete_token_i++ )
    {
        if ( delete_token_i % 2 == 0 )  free( kvfile->keys[delete_token_i / 2] );
        else                            free( kvfile->values[delete_token_i / 2] );
    }
    free( kvfile->keys );
    free( kvfile->values );
    kvfile->size = 0;
    free( kvfile );
    return NULL;
}

unsigned int KVFILE_tokenize( const char* buf, unsigned int* tokens, unsigned int* num_tokens )
{
    unsigned int max_num_tokens = *num_tokens;

    *num_tokens = 0;
    unsigned int in_token = 0;

    const char* c = buf;
    while ( ( *c ) != '\0' && *num_tokens < max_num_tokens )
    {
        if ( ( *c ) == '#' )
        {
            in_token = 0;
            while ( ( *c ) != '\n' && ( *c ) != '\0' ) c++;
        }

        if ( in_token )
        {
            if ( KVFILE_IS_WHITESPACE( *c ) ) in_token = 0;
        }
        else
        {
            if ( !KVFILE_IS_WHITESPACE( *c ) )
            {
                in_token = 1;
                tokens[(*num_tokens)++] = c - buf;
            }
        }

        c++;
    }

    if ( ( *c ) != '\0' ) return 0;
    return 1;
}

const char* KVFILE_write( const KVFILE* kvfile )
{
    // Figure out how much space we'll need to store the kv file in text form
    unsigned int size = 0;
    for ( unsigned int key_i = 0; key_i < kvfile->size; key_i++ )
    {
        char* c = kvfile->keys[key_i];
        while ( ( *c ) != '\0' )
        {
            size++;
            c++; // haha
        }

        size += 2; // = and \n

        c = kvfile->values[key_i];
        while ( ( *c ) != '\0' )
        {
            size++;
            c++;
        }
    }

    size += 2; // \n\0

    // Allocate enough space for the output
    char* buf = ( char* )malloc( sizeof( char ) * size );
    if ( buf == NULL )
    {
        printf( "\e[1;31mERROR\e[0m | [%s:%d] : malloc\n", __FILE__, __LINE__ );
        return NULL;
    }

    // Copy over the data
    int pos = 0;
    for ( unsigned int key_i = 0; key_i < kvfile->size; key_i++ )
    {
        pos += sprintf( buf + pos, "%s=%s\n", kvfile->keys[key_i], kvfile->values[key_i] );
    }

    return buf;
}

void KVFILE_free( KVFILE* kvfile )
{
    for ( unsigned int key_i = 0; key_i < kvfile->size; key_i++ )
    {
        free( kvfile->keys[key_i] );
    }
    free( kvfile->keys );

    for ( unsigned int value_i = 0; value_i < kvfile->size; value_i++ )
    {
        free( kvfile->values[value_i] );
    }
    free( kvfile->values );
    
    kvfile->size = 0;
    free( ( KVFILE* )kvfile );
}