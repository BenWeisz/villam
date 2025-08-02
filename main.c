#include <stdio.h>
#include <string.h>

#include "io.h"
#include "kvfile.h"
#include "glist.h"

int main()
{
	// IO_FILE file = IO_open("../main.c", READ);
	// char* data = IO_read_char(file);
	// // if (data != NULL)
	// // 	printf("%s\n", data);

	// free(data);

	// const char* buf = " asdf \t = azxcv\r\nz = s\n\n\n\n # mama mia \t \t \r\n asd         =  dss";
	// IO_FILE out_file = IO_open("test.key", APPEND);
	// IO_write(out_file, (unsigned char*)buf, strlen(buf));

	// KVFILE* kvfile = KVFILE_read( buf );
	// // for ( unsigned int i = 0; i < kvfile->size; i++ )
	// // {
	// // 	printf( "key=%s, value=%s\n", kvfile->keys[i], kvfile->values[i] );
	// // }

	// const char* a = KVFILE_get( kvfile, "asd" );
	// if ( a == NULL )
	// {
	// 	printf( "NULL\n" );
	// }
	// else
	// {
	// 	printf("%s\n", a);
	// }

	// const char* output = KVFILE_write( kvfile );
	// // printf( "%s", output );
	// free( ( char* )output );

	// KVFILE_free( kvfile );

	// GLIST
	GLIST* list = GLIST_create();
	GLIST_add(list, 1);
	GLIST_add(list, 2);
	GLIST_add(list, 3);
	GLIST_add(list, 4);
	GLIST_add(list, 5);
	GLIST_add(list, 6);
	GLIST_add(list, 7);
	GLIST_add(list, 8);
	GLIST_add(list, 9);
	GLIST_add(list, 10);

	GLIST_ITERATOR iter = GLIST_iterator(list);
	printf("GLIST values: ");
	for (unsigned int i = 0; i < list->size; i++)
	{
		int* temp = GLIST_next(&iter);
		printf("%d ", *temp);
	}
	printf("\n");

	GLIST_destroy(list);
	
	return 0;
}
