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


void test_dice_roll_instruction_stack_from_expression_subtract(void) {
	
	char exp[] = "10 - 2";
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	CU_ASSERT_EQUAL(dyn_array_count(dris->instructions), 3);
	DiceRollInstruction *instruction1 = dyn_array_element_at_index(dris->instructions, 2);
	CU_ASSERT_EQUAL(instruction1->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction1), 10);
	DiceRollInstruction *instruction2 = dyn_array_element_at_index(dris->instructions, 1);
	CU_ASSERT_EQUAL(instruction2->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction2), 2);
	DiceRollInstruction *instruction3 = dyn_array_element_at_index(dris->instructions, 0);
	CU_ASSERT_EQUAL(instruction3->operation_type, op_type_subtract);
	
	DiceRollInstructionResult* result = dice_roll_instruction_stack_evaluate(dris);
	CU_ASSERT_EQUAL(dice_roll_instruction_result_get_number(result), 8);

	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_divide(void) {
	
	char exp[] = "10 / 2";
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	CU_ASSERT_EQUAL(dyn_array_count(dris->instructions), 3);
	DiceRollInstruction *instruction1 = dyn_array_element_at_index(dris->instructions, 2);
	CU_ASSERT_EQUAL(instruction1->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction1), 10);
	DiceRollInstruction *instruction2 = dyn_array_element_at_index(dris->instructions, 1);
	CU_ASSERT_EQUAL(instruction2->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction2), 2);
	DiceRollInstruction *instruction3 = dyn_array_element_at_index(dris->instructions, 0);
	CU_ASSERT_EQUAL(instruction3->operation_type, op_type_divide);
	
	DiceRollInstructionResult* result = dice_roll_instruction_stack_evaluate(dris);
	CU_ASSERT_EQUAL(dice_roll_instruction_result_get_number(result), 5.0);

	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_add_multiply(void) {
	
	char exp[] = "10 + 2 * 8.2";
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	CU_ASSERT_EQUAL(dyn_array_count(dris->instructions), 5);
	DiceRollInstruction *instruction1 = dyn_array_element_at_index(dris->instructions, 4);
	CU_ASSERT_EQUAL(instruction1->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction1), 2);
	DiceRollInstruction *instruction2 = dyn_array_element_at_index(dris->instructions, 3);
	CU_ASSERT_EQUAL(instruction2->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction2), 8.2);
	DiceRollInstruction *instruction3 = dyn_array_element_at_index(dris->instructions, 2);
	CU_ASSERT_EQUAL(instruction3->operation_type, op_type_multiply);
	DiceRollInstruction *instruction4 = dyn_array_element_at_index(dris->instructions, 1);
	CU_ASSERT_EQUAL(instruction4->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction4), 10);
	DiceRollInstruction *instruction5 = dyn_array_element_at_index(dris->instructions, 0);
	CU_ASSERT_EQUAL(instruction5->operation_type, op_type_add);

	DiceRollInstructionResult* result = dice_roll_instruction_stack_evaluate(dris);
	double result_num = dice_roll_instruction_result_get_number(result);
	CU_ASSERT_EQUAL(result_num, 26.4);

	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_subtract_multiply(void) {
	
	char exp[] = "10 - 2 * 8";
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	CU_ASSERT_EQUAL(dyn_array_count(dris->instructions), 5);
	DiceRollInstruction *instruction1 = dyn_array_element_at_index(dris->instructions, 4);
	CU_ASSERT_EQUAL(instruction1->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction1), 2);
	DiceRollInstruction *instruction2 = dyn_array_element_at_index(dris->instructions, 3);
	CU_ASSERT_EQUAL(instruction2->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction2), 8);
	DiceRollInstruction *instruction3 = dyn_array_element_at_index(dris->instructions, 2);
	CU_ASSERT_EQUAL(instruction3->operation_type, op_type_multiply);
	DiceRollInstruction *instruction4 = dyn_array_element_at_index(dris->instructions, 1);
	CU_ASSERT_EQUAL(instruction4->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction4), 10);
	DiceRollInstruction *instruction5 = dyn_array_element_at_index(dris->instructions, 0);
	CU_ASSERT_EQUAL(instruction5->operation_type, op_type_subtract);

	DiceRollInstructionResult* result = dice_roll_instruction_stack_evaluate(dris);
	double result_num = dice_roll_instruction_result_get_number(result);
	CU_ASSERT_EQUAL(result_num, -6);

	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_simple_dice(void) {
	
	char exp[] = "2d6";
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	size_t count = dyn_array_count(dris->instructions);
	CU_ASSERT_EQUAL(count, 1);
	DiceRollInstruction *instruction1 = dyn_array_element_at_index(dris->instructions, 0);
	CU_ASSERT_EQUAL(instruction1->operation_type, op_type_dice_collection);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_simple_dice_add_expression(void) {
	
	char exp[] = "1d1 + 2";
	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_from_expression(exp);

	size_t count = dyn_array_count(dris->instructions);
	CU_ASSERT_EQUAL(count, 3);
	DiceRollInstruction *instruction1 = dyn_array_element_at_index(dris->instructions, 2);
	CU_ASSERT_EQUAL(instruction1->operation_type, op_type_dice_collection);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction1), 1);
	DiceRollInstruction *instruction2 = dyn_array_element_at_index(dris->instructions, 1);
	CU_ASSERT_EQUAL(instruction2->operation_type, op_type_number);
	CU_ASSERT_EQUAL(dice_roll_instruction_get_number(instruction2), 2);
	DiceRollInstruction *instruction3 = dyn_array_element_at_index(dris->instructions, 0);
	CU_ASSERT_EQUAL(instruction3->operation_type, op_type_add);

	DiceRollInstructionResult* result = dice_roll_instruction_stack_evaluate(dris);

	CU_ASSERT_EQUAL(dice_roll_instruction_result_get_number(result), 3.0);

	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_from_expression_parens_maths(void) {
	
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

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_subtract", 
        test_dice_roll_instruction_stack_from_expression_subtract)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_divide", 
        test_dice_roll_instruction_stack_from_expression_divide)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_add_multiply", 
        test_dice_roll_instruction_stack_from_expression_add_multiply)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_subtract_multiply", 
        test_dice_roll_instruction_stack_from_expression_subtract_multiply)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_simple_dice", 
        test_dice_roll_instruction_stack_from_expression_simple_dice)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_from_expression_simple_dice_add_expression", 
        test_dice_roll_instruction_stack_from_expression_simple_dice_add_expression)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}
