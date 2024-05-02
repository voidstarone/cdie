#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "DynArray.h"
#include "DiceNotationInterpreter.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "ExpressionToDiceRollInstructionStack.h"

typedef struct {
	size_t index;
	size_t length;
} Range;

typedef struct {
	OperationType operation;
	size_t index;
} OperatorWithLocation;

OperatorWithLocation *create_operator_with_location(
	char *text, 
	size_t index
) {
	OperatorWithLocation *op = malloc(sizeof(OperatorWithLocation));
	if (strcmp(text, "+") == 0) {
		op->index = index;
		op->operation = op_type_add;
	}
	if (strcmp(text, "-") == 0) {
		op->index = index;
		op->operation = op_type_subtract;
	}
	if (strcmp(text, "*") == 0) {
		op->index = index;
		op->operation = op_type_multiply;
	}
	if (strcmp(text, "/") == 0) {
		op->index = index;
		op->operation = op_type_divide;
	}
	return op;
}

void free_operator_with_location(OperatorWithLocation *op) {
	free(op);
	op = NULL;
}

Range *range_create() {
	return malloc(sizeof(Range));
}

void range_free(Range *r) {
	if (r != NULL) {
		free(r);
	}
	r = NULL;
}

typedef struct {
	Range *range;
	int priority;
} RangeWithPriority;

RangeWithPriority *range_with_priority_create() {
	RangeWithPriority *r = malloc(sizeof(RangeWithPriority));
	r->range = range_create();
	if (r->range == NULL) {
		free(r);
		r = NULL;
	}
	return r;
}


void range_with_priority_free(RangeWithPriority *r) {
	range_free(r->range);
	if (r != NULL) {
		free(r);
	}
	r = NULL;
}

int range_with_priority_compare_priority_asc(void *arg1, void *arg2) {
	RangeWithPriority *r1 = arg1, *r2 = arg2;
	if (r1->priority > r2->priority) {
		return 1;
	} else if (r1->priority < r2->priority) {
		return -1;
	} else {
		return 0;
	}
}

int range_with_priority_compare_priority_desc(void *arg1, void *arg2) {
	return range_with_priority_compare_priority_asc(arg2, arg1);
}


void range_with_priority_print(void *arg) {
	RangeWithPriority *r = arg;
	printf("{priority:%d, range: %zu:%zu}", r->priority, r->range->index, r->range->length);
}

int range_with_priority_prioriy(RangeWithPriority *r) {
	return r->priority;
}

void range_with_priority_set_prioriy(RangeWithPriority *r, int priority) {
	r->priority = priority;
}

size_t range_with_priority_index(RangeWithPriority *r) {
	return r->range->index;
}

void range_with_priority_set_index(RangeWithPriority *r, size_t index) {
	r->range->index = index;
}

size_t range_with_priority_length(RangeWithPriority *r) {
	return r->range->length;
}

void range_with_priority_set_length(RangeWithPriority *r, size_t length) {
	r->range->length = length;
}

DynArray *split_string(char *str, const char *delimiter) {
    char *token;
    char *rest = str;
    DynArray *array = dyn_array_create(32);

    // Iterate through the string and split it into tokens
    while ((token = strtok_r(rest, delimiter, &rest)) != NULL) {
        dyn_array_push(array, token);
    }
    return array;
}

bool is_operator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

bool could_be_number(char *str) {
	char c = str[0];
	if (!is_digit(c)) {
		return false;
	}
	int count_decimals = 0;
	
	for (size_t i = 1, len = strlen(str); i < len; i++) {
		c = str[i];
		if (c == '.') {
			count_decimals++;
			continue;
		}
		if (count_decimals > 1) {
			return false;
		}
		if (!is_digit(c)) {
			return false;
		}
	}
	return true;
}

bool could_be_dice_collection(char *str) {
	char c = str[0];
	if (!is_digit(c)) {
		return false;
	}
	int count_ds = 0;
	
	for (size_t i = 1, len = strlen(str); i < len; i++) {
		c = str[i];
		if (c == 'd' || c == 'D') {
			count_ds++;
			if (i+1 == len) {
				return false;
			}
			if (is_digit(str[i+1])) {
				continue;
			}			
		}
		if (count_ds > 1) {
			return false;
		}
		if (!is_digit(c)) {
			return false;
		}
	}
	return count_ds == 1;
}

// void expression_to_infix(DynArray *array, char *exp) {
	
// 	size_t len = strlen(exp);
// 	size_t part_start = 0;
// 	char c;
// 	for (size_t i = 0; i < len; i++) {
// 		c = exp[i];
// 		if (is_operator(c) && part_start == i) {
// 			dyn_array_push(array, c);
// 		}
// 	}
	
// }

size_t index_of_innermost_opening_paren(char *str, size_t start_index, size_t length) {
	size_t paren_index = 0;
	int max_depth = 0;
	int depth = 0;
	for (size_t i = start_index; i < length; i++) {
		if (str[i] == '(') {
			if (depth > max_depth) {
				paren_index = i;
				max_depth = depth;
			}
			++depth;
		}
		if (str[i] == ')') {
			--depth;
		}
	}
	return paren_index;
}

size_t index_of_next_closing_param(char *str, size_t start_index) {
	for (size_t i = start_index, length = strlen(str); i < length; i++) {
		if (str[i] == ')') {
			return i;
		}
	}
	return 0;
}

size_t count_pairs_of_parens_in_range(char *str, size_t start_index, size_t length) {
	int depth = 0;
	size_t count = 0;
	for (size_t i = start_index; i < length; i++) {
		if (str[i] == '(') {
			++depth;
		}
		if (str[i] == ')') {
			if (depth == 1) {
				++count;
			}
			--depth;
		}
	}
	return count;
}

bool char_is_operator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

int priority_for_operator(char c) {
	if (c == '^') {
		return 6;
	}
	if (c == '*' || c == '/') {
		return 5;
	}
	if (c == '+' || c == '-') {
		return 4;
	}
	return 0;
}

size_t index_of_next_operator_in_range(char *str, size_t start_index, size_t length) {
	for (size_t i = start_index; i < length; i++) {
		if (char_is_operator(str[i])) {
			return i;
		}
	}
	return SIZE_MAX;
}



void dice_roll_instruction_stack_from_expression(char *expression) {
	DynArray *postfix = dyn_array_create(32);
	size_t opening_index = index_of_innermost_opening_paren(expression, 0, strlen(expression) ) + 1;
	size_t closing_index = index_of_next_closing_param(expression, opening_index);
	size_t length = closing_index - opening_index;
	char part[64];
	for (size_t i = 0; i < length; i++)
	{
		part[i] = expression[opening_index + i];
	}
	part[length] = '\0';
	printf("part: %s\n", part);
	size_t pairs = count_pairs_of_parens_in_range(expression, opening_index + 1, length);
	printf("pairs: %ld\n", pairs);
	for (size_t pair = 0; pair < pairs; pair++) {
		// do this after going up a level
	}
	DynArray *operators = dyn_array_create(8);
	// create list of operator indices in priority order
	printf("opening_index: %ld, length: %ld\n", opening_index, length);
	for (size_t i = opening_index; i < closing_index; i++) {
		char c = expression[i];
		printf("%c", c);
		if (is_operator(c)) {
			int priority = priority_for_operator(c);
			RangeWithPriority *r = range_with_priority_create();
			range_with_priority_set_index(r, i);
			range_with_priority_set_length(r, 1);
			range_with_priority_set_prioriy(r, priority_for_operator(c));
			dyn_array_push(operators, r);
		}
	}
	printf("\n");
	dyn_array_print(operators, &range_with_priority_print);
	dyn_array_sort_in_place(operators, &range_with_priority_compare_priority_desc);
	// sort list
	
	for (size_t i = 0; i < dyn_array_count(operators); i++) {
		if (i != 0) {
			printf(", ");
		}
		RangeWithPriority *r = dyn_array_element_at_index(operators, i);
		int priority = range_with_priority_prioriy(r);
		printf("%d", priority);
		/* code */
	}
	printf("\n");

	printf("index: %ld-%ld\n", opening_index, closing_index);

	size_t first_op = index_of_next_operator_in_range(expression, opening_index, closing_index);
}

    // char exp[] = "(1 + 2)";
    // split_string(array, "1 + 2d6 * 10, hello world", ",");
    // DynArray *array = printWords("1 + 2d6 * 10", " ");
        
    // const char delimiter[] = " ";

    // DynArray *strings = split_string(exp, delimiter);

    // for (size_t i = 0; i < dyn_array_count(strings); i++)
    // {
    //     printf("%s\n", dyn_array_element_at_index(strings, i));
    // }
    
	// DynArray *infix = dyn_array_create(32);
    // expression_to_infix(infix, "1 +  2d6 * 10");

	// for (size_t i = 0; i < dyn_array_count(infix); i++)
    // {
    //     printf("%s\n", dyn_array_element_at_index(infix, i));
    // }

	// printf("%d\n", could_be_number("1"));
	// printf("%d\n", could_be_number("200"));
	// printf("%d\n", could_be_number("1.1"));
	// printf("%d\n", could_be_number("1e"));
	// printf("%d\n", could_be_number("+"));
	// printf("%d\n", could_be_number("."));
	// printf("%d\n", could_be_number("10d6"));
	// printf("%d\n", could_be_number("10..6"));
	// printf("---\n");
	// printf("%d\n", could_be_dice_collection("1"));
	// printf("%d\n", could_be_dice_collection("200"));
	// printf("%d\n", could_be_dice_collection("1.1"));
	// printf("%d\n", could_be_dice_collection("1e"));
	// printf("%d\n", could_be_dice_collection("+"));
	// printf("%d\n", could_be_dice_collection("."));
	// printf("%d\n", could_be_dice_collection("10d6"));
	// printf("%d\n", could_be_dice_collection("d6"));
	// printf("%d\n", could_be_dice_collection("1d96"));
	// printf("%d\n", could_be_dice_collection("10dd6"));
	// printf("%d\n", could_be_dice_collection("10d1d6"));