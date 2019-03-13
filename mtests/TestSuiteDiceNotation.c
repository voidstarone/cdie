#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceNotation.h"

#include "TestSuiteDiceNotation.h"


void test_dice_collection_from_core_notation() {
	DiceCollection *dc = dice_collection_from_core_notation("2d6");

	int count = dice_collection_count(dc);
	int faces = dice_collection_faces(dc);
	CU_ASSERT_EQUAL(faces, 6);
	CU_ASSERT_EQUAL(count, 2);
	
	dice_collection_clean(dc);
}


int test_suite_dice_notation(int(*init_suite)(void), int(*clean_suite)(void) ) {
	
	CU_pSuite pSuite = CU_add_suite("test_suite_dice_notation", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	

	if (NULL == CU_add_test(pSuite, "test_dice_collection_from_core_notation",
		test_dice_collection_from_core_notation)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}