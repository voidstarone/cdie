#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"

#include "TestSuiteDiceRollInstructionResult.h"


void test_dice_roll_instruction_result_init() {
	DiceRollInstructionResult *drir = dice_roll_instruction_result_init();
	CU_ASSERT_PTR_NOT_NULL(drir);
	dice_roll_instruction_result_free(drir);
}

int test_suite_dice_roll_instruction_result(int(*init_suite)(void), int(*clean_suite)(void) ) {
	char **test_suite_name = "test_suite_dice_roll_instruction_result";
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


	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}