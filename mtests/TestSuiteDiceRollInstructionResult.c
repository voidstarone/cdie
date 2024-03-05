#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"

#include "TestSuiteDiceRollInstructionResult.h"


void test_dice_roll_instruction_result_init() {
	DiceRollInstructionResult *drir = dice_roll_instruction_result_create();
	CU_ASSERT_PTR_NOT_NULL(drir);
	dice_roll_instruction_result_free(drir);
}

void test_dice_roll_instruction_result_with_double() {
	double intended_result_number = 9.0;
	DiceRollInstructionResult *drir = dice_roll_instruction_result_with_double(intended_result_number);
	CU_ASSERT_PTR_NOT_NULL(drir);
	double result_number = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(intended_result_number, result_number);
	dice_roll_instruction_result_free(drir);
}

int test_suite_dice_roll_instruction_result(int(*init_suite)(void), int(*clean_suite)(void) ) {
	const char *test_suite_name = "test_suite_dice_roll_instruction_result";
	CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_result_init",
		test_dice_roll_instruction_result_init)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_result_with_double",
		test_dice_roll_instruction_result_with_double)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}