#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 4096

void test(char* data, size_t size){

}

int main(){
	char* data = (char*)malloc(sizeof(char) * BUF_SIZE);
	char c;
	size_t size;

	for(size = 0; (c = getchar()) != EOF; size++){
		if((size + 1) % BUF_SIZE == 0){
			printf("realloc\n");
			data = realloc(data, sizeof(char) * BUF_SIZE * (BUF_SIZE / (size + 1) + 1));
		}
		data[size] = c;
	}
	data[size] = 0x0;

	printf("Data: %s(%ld)\n", data, size);

	return 0;
}
