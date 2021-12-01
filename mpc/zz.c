#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "mpc.h"

void test(char* Data, size_t Size){
	char *new_str = (char *)malloc(Size+1);
	if (new_str == NULL){
		perror("Null string");
		exit(1);
	}
	memcpy(new_str, Data, Size);
	new_str[Size] = '\0';

	for(int i=0;i<3;i++){
		mpc_parser_t *re0 = mpc_re_mode(new_str, i);
		mpc_delete(re0);
	}

	free(new_str);
}

int main(){
	char Data[4096];
	size_t Size = 0;
	Size = read(0, Data, 4096);
	
	test(Data, Size); 
	return 0;
}
