#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "Die.h"
#include "DiceCollection.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "DiceRollInstructionResultStack.h"

#include "TestSuiteDiceRollInstructionStack.h"


void test_dice_roll_instruction_stack_create(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(1);

	CU_ASSERT_PTR_NOT_NULL(dris);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_push(void) {
	// given
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(1);
	DiceRollInstruction *dri1 = dice_roll_instruction_from_string("max");

	// when
	dice_roll_instruction_stack_push(dris, dri1);

	// then
	DiceRollInstruction *dri2 = dice_roll_instruction_stack_peek(dris);
	int args1 = dice_roll_instruction_get_num_args(dri1);
	int args2 = dice_roll_instruction_get_num_args(dri2);
	CU_ASSERT_EQUAL(args1, args2);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_pop(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(1);
	DiceRollInstruction *dri1 = dice_roll_instruction_from_string("max");
	dice_roll_instruction_stack_push(dris, dri1);
	DiceRollInstruction *dri2 = dice_roll_instruction_stack_pop(dris);
	int args1 = dice_roll_instruction_get_num_args(dri1);
	int args2 = dice_roll_instruction_get_num_args(dri2);
	CU_ASSERT_EQUAL(args1, args2);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_dice_collection(void) {	
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(1);
	DiceRollInstruction *dri_dc = dice_roll_instruction_from_string("10d20");
	dice_roll_instruction_stack_push(dris, dri_dc);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);
	DiceCollection *real_result = dice_roll_instruction_result_get_dice_collection(drir);
	CU_ASSERT_EQUAL(dice_collection_count(real_result), 10);
	CU_ASSERT_EQUAL(dice_collection_faces(real_result), 20);

	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_add_two_ints(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(3);
	DiceRollInstruction *dri_num1 = dice_roll_instruction_from_string("1");
	DiceRollInstruction *dri_num2 = dice_roll_instruction_from_string("2");
	DiceRollInstruction *dri_add = dice_roll_instruction_from_string("+");

	dice_roll_instruction_stack_push(dris, dri_add);
	dice_roll_instruction_stack_push(dris, dri_num2);
	dice_roll_instruction_stack_push(dris, dri_num1);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 3.0);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_subtract_two_ints(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(3);
	DiceRollInstruction *dri_num1 = dice_roll_instruction_from_string("109");
	DiceRollInstruction *dri_num2 = dice_roll_instruction_from_string("9");
	DiceRollInstruction *dri_subtract = dice_roll_instruction_from_string("-");
	
	dice_roll_instruction_stack_push(dris, dri_subtract);
	dice_roll_instruction_stack_push(dris, dri_num2);
	dice_roll_instruction_stack_push(dris, dri_num1);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 100.0);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_multiply_two_ints(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(3);
	DiceRollInstruction *dri_num1 = dice_roll_instruction_from_string("7");
	DiceRollInstruction *dri_num2 = dice_roll_instruction_from_string("8");
	DiceRollInstruction *dri_multiply = dice_roll_instruction_from_string("*");

	dice_roll_instruction_stack_push(dris, dri_multiply);
	dice_roll_instruction_stack_push(dris, dri_num1);
	dice_roll_instruction_stack_push(dris, dri_num2);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 56.0);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_divide_two_ints(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(3);
	DiceRollInstruction *dri_num1 = dice_roll_instruction_from_string("101");
	DiceRollInstruction *dri_num2 = dice_roll_instruction_from_string("2");
	DiceRollInstruction *dri_divide = dice_roll_instruction_from_string("/");

	dice_roll_instruction_stack_push(dris, dri_divide);
	dice_roll_instruction_stack_push(dris, dri_num2);
	dice_roll_instruction_stack_push(dris, dri_num1);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 50.5);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_max_dice_collection(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(1);
	DiceRollInstruction *max = dice_roll_instruction_from_string("max");
	DiceRollInstruction *dri_dc = dice_roll_instruction_from_string("4d6");

	long long int *results = malloc(sizeof(long long int) * 4);
	results[0] = 1; results[1] = 5; results[2] = 3; results[3] = 2;
	dice_collection_set_results(dri_dc->value, results);

	dice_roll_instruction_stack_push(dris, max);
	dice_roll_instruction_stack_push(dris, dri_dc);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 5.0);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_add_num_to_dc(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(3);
	DiceRollInstruction *add = dice_roll_instruction_from_string("+");
	DiceRollInstruction *num10 = dice_roll_instruction_from_string("10");
	DiceRollInstruction *dri_dc = dice_roll_instruction_from_string("4d6");
	long long int results[] = {1, 6, 3, 2};
	dice_collection_set_results(dri_dc->value, results);
	
	dice_roll_instruction_stack_push(dris, add);
	dice_roll_instruction_stack_push(dris, num10);
	dice_roll_instruction_stack_push(dris, dri_dc);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 22);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_add_num_to_dc2(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(3);
	DiceRollInstruction *add = dice_roll_instruction_from_string("+");
	DiceRollInstruction *num10 = dice_roll_instruction_from_string("10");
	DiceRollInstruction *dri_dc = dice_roll_instruction_from_string("2d1");
	
	dice_roll_instruction_stack_push(dris, add);
	dice_roll_instruction_stack_push(dris, num10);
	dice_roll_instruction_stack_push(dris, dri_dc);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 12);
	dice_roll_instruction_stack_free(dris);
}

void test_dice_roll_instruction_stack_evaluate_add_dc_to_dc(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(3);
	DiceRollInstruction *add = dice_roll_instruction_from_string("+");
	DiceRollInstruction *dri_dc1 = dice_roll_instruction_from_string("6d6");
	DiceRollInstruction *dri_dc2 = dice_roll_instruction_from_string("4d6");
	long long int results1[] = {1, 2, 4, 6, 3, 2};
	long long int results2[] = {1, 6, 3, 2};
	dice_collection_set_results(dri_dc1->value, results1);
	dice_collection_set_results(dri_dc2->value, results2);

	dice_roll_instruction_stack_push(dris, add);
	dice_roll_instruction_stack_push(dris, dri_dc1);
	dice_roll_instruction_stack_push(dris, dri_dc2);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	CU_ASSERT_EQUAL(real_result, (double) 18 + 12);
	dice_roll_instruction_stack_free(dris);
}


void test_dice_roll_instruction_stack_evaluate_add_three_ints(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(5);
	DiceRollInstruction *dri_add1 = dice_roll_instruction_from_string("+");
	DiceRollInstruction *dri_add2 = dice_roll_instruction_from_string("+");

	DiceRollInstruction *dri_num2 = dice_roll_instruction_from_string("2");
	DiceRollInstruction *dri_num5 = dice_roll_instruction_from_string("5");
	DiceRollInstruction *dri_num10 = dice_roll_instruction_from_string("10");

	dice_roll_instruction_stack_push(dris, dri_add1);
	dice_roll_instruction_stack_push(dris, dri_num10);
	dice_roll_instruction_stack_push(dris, dri_add2);
	dice_roll_instruction_stack_push(dris, dri_num5);
	dice_roll_instruction_stack_push(dris, dri_num2);

	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
    printf("real_result=%lf;\n", real_result);
	CU_ASSERT_EQUAL(real_result, (double) 2 + 5 + 10);
	dice_roll_instruction_stack_free(dris);
}


void test_dice_roll_instruction_stack_evaluate_multiply_then_add_three_ints(void) {
	DiceRollInstructionStack *dris = dice_roll_instruction_stack_create(5);

	DiceRollInstruction *dri_multiply = dice_roll_instruction_from_string("*");
	DiceRollInstruction *dri_add = dice_roll_instruction_from_string("+");
	DiceRollInstruction *dri_num9 = dice_roll_instruction_from_string("9");
	DiceRollInstruction *dri_num10 = dice_roll_instruction_from_string("10");
	DiceRollInstruction *dri_num1 = dice_roll_instruction_from_string("1");

	dice_roll_instruction_stack_push(dris, dri_add);
	dice_roll_instruction_stack_push(dris, dri_num1);
	dice_roll_instruction_stack_push(dris, dri_multiply);
	dice_roll_instruction_stack_push(dris, dri_num10);
	dice_roll_instruction_stack_push(dris, dri_num9);
	DiceRollInstructionResult *drir = dice_roll_instruction_stack_evaluate(dris);

	double real_result = dice_roll_instruction_result_get_number(drir);
	printf("%lf\n", real_result);
	CU_ASSERT_EQUAL(real_result, (double) 91);
	dice_roll_instruction_stack_free(dris);
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

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_dice_collection",
		test_dice_roll_instruction_stack_evaluate_dice_collection)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_add_two_ints",
		test_dice_roll_instruction_stack_evaluate_add_two_ints)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_subtract_two_ints",
		test_dice_roll_instruction_stack_evaluate_subtract_two_ints)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_multiply_two_ints",
		test_dice_roll_instruction_stack_evaluate_multiply_two_ints)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_divide_two_ints",
		test_dice_roll_instruction_stack_evaluate_divide_two_ints)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
		
	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_max_dice_collection",
		test_dice_roll_instruction_stack_evaluate_max_dice_collection)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_add_num_to_dc",
		test_dice_roll_instruction_stack_evaluate_add_num_to_dc)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_add_num_to_dc2",
		test_dice_roll_instruction_stack_evaluate_add_num_to_dc2)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_add_dc_to_dc",
		test_dice_roll_instruction_stack_evaluate_add_dc_to_dc)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_add_three_ints",
		test_dice_roll_instruction_stack_evaluate_add_three_ints)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite, "test_dice_roll_instruction_stack_evaluate_multiply_then_add_three_ints",
		test_dice_roll_instruction_stack_evaluate_multiply_then_add_three_ints)) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}
