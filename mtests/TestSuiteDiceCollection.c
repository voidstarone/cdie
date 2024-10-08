#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"

#include "TestSuiteDiceCollection.h"

void test_dice_collection_init_faces(void) {
	DiceCollection *dc = dice_collection_create(10, 20);
	
	CU_ASSERT_EQUAL(dice_collection_faces(dc), 20);
	
	dice_collection_free(dc);
}

void test_dice_collection_init_count(void) {
	DiceCollection *dc = dice_collection_create(20, 7);

	CU_ASSERT_EQUAL(dice_collection_count(dc), 20);
	dice_collection_free(dc);
}

void test_dice_collection_die_at(void) {
	DiceCollection *dc = dice_collection_create(20, 7);

	Die *d = dice_collection_die_at(dc, 2);

	CU_ASSERT_PTR_NOT_NULL(d);
	dice_collection_free(dc);
}

void test_dice_collection_roll(void) {
	DiceCollection *dc = dice_collection_create(50, 6);
	DiceCollectionResults *dcr = dice_collection_results_create(50);
	size_t rolls[50] = { 0 } ;
	bool have_rolled_every_number_at_least_once = true;
	size_t results[6] = { 0, 0, 0, 0, 0, 0 };
	
	dice_collection_roll(dc, dcr);
	dice_collection_results_as_array(dcr, rolls);

	for (size_t i = 0; i < 50; ++i) {
		results[rolls[i]-1]++;
	}
	for (size_t i = 0; i < 6; ++i) {
		if (results[i] == 0) {
			have_rolled_every_number_at_least_once = false;
			break;
		}
	}
	CU_ASSERT(have_rolled_every_number_at_least_once);
	dice_collection_results_free(dcr);
	dice_collection_free(dc);
}

void test_dice_collection_roll_explode(void) {
	DiceCollection *dc = dice_collection_create(50, 6);
	 
	dice_collection_set_explosion_lower_bound(dc, 6);
	
	CU_ASSERT_EQUAL(dice_collection_get_explosion_lower_bound(dc), 6);

	dice_collection_roll_silent(dc);

	DiceCollectionResults *dcr = dice_collection_last_results(dc);
	size_t num_results = dice_collection_results_count(dcr);

	CU_ASSERT(num_results > 50);
	dice_collection_free(dc);
}


int test_suite_dice_collection (int(*init_suite)(void),
                                int(*clean_suite)(void) ) {
	const char *test_suite_name = "test_suite_dice_collection";
	CU_pSuite pSuite = CU_add_suite(test_suite_name, 
		init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);

	
	if (NULL == CU_add_test(pSuite, "test_dice_collection_init_faces",
		test_dice_collection_init_faces)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_collection_init_count",
		test_dice_collection_init_count)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_collection_die_at",
		test_dice_collection_die_at)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_collection_roll",
		test_dice_collection_roll)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	

	if (NULL == CU_add_test(pSuite, "test_dice_collection_roll_explode",
		test_dice_collection_roll_explode)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}
