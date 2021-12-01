#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tomcrypt.h>

#define BUF_SIZE 4096

base32_alphabet testid[4] = {
	BASE32_RFC4648,
	BASE32_BASE32HEX,
	BASE32_ZBASE32,
	BASE32_CROCKFORD
};

	int
do_compare_testvector(const void* is, const unsigned long is_len, const void* should, const unsigned long should_len, const char* what, int which)
{
	if (compare_testvector(is, is_len, should, should_len, what, which) == 0) {
		return CRYPT_OK;
	} else {
		return CRYPT_FAIL_TESTVECTOR;
	}
}

void test64(char* data, int size){
	unsigned char tmp[BUF_SIZE];
	unsigned long l1, l2;
	char out[size+1024];
	
	for(unsigned long x = 0; x < 3; ++x){
		memset(out, 0, sizeof(out));
		memset(tmp, 0, sizeof(tmp));

		l1 = sizeof(out);
		if(base64_encode((unsigned char*)data, size, out, &l1) != CRYPT_OK){
			perror("64 Encode error");
			exit(1);
		}
		l2 = sizeof(tmp);
		
		/* base 64 decode */
		if(base64_decode(out, l1, tmp, &l2) != CRYPT_OK){
			perror("64 Decode error");
			exit(1);
		}
		if(do_compare_testvector(tmp, l2, data, size, "base64_decode", x) != CRYPT_OK){
			perror("64 compare error(DECODE)");
		}
	
		/* base 64 strict decode */
		if(base64_strict_decode(out, l1, tmp, &l2) != CRYPT_OK){
			perror("64 Strict Decode error");
			exit(1);
		}
		if(do_compare_testvector(tmp, l2, data, size, "base64_strict_decode", x) != CRYPT_OK){
			perror("64 compare error(STRICT)");
		}
		
		/* base 64i sane decode */
		if(base64_sane_decode(out, l1, tmp, &l2) != CRYPT_OK){
			perror("64 SANE Decode error");
			exit(1);
		}
		if(do_compare_testvector(tmp, l2, data, size, "base64_sane_decode", x) != CRYPT_OK){
			perror("64 compare error(SANE)");
		}
	}
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

	test64(data, size);

	free(data);
	return 0;
}
