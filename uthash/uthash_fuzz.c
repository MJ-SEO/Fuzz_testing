#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "include/utstring.h"
#include "include/uthash.h"

#define BUF_SIZE 128

struct my_struct {
	int id;
	char name[BUF_SIZE];
	UT_hash_handle hh;
};

struct my_struct *tests = NULL;

void test_utstring(char* data, size_t size){
	UT_string* str;
	utstring_new(str);

	utstring_printf(str, data);
	printf("size: %ld\n", size);
	printf("len: %u\n", (unsigned)utstring_len(str));
	assert(size == (unsigned)utstring_len(str));
}

void add_user(int user_id, const char *name){
	printf("ADD\n");
	struct my_struct *s;
	HASH_FIND_INT(tests, &user_id, s);
	if(s == NULL){
		s = (struct my_struct*)malloc(sizeof *s);
		s->id = user_id;
		HASH_ADD_INT(tests, id, s);
	}
	strcpy(s->name, name);
}

int by_name(const struct my_struct *a, const struct my_struct *b){
	return strcmp(a->name, b->name);
}

int by_id(const struct my_struct *a, const struct my_struct *b){
	return (a->id - b->id);
}

void delete_users(){
	struct my_struct *current_user;
	struct my_struct *tmp;
	HASH_ITER(hh, tests, current_user, tmp){
		HASH_DEL(tests, current_user);
		free(current_user);
	}
}

void test_uthash(char* data, size_t size){
	int id = 1;
	int temp;

	for(int i=0; i<100; i++){
		int op = rand()%9+1;
		switch (op){
			case 1:
			case 2:
			case 3:
			case 4:
				add_user(id++, data);
				break;
			case 5:
				HASH_SORT(tests, by_name);	
				break;
			case 6:
				HASH_SORT(tests, by_id);
				break;
			case 7:
				temp = HASH_COUNT(tests);
				break;
			case 8:
				break;
			case 9:
				delete_users();
				break;
		}
	}
}

int main(){
	srand((unsigned int)time(NULL));
	char data[BUF_SIZE];
	fgets(data, BUF_SIZE, stdin);
	size_t size = strlen(data);
	//	printf("Data: %s(%ld)\n", data, size);

	test_utstring(data, size);

	test_uthash(data, size);
	return 0;
}
