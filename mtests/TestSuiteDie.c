#include <stdbool.h>
#include "CUnit/Basic.h"
#include "Die.h"

void test_roll_returns_reasonable_result() {
	Die *d = die_init(6);
	
	int result = die_roll(d);

	CU_ASSERT(result >= 1);
	CU_ASSERT(result <= 6);
	die_free(d);
}

void test_die_saves_result() {
	Die *d = die_init(6);
	
	int result = die_roll(d);
	
	CU_ASSERT(result == die_last_result(d));
}

void test_die_roll_returns_random_results() {
	size_t num_dice = 50;
	int results[6] = { 0 };
	bool have_rolled_every_number_at_least_once = true;
	
	for (size_t i = 0; i < num_dice; ++i) {
		Die *d = die_init(6);
		results[die_roll(d)-1]++;
		die_free(d);
	}
	
	for (size_t i = 0; i < 6; ++i) {
		if (results[i] == 0) {
			have_rolled_every_number_at_least_once = false;
			break;
		}
	}
	CU_ASSERT(have_rolled_every_number_at_least_once);
	
}

int test_suite_die( int(*init_suite)(void), int(*clean_suite)(void)  ) {

	CU_pSuite pSuite = CU_add_suite("die_test_suite", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_roll_returns_reasonable_result", 
		test_roll_returns_reasonable_result)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_die_saves_result", 
			test_die_saves_result)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_die_roll_returns_random_results", 
			test_die_roll_returns_random_results)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	return CU_get_error();
}