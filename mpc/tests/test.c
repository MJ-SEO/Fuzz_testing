#include "ptest.h"
#include "../mpc.h"

void suite_core(void);
// void suite_regex(void);
// void suite_grammar(void);
// void suite_combinators(void);

void suite_core(void){
	pt_add_test(test_ident,  "Test Ident",  "Suite Core");
	pt_add_test(test_maths,  "Test Maths",  "Suite Core");
	pt_add_test(test_strip,  "Test Strip",  "Suite Core");
	pt_add_test(test_repeat, "Test Repeat", "Suite Core");
	pt_add_test(test_copy,   "Test Copy",   "Suite Core");
	pt_add_test(test_reader, "Test Reader", "Suite Core");
	pt_add_test(test_tokens, "Test Tokens", "Suite Core");
	pt_add_test(test_eoi,    "Test EOI",    "Suite Core");
}

int main(int argc, char** argv) {
	// (void) argc; (void) argv;
	pt_add_suite(suite_core);
	//  pt_add_suite(suite_regex);
	//  pt_add_suite(suite_grammar);
	// pt_add_suite(suite_combinators);

	return pt_run();
}

