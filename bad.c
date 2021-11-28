#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

/* usage
 *	afl-gcc bad.c -i bad
 *	afl-fuzz -i [input_dir] -o [output_dir] ./bad
 * */

void my_echo(char* data){
	char buf[10];
	strcpy(buf, data);
	printf("%s\n", buf);
	if(data[0] == 'b')
		if(data[1] == 'a')
			if(data[2] == 'd')
				if(data[3] == '!')
					assert(0);
}

int main(){
	char inp[50];
	read(0, inp, 50);
	my_echo(inp);
}
