#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include <stdint.h>
#include <elf.h>

#define BUF_SIZE 4096

char* read_data_from_file(const char *path, size_t *ptr_data_size)
{
	FILE *file_desc;
	int result = -1;
	char *buffer = NULL;

	if (path == NULL)
		return NULL;

	struct stat stat_str;
	result = stat(path, &stat_str);
	if (result != 0)
		return NULL;		

	file_desc = fopen(path, "r");
	if (file_desc == NULL)
		return NULL;
	size_t data_size = stat_str.st_size;

	buffer = malloc(data_size+1);
	if (buffer == NULL)
		return NULL;

	buffer[data_size] = 0;

	size_t data_size_read = fread(buffer, 1, data_size, file_desc);

	assert(data_size_read == data_size);
	*ptr_data_size = data_size;

#ifdef DISPLAY_INPUT_HEX
	for (int i=0; i<data_size+1; ++i)
		printf("%02hhX ", buffer[i] & 0xff);
	printf("\n");
#endif // #ifdef DISPLAY_INPUT_HEX

#ifdef DISPLAY_INPUT_PLAIN
	printf("-----------------------------\n%s\n-------------------------\n", buffer);
#endif // #ifdef DISPLAY_INPUT_PLAIN

	fclose(file_desc);
	return buffer;
}

size_t data_size;
char* data = NULL;

int main(int argc, char* argv[]){
	Fhdr fhdr;
	FILE * f;
	uint8_t *buf;
	uint64_t len;
	
	data = read_data_from_file(argv[1], &data_size);
	
	f = fmemopen(data, data_size, "rb");
	if (f == NULL){
		perror("F error!");
		return -1;
	}

	readelf(f, &fhdr);

	buf = readelfsection(f, ".text", &len, &fhdr);
	if(buf == NULL){
		perror("buf NULL");
		return -1;
	}

	printelfhdr(&fhdr);	

	freeelf(&fhdr);
	return 0;
}
