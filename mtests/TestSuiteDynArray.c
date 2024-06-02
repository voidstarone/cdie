#include <stdbool.h>
#include <stdlib.h>

#include "CUnit/Basic.h"
#include "DynArray.h"
#include "TestSuiteDynArray.h"

void test_dyn_array_create() {
    DynArray *a = dyn_array_create(6);
    
    CU_ASSERT_NOT_EQUAL(a, NULL);
    CU_ASSERT_EQUAL(a->size, 6);
    CU_ASSERT_EQUAL(a->used, 0);

    dyn_array_free(a);
}

void test_dyn_array_push_int() {
    DynArray *a = dyn_array_create(6);

    int expected_number = 8;
    int *expected_element = &expected_number;
    dyn_array_push(a, expected_element);
    
    int *actual_element = dyn_array_element_at_index(a, 0);
    int actual_number = *actual_element;
    CU_ASSERT_EQUAL(actual_number, expected_number);
    dyn_array_free(a);
}

void test_dyn_array_peek_int() {
    DynArray *a = dyn_array_create(6);

    int expected_number = 8;
    int *expected_element = &expected_number;
    char junk = '0';
    dyn_array_push(a, &junk);
    dyn_array_push(a, expected_element);
    
    int *actual_element = dyn_array_peek(a);
    int actual_number = *actual_element;
    CU_ASSERT_EQUAL(actual_number, expected_number);
    dyn_array_free(a);
}

void test_dyn_array_push_ints() {
    DynArray *a = dyn_array_create(6);

    int expected_number1 = 8;
    int expected_number2 = 864;
    int expected_number3 = 10098;
    int *expected_element1 = &expected_number1;
    int *expected_element2 = &expected_number2;
    int *expected_element3 = &expected_number3;
    dyn_array_push(a, expected_element1);
    dyn_array_push(a, expected_element2);
    dyn_array_push(a, expected_element3);
    
    int *actual_element1 = dyn_array_element_at_index(a, 0);
    int *actual_element2 = dyn_array_element_at_index(a, 1);
    int *actual_element3 = dyn_array_element_at_index(a, 2);

    CU_ASSERT_EQUAL(actual_element1, expected_element1);
    CU_ASSERT_EQUAL(actual_element2, expected_element2);
    CU_ASSERT_EQUAL(actual_element3, expected_element3);
    
    dyn_array_free(a);
}

void test_dyn_array_push_ints_requiring_growth() {
    DynArray *a = dyn_array_create(2);

    int expected_number1 = 8;
    int expected_number2 = 864;
    int expected_number3 = 10098;
    int *expected_element1 = &expected_number1;
    int *expected_element2 = &expected_number2;
    int *expected_element3 = &expected_number3;
    dyn_array_push(a, expected_element1);
    dyn_array_push(a, expected_element2);
    dyn_array_push(a, expected_element3);
    
    int *actual_element1 = dyn_array_element_at_index(a, 0);
    int *actual_element2 = dyn_array_element_at_index(a, 1);
    int *actual_element3 = dyn_array_element_at_index(a, 2);

    CU_ASSERT_EQUAL(actual_element1, expected_element1);
    CU_ASSERT_EQUAL(actual_element2, expected_element2);
    CU_ASSERT_EQUAL(actual_element3, expected_element3);
    
    dyn_array_free(a);
}

void test_dyn_array_set_ints_requiring_growth() {
    DynArray *a = dyn_array_create(2);

    int expected_number1 = 8;
    int expected_number2 = 864;
    int *expected_element1 = &expected_number1;
    int *expected_element2 = &expected_number2;

    dyn_array_set_element_at_index(a, 0, expected_element1);
    dyn_array_set_element_at_index(a, 40, expected_element2);
    
    int *actual_element1 = dyn_array_element_at_index(a, 0);
    int *actual_element2 = dyn_array_element_at_index(a, 40);

    CU_ASSERT_EQUAL(actual_element1, expected_element1);
    CU_ASSERT_EQUAL(actual_element2, expected_element2);
    
    dyn_array_free(a);
}

void test_dyn_array_push_ints_pop_ints() {
    DynArray *a = dyn_array_create(2);

    int expected_number1 = 8;
    int expected_number2 = 864;
    int expected_number3 = 10098;
    int *expected_element1 = &expected_number1;
    int *expected_element2 = &expected_number2;
    int *expected_element3 = &expected_number3;
    dyn_array_push(a, expected_element1);
    dyn_array_push(a, expected_element2);
    dyn_array_push(a, expected_element3);
    
    int *actual_element3 = dyn_array_pop(a);
    int *actual_element2 = dyn_array_pop(a);
    int *actual_element1 = dyn_array_pop(a);
    

    CU_ASSERT_EQUAL(actual_element1, expected_element1);
    CU_ASSERT_EQUAL(actual_element2, expected_element2);
    CU_ASSERT_EQUAL(actual_element3, expected_element3);
    
    dyn_array_free(a);
}

int compare_int(void *a, void *b) {
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    return (*int_a - *int_b);
}

void print_element_int(void *element) {
    int *value = element;
    printf("%d", *value);
}

void print_element_address(void *element) {
    printf("%p", element);
}

void test_dyn_array_sort_in_place_ints() {
    int expected_number0 = 0;
    int expected_number1 = 8;
    int expected_number2 = 864;
    int expected_number3 = 10098;
    int expected_number4 = 10098;
    int expected_number5 = 50000;
    int expected_number6 = 100390;
    int *expected_element0 = &expected_number0;
    int *expected_element1 = &expected_number1;
    int *expected_element2 = &expected_number2;
    int *expected_element3 = &expected_number3;
    int *expected_element4 = &expected_number4;
    int *expected_element5 = &expected_number5;
    int *expected_element6 = &expected_number6;
    DynArray *a = dyn_array_create(2);
    dyn_array_push(a, expected_element1);
    dyn_array_push(a, expected_element6);
    dyn_array_push(a, expected_element2);
    dyn_array_push(a, expected_element4);
    dyn_array_push(a, expected_element0);
    dyn_array_push(a, expected_element3);
    dyn_array_push(a, expected_element5);

    dyn_array_sort_in_place(a, &compare_int);

    int *actual_element0 = dyn_array_element_at_index(a, 0);
    int *actual_element1 = dyn_array_element_at_index(a, 1);
    int *actual_element2 = dyn_array_element_at_index(a, 2);
    int *actual_element3 = dyn_array_element_at_index(a, 3);
    int *actual_element4 = dyn_array_element_at_index(a, 4);
    int *actual_element5 = dyn_array_element_at_index(a, 5);
    int *actual_element6 = dyn_array_element_at_index(a, 6);

    CU_ASSERT_EQUAL(actual_element0, expected_element0);
    CU_ASSERT_EQUAL(actual_element1, expected_element1);
    CU_ASSERT_EQUAL(actual_element2, expected_element2);
    CU_ASSERT_EQUAL(actual_element3, expected_element3);
    CU_ASSERT_EQUAL(actual_element4, expected_element4);
    CU_ASSERT_EQUAL(actual_element5, expected_element5);
    CU_ASSERT_EQUAL(actual_element6, expected_element6);
}

int test_suite_dyn_array (int(*init_suite)(void), int(*clean_suite)(void)) {
    
    const char *test_suite_name = "test_suite_dyn_array";
    CU_pSuite pSuite = CU_add_suite(test_suite_name, init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    printf("%s\n", test_suite_name);
    
    if (NULL == CU_add_test(pSuite, "test_dyn_array_create", 
        test_dyn_array_create)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test_dyn_array_push_int", 
        test_dyn_array_push_int)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test_dyn_array_peek_int", 
        test_dyn_array_peek_int)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test_dyn_array_push_ints", 
        test_dyn_array_push_ints)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test_dyn_array_push_ints_requiring_growth", 
        test_dyn_array_push_ints_requiring_growth)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

        if (NULL == CU_add_test(pSuite, "test_dyn_array_set_ints_requiring_growth", 
        test_dyn_array_set_ints_requiring_growth)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test_dyn_array_push_ints_pop_ints", 
        test_dyn_array_push_ints_pop_ints)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test_dyn_array_sort_in_place_ints", 
        test_dyn_array_sort_in_place_ints)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // Run the tests and show the run summary
    CU_basic_run_tests();
    return CU_get_error();
}