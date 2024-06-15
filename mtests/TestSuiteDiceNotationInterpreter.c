#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceNotationInterpreter.h"

#include "TestSuiteDiceNotationInterpreter.h"


void test_dice_collection_from_core_notation(void) {
	DiceCollection *dc = dice_collection_from_core_notation("2d6");

	size_t count = dice_collection_count(dc);
    size_t faces = dice_collection_faces(dc);
	CU_ASSERT_EQUAL(faces, 6);
	CU_ASSERT_EQUAL(count, 2);
	
	dice_collection_free(dc);
}

void test_dice_collection_from_simple_percentile_notation(void) {
	DiceCollection *dc = dice_collection_from_percentile_notation("4%");

    size_t count = dice_collection_count(dc);
    size_t faces = dice_collection_faces(dc);
	CU_ASSERT_EQUAL(faces, 100);
	CU_ASSERT_EQUAL(count, 4);
	
	dice_collection_free(dc);
}


void test_dice_collection_from_any_notation(void) {
	DiceCollection *dc = dice_collection_create_from_notation("2d6");

    size_t count = dice_collection_count(dc);
    size_t faces = dice_collection_faces(dc);
	CU_ASSERT_EQUAL(faces, 6);
	CU_ASSERT_EQUAL(count, 2);
	
	dice_collection_free(dc);

	dc = dice_collection_create_from_notation("50%");

	count = dice_collection_count(dc);
	faces = dice_collection_faces(dc);
	CU_ASSERT_EQUAL(faces, 100);
	CU_ASSERT_EQUAL(count, 50);

	dc = dice_collection_create_from_notation("2d6!5");

	count = dice_collection_count(dc);
	faces = dice_collection_faces(dc);
    size_t explosion_lower = dice_collection_get_explosion_lower_bound(dc);
	CU_ASSERT_EQUAL(faces, 6);
	CU_ASSERT_EQUAL(count, 2);
	CU_ASSERT_EQUAL(explosion_lower, 5);

	dc = dice_collection_create_from_notation("8d10!");

	count = dice_collection_count(dc);
	faces = dice_collection_faces(dc);
	explosion_lower = dice_collection_get_explosion_lower_bound(dc);
	CU_ASSERT_EQUAL(faces, 10);
	CU_ASSERT_EQUAL(count, 8);
	CU_ASSERT_EQUAL(explosion_lower, 10);
	
	dice_collection_free(dc);
}

int test_suite_dice_notation(int(*init_suite)(void), int(*clean_suite)(void) ) {
	
	const char *test_suite_name = "test_suite_dice_notation";
	CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);

	if (NULL == CU_add_test(pSuite, "test_dice_collection_from_core_notation",
		test_dice_collection_from_core_notation)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_dice_collection_from_simple_percentile_notation",
		test_dice_collection_from_simple_percentile_notation)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_dice_collection_from_any_notation",
		test_dice_collection_from_any_notation)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}
