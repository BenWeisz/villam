#include "io.h"

IO_FILE IO_open(const char* path, IO_MODE mode)
{
	IO_FILE file;
	file.mode = mode;

	char *read_mode[3] = {
		"rb",
		"wb",
		"ab"
	};

	char *error_msg[3] = {
		"ERROR: Failed to open file for reading: %s\n",
		"ERROR: Failed to open file for writing: %s\n",
		"ERROR: Failed to open file for appending: %s\n"
	};

	file.file = fopen(path, read_mode[(int)mode]);
	if (file.file == NULL)
	{
		printf(error_msg[(int)mode], path);
	}

	return file;
}

void IO_close(const IO_FILE file)
{
	fclose(file.file);
}

unsigned char* IO_read_internal(IO_FILE file, unsigned int* size, const unsigned int padding)
{
	if (file.file == NULL)
	{
		printf("ERROR: Attempting to read from file that is closed\n");
		*size = 0;
		return NULL;
	}

	// Jump to the end of the file and read the file size
	int r = fseek(file.file, 0, SEEK_END);
	if (r == -1)
	{
		printf("ERROR: Failed to seek to the end of the file\n");
		*size = 0;
		return NULL;
	}

	long file_size = ftell(file.file);
	if (file_size == 0)
	{
		// This file is empty, not necessarily an error
		*size = 0;
		return NULL;
	}

	// Jump back to the start of the file to read the file data
	r = fseek(file.file, 0, SEEK_SET);
	if (r == -1)
	{
		printf("ERROR: Failed to seek to the start of the file\n");
		*size = 0;
		return NULL;
	}

	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * (file_size + padding));
	if (data == NULL)
	{
		printf("ERROR: Failed to allocate space for data\n");
		*size = 0;
		return NULL;
	}

	size_t bytes_read = fread(data, sizeof(unsigned char), file_size, file.file);
	if (bytes_read != file_size)
	{
		printf("ERROR: Failed to read all of the file contents\n");
		*size = 0;
		free(data);
		return NULL;
	}

	*size = bytes_read;

	return data;
}

unsigned char* IO_read_uchar(IO_FILE file, unsigned int* size)
{
	return IO_read_internal(file, size, 0);
}

char* IO_read_char(IO_FILE file)
{
	unsigned int size;
	char* data = IO_read_char_sized(file, &size);
	return data;
}

char* IO_read_char_sized(IO_FILE file, unsigned int* size)
{
	char* data = (char*)IO_read_internal(file, size, 1);
	data[*size] = '\0';
	return data;
}

unsigned int IO_write(IO_FILE file, const unsigned char* buf, const unsigned int size)
{
	if (file.mode == READ)
	{
		printf("ERROR: Attempting to write to file open for reading\n");
		return 0;
	}

    if (file.file == NULL)
    {
        printf("ERROR: Attempting to write to file that is closed\n");
        return 0;
    }

    size_t bytes_written = fwrite(buf, sizeof(unsigned char), size, file.file);
    if (bytes_written != size)
    {
        printf("ERROR: Failed to write all the bytes\n");
    }

    return bytes_written;
}
