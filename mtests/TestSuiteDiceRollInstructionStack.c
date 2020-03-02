#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"

#include "TestSuiteDiceRollInstructionStack.h"


void test_dice_roll_instruction_stack_init() {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_init();
	CU_ASSERT_PTR_NOT_NULL(dris);
	dice_roll_instruction_stack_free(dris);
}


int test_suite_dice_roll_instruction_stack(int(*init_suite)(void), int(*clean_suite)(void) ) {
	
	CU_pSuite pSuite = CU_add_suite("test_suite_dice_roll_instruction_stack", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "dice_roll_instruction_stack_init",
		test_dice_roll_instruction_stack_init)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}