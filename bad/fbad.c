#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

/* usage
 *	afl-gcc bad.c -i bad
 *	afl-fuzz -i [input_dir] -o [output_dir] ./bad
 * */
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

char* inp = NULL;
size_t data_size;

int main(int argc, char * argv[]){

	inp = read_data_from_file(argv[1], &data_size);
	printf("buf: %s, size: %ld\n", inp, data_size);

	if(strlen(inp) > 0 && inp[0] == 'b'){
		if(strlen(inp) > 1 && inp[1] == 'a'){
			if(strlen(inp) > 2 && inp[2] == 'd'){
				if(strlen(inp) > 3 && inp[3] == '!'){
					assert(0);
				}
			}
		}
	}

	return 0;
}

