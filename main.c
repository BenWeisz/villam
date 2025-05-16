#include <stdio.h>

#include "io.h"
#include "kvfile.h"

int main()
{
	char* data = IO_read_char("../main.c");
	if (data != NULL)
		printf("%s\n", data);


	const char* buf = " asdf \t = azxcv\r\nz = s\n\n\n\n # mama mia \t \t \r\n asd         =  dss";
	
	KVFILE* kvfile = KVFILE_read( buf );
	for ( unsigned int i = 0; i < kvfile->size; i++ )
	{
		printf( "key=%s, value=%s\n", kvfile->keys[i], kvfile->values[i] );
	}

	KVFILE_free( kvfile );

	return 0;
}
