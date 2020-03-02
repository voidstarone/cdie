#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"

#include "TestSuiteDiceRollInstruction.h"


void test_dice_roll_instruction_init() {
	DiceRollInstruction *dri = dice_roll_instruction_init();
	CU_ASSERT_PTR_NOT_NULL(dri);
	dice_roll_instruction_free(dri);
}

void test_dice_roll_instruction_from_string_with_plus() {
	DiceRollInstruction *dri = dice_roll_instruction_from_string("+");
	CU_ASSERT_PTR_NOT_NULL(dri);
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	CU_ASSERT_EQUAL(op_type, op_type_add);
	dice_roll_instruction_free(dri);
}

void test_dice_roll_instruction_from_string_with_minus() {
	DiceRollInstruction *dri = dice_roll_instruction_from_string("-");
	CU_ASSERT_PTR_NOT_NULL(dri);
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	CU_ASSERT_EQUAL(op_type, op_type_subtract);
	dice_roll_instruction_free(dri);
}

void test_dice_roll_instruction_from_string_with_sum() {
	DiceRollInstruction *dri = dice_roll_instruction_from_string("sum");
	CU_ASSERT_PTR_NOT_NULL(dri);
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	CU_ASSERT_EQUAL(op_type, op_type_sum);
	dice_roll_instruction_free(dri);
}


void test_dice_roll_instruction_from_string_with_mean() {
	DiceRollInstruction *dri = dice_roll_instruction_from_string("mean");
	CU_ASSERT_PTR_NOT_NULL(dri);
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	CU_ASSERT_EQUAL(op_type, op_type_mean);
	dice_roll_instruction_free(dri);
}

void test_dice_roll_instruction_from_string_with_max() {
	DiceRollInstruction *dri = dice_roll_instruction_from_string("max");
	CU_ASSERT_PTR_NOT_NULL(dri);
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	CU_ASSERT_EQUAL(op_type, op_type_max);
	dice_roll_instruction_free(dri);
}

void test_dice_roll_instruction_from_string_with_double() {
	DiceRollInstruction *dri = dice_roll_instruction_from_string("3.5");
	CU_ASSERT_PTR_NOT_NULL(dri);
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	CU_ASSERT_EQUAL(op_type, op_type_number);
	dice_roll_instruction_free(dri);
}

void test_dice_roll_instruction_from_string_with_dice_collection() {
	DiceRollInstruction *dri = dice_roll_instruction_from_string("3d6");
	CU_ASSERT_PTR_NOT_NULL(dri);
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	CU_ASSERT_EQUAL(op_type, op_type_dice_collection);
	dice_roll_instruction_free(dri);
}

int test_suite_dice_roll_instruction(int(*init_suite)(void), int(*clean_suite)(void) ) {
	
	CU_pSuite pSuite = CU_add_suite("test_suite_dice_roll_instruction", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_init",
		test_dice_roll_instruction_init)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_from_string_with_plus",
		test_dice_roll_instruction_from_string_with_plus)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_from_string_with_minus",
		test_dice_roll_instruction_from_string_with_minus)) {
		CU_cleanup_registry();
		return CU_get_error();
	}


	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_from_string_with_sum",
		test_dice_roll_instruction_from_string_with_sum)) {
		CU_cleanup_registry();
		return CU_get_error();
	}


	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_from_string_with_mean",
		test_dice_roll_instruction_from_string_with_mean)) {
		CU_cleanup_registry();
		return CU_get_error();
	}


	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_from_string_with_max",
		test_dice_roll_instruction_from_string_with_max)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_from_string_with_double",
		test_dice_roll_instruction_from_string_with_double)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_from_string_with_dice_collection",
		test_dice_roll_instruction_from_string_with_dice_collection)) {
		CU_cleanup_registry();
		return CU_get_error();
	}


	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}