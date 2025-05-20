#include <stdio.h>

#include "io.h"
#include "kvfile.h"

int main()
{
	char* data = IO_read_char("../main.c");
	// if (data != NULL)
	// 	printf("%s\n", data);

	free(data);

	const char* buf = " asdf \t = azxcv\r\nz = s\n\n\n\n # mama mia \t \t \r\n asd         =  dss";
	
	KVFILE* kvfile = KVFILE_read( buf );
	// for ( unsigned int i = 0; i < kvfile->size; i++ )
	// {
	// 	printf( "key=%s, value=%s\n", kvfile->keys[i], kvfile->values[i] );
	// }

	const char* a = KVFILE_get( kvfile, "asd" );
	if ( a == NULL )
	{
		printf( "NULL\n" );
	}
	else
	{
		printf("%s\n", a);
	}

	const char* output = KVFILE_write( kvfile );
	// printf( "%s", output );
	free( ( char* )output );

	KVFILE_free( kvfile );
	
	return 0;
}
