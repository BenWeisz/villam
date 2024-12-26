#include <stdio.h>

#include "io.h"

int main()
{
	char* data = IO_read_char("../main.c");
	if (data != NULL)
		printf("%s\n", data);
	return 0;
}
