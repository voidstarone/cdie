#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "TestSuiteDiceCollection.h"

void test_dice_collection_init_faces() {
	DiceCollection *dc = dice_collection_init(10, 20);
	
	CU_ASSERT(dc->num_faces == 10);
}

void test_dice_collection_init_count() {
	DiceCollection *dc = dice_collection_init(20, 7);
	
	CU_ASSERT(dice_collection_count(dc) == 7);
}

void test_dice_collection_die_at() {
	DiceCollection *dc = dice_collection_init(20, 7);
	
	Die *d = dice_collection_die_at(dc, 2);
	
	int last_result = die_last_result(d);
	CU_ASSERT(last_result == -1);
}

void test_dice_collection_roll() {
	DiceCollection *dc = dice_collection_init(6, 50);
	int rolls[50];
	bool have_rolled_every_number_at_least_once = true;
	int results[6] = { 0, 0, 0, 0, 0, 0 };
	dice_collection_roll(dc, results);
	
	for (size_t i = 0; i < 50; ++i) {
		results[roll[i]]++;
	}
	for (size_t i = 0; i < 6; ++i) {
		if (results[i] == 0) {
			have_rolled_every_number_at_least_once = false;
			break;
		}
	}
	CU_ASSERT(have_rolled_every_number_at_least_once)
}


int test_suite_dice_collection (int(*init_suite)(void), int(*clean_suite)(void) ) {
	
	CU_pSuite pSuite = CU_add_suite("test_suite_dice_collection", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
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
	
	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}