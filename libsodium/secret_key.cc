#include <assert.h>
#include <stdlib.h>
#include <sodium.h>
#include <unistd.h>

#include "fake_random.h"

void sodium_driver(unsigned char* data, ssize_t size){
	int initialized = sodium_init();
	assert(initialized >= 0);

	setup_fake_random(data, size);
	
	unsigned char key[crypto_auth_KEYBYTES];
	unsigned char mac[crypto_auth_BYTES];

	crypto_auth_keygen(key);
	crypto_auth(mac, data, size, key);
	int err = crypto_auth_verify(mac, data, size, key);
	assert(err == 0);
}

int main(void){
	unsigned char data[8196];
	ssize_t size = 0;
	size = read(0, data, 8196);
	
	sodium_driver(data, size);

	return 0;
}
