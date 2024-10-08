#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DieFactory.h"
#include "TestSuiteDieFactory.h"

void test_die_factory_create_dice(void) {
	size_t num_dice = 50;
	long long int result = 0;
    long long int max_result = 0;
	int die_sides = 6;
	Die **dice = diefactory_make_die_array(die_sides, num_dice);
	
	// Make sure they are valid dice
	for(size_t i = 0; i < num_dice; ++i) {
		result = die_roll(dice[i]);
		max_result =  result > max_result ? result : max_result;
	}
	CU_ASSERT(result <= 6);
	for(size_t i = 0; i < num_dice; ++i)
	{
		die_free(dice[i]);
	}
	free(dice);
}

int test_suite_die_factory (int(*init_suite)(void), int(*clean_suite)(void) ) {
	
	const char *test_suite_name = "die_factory_test_suite";
	CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);
	
	if (NULL == CU_add_test(pSuite, "test_die_factory_create_dice", 
		test_die_factory_create_dice)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}
