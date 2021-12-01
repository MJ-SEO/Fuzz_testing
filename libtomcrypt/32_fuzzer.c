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

void test32_rc4(char* data, size_t size){
	printf("[RC4] \n");
	int err = CRYPT_NOP;
	prng_state prng;
	
	unsigned char buf[BUF_SIZE];
	unsigned char tmp[BUF_SIZE];
	
	unsigned long l1, l2;
	char out[size+1024];

	if ((err = rc4_start(&prng)) != CRYPT_OK) {
		printf("RC4 init error: %s\n", error_to_string(err));
		exit(1);
	}
	/* use "key" as the key */
	if ((err = rc4_add_entropy(data, size, &prng)) != CRYPT_OK) {
		printf("RC4 add entropy error: %s\n", error_to_string(err));
		exit(1);
	}
	/* setup RC4 for use */
	if ((err = rc4_ready(&prng)) != CRYPT_OK) {
		printf("RC4 ready error: %s\n", error_to_string(err));
		exit(1);
	}
	/* encrypt buffer */
	strcpy(buf, data);
	if (rc4_read(buf, size, &prng) != size) {
		printf("RC4 read error\n");
		exit(1);
	}
	
	for(int idx = 0; idx < 4; idx++){
		for(unsigned long x = 0; x < size; x++){
			rc4_read(buf, x, &prng);
			
			l1 = sizeof(out);
			if(base32_encode(buf, x, out, &l1, testid[idx]) != CRYPT_OK){
				perror("RC4 encode error");	
				exit(1);
			}
//			printf("[ENCODE] buf: %s out: %s ||| l1: %ld\n", buf, out, l1);

			l2 = sizeof(l2);
			if(base32_decode(out, l1, tmp, &l2, testid[idx]) != CRYPT_OK){
				perror("RC4 decode error");
				exit(1);
			}
//			printf("[DECODE] buf: %s out: %s ||| l1: %ld l2:%ld\n", buf, out, l1, l2);

			if(do_compare_testvector(tmp, l2, buf, x, "random base32", idx * 100 + x) != CRYPT_OK){
				perror("RC4 compare error");	
				exit(1);
			}
		}
	}
}

void test32_yarrow(char* data, size_t size){
	printf("[YARROW] \n");
	int err = CRYPT_NOP;
	prng_state prng;
	
	unsigned char buf[BUF_SIZE];
	unsigned char tmp[BUF_SIZE];

	unsigned long l1, l2;
	char out[size+1024];
	
	/* start it */
	if ((err = yarrow_start(&prng)) != CRYPT_OK) {
		printf("Start error: %s\n", error_to_string(err));
		exit(1);
	}
	/* add entropy */
	if ((err = yarrow_add_entropy(data, size, &prng)) != CRYPT_OK) {
		printf("Add_entropy error: %s\n", error_to_string(err));
		exit(1);
	}
	/* ready and read */
	if ((err = yarrow_ready(&prng)) != CRYPT_OK) {
		printf("Ready error: %s\n", error_to_string(err));
		exit(1);
	}
	if (yarrow_read(buf, size, &prng) != size){
		printf("yarrow read error\n");
		exit(1);
	}

	for(int idx = 0; idx < 4; idx++){
		for(unsigned long x = 0; x < size; x++){
			yarrow_read(buf, x, &prng);
			
			l1 = sizeof(out);
			if(base32_encode(buf, x, out, &l1, testid[idx]) != CRYPT_OK){
				perror("YARROW encode error");	
				exit(1);
			}
//			printf("[ENCODE] buf: %s out: %s ||| l1: %ld\n", buf, out, l1);

			l2 = sizeof(l2);
			if(base32_decode(out, l1, tmp, &l2, testid[idx]) != CRYPT_OK){
				perror("YARROW decode error");
				exit(1);
			}
//			printf("[DECODE] buf: %s out: %s ||| l1: %ld l2:%ld\n", buf, out, l1, l2);

			if(do_compare_testvector(tmp, l2, buf, x, "random base32", idx * 100 + x) != CRYPT_OK){
				perror("YARROW compare error");	
				exit(1);
			}
		}
	}
}

void test32(char* data, int size){
	unsigned char tmp[BUF_SIZE];
	unsigned long l1, l2;
	char out[size+1024];
	printf("[TEST32] \n");

	for(int idx = 0; idx < 4; idx++){
		l1 = sizeof(out);
		if(base32_encode(data, size, out, &l1, testid[idx]) != CRYPT_OK){
			perror("TEST32 encode error");
			exit(1);	
		};
//		printf("[ENCODE] data: %s out: %s ||| l1: %ld\n", data, out, l1);

		l2 = sizeof(tmp);
		if(base32_decode(out, l1, tmp, &l2, testid[idx]) != CRYPT_OK){
			perror("TEST32 decode error");
			exit(1);
		}
//		printf("[DECODE] data: %s out: %s ||| l1: %ld l2:%ld\n", data, out, l1, l2);

		if(do_compare_testvector(tmp, l2, data, size, "testin base32", idx) != CRYPT_OK){
			perror("TEST32 compare error");
			exit(1);
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

//	printf("Data: %s(%ld)\n", data, size);

	test32(data, size);
	test32_yarrow(data, size);
	test32_rc4(data, size);

	return 0;
}
