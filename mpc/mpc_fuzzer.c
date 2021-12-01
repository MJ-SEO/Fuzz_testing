#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mpc.h"
#include <assert.h>

#define BUF_SIZE 4096

static int streq(const void* x, const void* y) { return (strcmp(x, y) == 0); }
static void strprint(const void* x) { printf("'%s'", (char*)x); }

void mpc_test(char* data, size_t size){
	int success;
	mpc_result_t ret;
	mpc_parser_t *p = mpc_string(data);

	success = mpc_parse("test", data, p, &ret);

//	printf("success: %d\n", success);
//	printf("data: %s\n", data);

	assert(success);
	assert(strcmp(ret.output, data) == 0);

	mpc_delete(p);
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

	mpc_test(data, size);

	return 0;
}
