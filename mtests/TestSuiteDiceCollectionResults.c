#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollectionResults.h"

#include "TestSuiteDiceCollectionResults.h"


void test_dice_collection_results_init(void) {
	DiceCollectionResults *dcr = dice_collection_results_create(10);
	
	size_t count = dice_collection_results_count(dcr);
	
	CU_ASSERT_EQUAL(count, 0);
	
	dice_collection_results_free(dcr);
}


void test_dice_collection_results_add_count(void) {
	DiceCollectionResults *dcr = dice_collection_results_create(10);
	size_t count = dice_collection_results_count(dcr);
	
	CU_ASSERT_EQUAL(count, 0);
	dice_collection_results_add(dcr, 4);
	
	count = dice_collection_results_count(dcr);
	CU_ASSERT_EQUAL(count, 1);
	
	dice_collection_results_free(dcr);
}

void test_dice_collection_results_get_array(void) {
	DiceCollectionResults *dcr = dice_collection_results_create(10);
	dice_collection_results_add(dcr, 4);
	dice_collection_results_add(dcr, 10);
	dice_collection_results_add(dcr, 20);
	
	size_t results_array[10] = {0};
	dice_collection_results_as_array(dcr, results_array);
		
	CU_ASSERT_EQUAL(results_array[0], 4);
	CU_ASSERT_EQUAL(results_array[1], 10);
	CU_ASSERT_EQUAL(results_array[2], 20);
	
	dice_collection_results_free(dcr);
}

void test_dice_collection_results_result_at(void) {
	DiceCollectionResults *dcr = dice_collection_results_create(10);

	dice_collection_results_add(dcr, 90);

	long long int value = dice_collection_results_result_at(dcr, 0);

	CU_ASSERT_EQUAL(value, 90);

	dice_collection_results_free(dcr);
}

int test_suite_dice_collection_results(int(*init_suite)(void),                                                         int(*clean_suite)(void) ) {
	const char *test_suite_name = "test_suite_dice_collection_results";

	CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);

	if (NULL == CU_add_test(pSuite, "test_dice_collection_results_init",
		test_dice_collection_results_init)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_dice_collection_results_add_count",
		test_dice_collection_results_add_count)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_dice_collection_results_get_array",
		test_dice_collection_results_get_array)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_collection_results_result_at",
		test_dice_collection_results_result_at)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}
