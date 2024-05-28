#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "DiceRollInstructionResultStack.h"
#include "ExpressionToDiceRollInstructionStack.h"
#include "TestSuiteDiceRollInstructionStack.h"


void test_dice_roll_instruction_stack_from_expression_simple_maths() {
	
	char exp[] = "10 + 2 * 8.2";
	printf("\n%s\n", exp);
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	CU_ASSERT_EQUAL(dyn_array_count(dris->instructions), 5);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_simple_dice() {
	
	char exp[] = "2d6";
	printf("\n%s\n", exp);
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	size_t count = dyn_array_count(dris->instructions);
	printf("count: %zu\n", count);
	CU_ASSERT_EQUAL(count, 1);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_parens_maths() {
	
	char exp[] = "(1 + ((2d6) * ((10 + 1 * 80) + 1)))";
	printf("\n%s\n", exp);
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	CU_ASSERT_EQUAL(dyn_array_count(dris->instructions), 11);
	dice_roll_instruction_stack_free(dris);
}

int test_suite_expression_to_dice_roll_instruction_stack(int(*init_suite)(void), int(*clean_suite)(void) ) {
	const char *test_suite_name = "test_suite_expression_to_dice_roll_instruction_stack";
	CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);

	// if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_simple_maths", 
    //     test_dice_roll_instruction_stack_from_expression_simple_maths)) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_simple_dice", 
        test_dice_roll_instruction_stack_from_expression_simple_dice)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}