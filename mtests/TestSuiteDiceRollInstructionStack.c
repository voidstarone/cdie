#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "DiceRollInstructionResultStack.h"

#include "TestSuiteDiceRollInstructionStack.h"


void test_dice_roll_instruction_stack_create() {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create();
	CU_ASSERT_PTR_NOT_NULL(dris);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_push() {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create();
	DiceRollInstruction *dri1 = dice_roll_instruction_from_string("max");
	dice_roll_instruction_stack_push(dris, dri1);
	DiceRollInstruction *dri2 = dice_roll_instruction_stack_peek(dris);
	int args1 = dice_roll_instruction_get_num_args(dri1);
	int args2 = dice_roll_instruction_get_num_args(dri2);
	CU_ASSERT_EQUAL(args1, args2);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_pop() {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create();
	DiceRollInstruction *dri1 = dice_roll_instruction_from_string("max");
	dice_roll_instruction_stack_push(dris, dri1);
	DiceRollInstruction *dri2 = dice_roll_instruction_stack_pop(dris);
	int args1 = dice_roll_instruction_get_num_args(dri1);
	int args2 = dice_roll_instruction_get_num_args(dri2);
	CU_ASSERT_EQUAL(args1, args2);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_add_two_ints() {
	DiceRollInstructionResultStack *drirs = dice_roll_instruction_result_stack_create();
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create();
	DiceRollInstruction *dri_num1 = dice_roll_instruction_from_string("1");
	DiceRollInstruction *dri_num2 = dice_roll_instruction_from_string("2");
	DiceRollInstruction *dri_add = dice_roll_instruction_from_string("+");

	dice_roll_instruction_stack_push(dris, dri_add);
	dice_roll_instruction_stack_push(dris, dri_num1);
	dice_roll_instruction_stack_push(dris, dri_num2);

	dice_roll_instruction_stack_evaluate(dris, drirs);

	DiceRollInstructionResult *drir = dice_roll_instruction_result_stack_pop(drirs);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 3.0);
	dice_roll_instruction_stack_free(dris);
	dice_roll_instruction_result_stack_free(drirs);
}

void test_dice_roll_instruction_stack_evaluate_max_dice_collection() {
	DiceRollInstructionResultStack *drirs = dice_roll_instruction_result_stack_create();
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create();
	DiceRollInstruction *max = dice_roll_instruction_from_string("max");
	DiceRollInstruction *dc = dice_roll_instruction_from_string("2d6");
	size_t count = dice_collection_count(dc);
	printf("count: %d\n", count);
	int results[] = {1, 5};
	dice_collection_set_results(dc->value, results);
	dice_roll_instruction_stack_push(dris, max);
	dice_roll_instruction_stack_push(dris, dc);

	dice_roll_instruction_stack_evaluate(dris, drirs);

	DiceRollInstructionResult *drir = dice_roll_instruction_result_stack_pop(drirs);

	double real_result = dice_roll_instruction_result_get_number(drir);
	printf("real_result: %d\n", real_result);
	CU_ASSERT_EQUAL(real_result, (double) 5.0);
	dice_roll_instruction_stack_free(dris);
	dice_roll_instruction_result_stack_free(drirs);
}


int test_suite_dice_roll_instruction_stack(int(*init_suite)(void), int(*clean_suite)(void) ) {
	const char *test_suite_name = "test_suite_dice_roll_instruction_stack";
	CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);

	if (NULL == CU_add_test(pSuite, "dice_roll_instruction_stack_create",
		test_dice_roll_instruction_stack_create)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_push",
		test_dice_roll_instruction_stack_push)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_pop",
		test_dice_roll_instruction_stack_pop)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_add_two_ints",
		test_dice_roll_instruction_stack_evaluate_add_two_ints)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_max_dice_collection",
		test_dice_roll_instruction_stack_evaluate_max_dice_collection)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}

/*
2d6 + 4d20 =>
DRIS[DRI(+), DRI(2d6), DRI(4d20)] => 
int
--
2d6 + 4d20 1d100 =>
DRIS[DRI(+), DRI(2d6), DRI(4d20), DRI(1d100)] => 
int, int
--
2d6 + 4d20 2d100 =>
DRIS[DRI(+), DRI(2d6), DRI(4d20), DRI(2d100)] => 
int, int, int
--
1 + 4d20 =>
DRIS[DRI(+), DRI(1), DRI(4d20)] => 
int
--
2d6 - 4d20 =>
DRIS[DRI(-), DRI(2d6), DRI(4d20)] => 
int
--
sum(2d6, 4d20, 1d4)
DRIS[DRI("sum"), DRI(2d6), DRI(4d20), DRI(1d4)] =>
int
--
mean(2d6, 4d20, 1d4)
DRIS[DRI("mean"), DRI(2d6), DRI(4d20), DRI(1d4)] =>
int
--
20 + sum(2d6, 4d20, 1d4)
DRIS[DRI(+), DRI(20), DRI("sum"), DRI(2d6), DRI(4d20), DRI(1d4)] =>
int
*/