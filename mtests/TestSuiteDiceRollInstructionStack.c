#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "DiceRollInstructionResultStack.h"

#include "TestSuiteDiceRollInstructionStack.h"


void test_dice_roll_instruction_stack_init() {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_init();
	CU_ASSERT_PTR_NOT_NULL(dris);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_push() {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_init();
	DiceRollInstruction *dri1 = dice_roll_instruction_from_string("max");
	dice_roll_instruction_stack_push(dris, dri1);
	DiceRollInstruction *dri2 = dice_roll_instruction_stack_peek(dris);
	int args1 = dice_roll_instruction_get_num_args(dri1);
	int args2 = dice_roll_instruction_get_num_args(dri2);
	CU_ASSERT_EQUAL(args1, args2);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_pop() {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_init();
	DiceRollInstruction *dri1 = dice_roll_instruction_from_string("max");
	dice_roll_instruction_stack_push(dris, dri1);
	DiceRollInstruction *dri2 = dice_roll_instruction_stack_pop(dris);
	int args1 = dice_roll_instruction_get_num_args(dri1);
	int args2 = dice_roll_instruction_get_num_args(dri2);
	CU_ASSERT_EQUAL(args1, args2);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_add_two_ints() {
	DiceRollInstructionResultStack *drirs = NULL;
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_init();
	DiceRollInstruction *dri_num1 = dice_roll_instruction_from_string("1");
	DiceRollInstruction *dri_num2 = dice_roll_instruction_from_string("2");
	DiceRollInstruction *dri_add = dice_roll_instruction_from_string("+");

	dice_roll_instruction_stack_push(dris, dri_add);
	dice_roll_instruction_stack_push(dris, dri_num1);
	dice_roll_instruction_stack_push(dris, dri_num2);

	dice_roll_instruction_stack_evaluate(dris, drirs);
}

int test_suite_dice_roll_instruction_stack(int(*init_suite)(void), int(*clean_suite)(void) ) {
	
	char ** test_suite_name = "test_suite_dice_roll_instruction_stack";
	CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	printf("%s\n", test_suite_name);

	if (NULL == CU_add_test(pSuite, "dice_roll_instruction_stack_init",
		test_dice_roll_instruction_stack_init)) {
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