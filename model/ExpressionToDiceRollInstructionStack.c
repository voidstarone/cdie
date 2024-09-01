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

typedef enum {
	RANGE,
	RANGE_WITH_PRIORITY
} RangeType;

typedef struct {
	RangeType type;
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

Range *range_create(void) {
	return malloc(sizeof(Range));
}

Range *range_create_and_init(size_t index, size_t length) {
	Range *r = malloc(sizeof(Range));
	if (r == NULL) {
		return NULL;
	}
	r->index = index;
	r->length = length;
	return r;
}

void range_free(Range *r) {
	if (r != NULL) {
		free(r);
	}
	r = NULL;
}

size_t range_get_index(Range *r) {
	return r->index;
}

size_t range_get_length(Range *r) {
	return r->length;
}

bool range_is_in_range(Range *r, size_t i) {
	return i >= r->index && i <= (r->index + r->length);
}

typedef struct {
	RangeType type;
	Range *range;
	int priority;
} RangeWithPriority;

RangeWithPriority *range_with_priority_create(void) {
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

int range_with_priority_get_prioriy(RangeWithPriority *r) {
	return r->priority;
}

void range_with_priority_set_prioriy(RangeWithPriority *r, int priority) {
	r->priority = priority;
}

size_t range_with_priority_get_index(RangeWithPriority *r) {
	return r->range->index;
}

void range_with_priority_set_index(RangeWithPriority *r, size_t index) {
	r->range->index = index;
}

size_t range_with_priority_get_length(RangeWithPriority *r) {
	return r->range->length;
}

void range_with_priority_set_length(RangeWithPriority *r, size_t length) {
	r->range->length = length;
}

bool range_compare(void *r1, void *r2) {
	Range *range1 = r1, *range2 = r2;
	return range1->index == range2->index && range1->length == range2->length;
}

void range_print(void *r) {
	printf("Range{index:%zu, length:%zu}", range_get_index(r), range_get_length(r));
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

bool could_be_operand(char *str) {
	return is_digit(str[0]);
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
	if (max_depth == 0) {
		return SIZE_MAX;
	}
	return paren_index;
}

size_t index_of_innermost_opening_paren_ignoring_ranges(
	char *str, 
	size_t start_index, 
	size_t length,
	DynArray *ranges_to_ignore
) {
	size_t paren_index = 0;
	int max_depth = 0;
	int depth = 0;
	for (size_t i = start_index; i < length; i++) {
		for (size_t ri = 0; ri < dyn_array_count(ranges_to_ignore); ri++) {
			Range *r = dyn_array_element_at_index(ranges_to_ignore, ri);
			if (range_is_in_range(r, i)) {
				continue;
			}
		}
		
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

bool char_is_inline_whitespace(char c) {
	return c == ' ' || c == '\t';
}

int precedence_for_operator(char c) {
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

Range *range_of_operand_moving_left(char *str, size_t start_index, size_t leftmost_bound) {
	Range *range = range_create();
	bool have_found_operand = false;
	bool have_found_end_of_operand = false;
	size_t left_index = leftmost_bound;
	size_t right_index = start_index;
    if (start_index == 0) {
        range_free(range);
        return NULL;
    }
	for (size_t i = start_index; i >= leftmost_bound; i--) {
		
		// printf("i: %zu\n", i);
		if (!have_found_operand && !char_is_inline_whitespace(str[i])) {
			right_index = i;
			have_found_operand = true;
			continue;
		}
		if (have_found_operand && char_is_inline_whitespace(str[i])) {
			left_index = i + 1;
			have_found_end_of_operand = true;
			break;
		}
		if (i == 0) {
			break;
		}
	}
	if (!have_found_end_of_operand) {
		left_index = leftmost_bound;
	}
	range->index = left_index;
	range->length = right_index - left_index + 1;
	return range;
}


Range *range_of_operand_moving_right(char *str, size_t start_index, size_t rightmost_bound) {
	Range *range = range_create();
	bool have_found_operand = false;
	bool have_found_end_of_operand = false;
	size_t left_index = start_index;
	size_t right_index = rightmost_bound;
	for (size_t i = start_index; i <= rightmost_bound; i++) {
		if (!have_found_operand && !char_is_inline_whitespace(str[i])) {
			left_index = i;
			have_found_operand = true;
			continue;
		}
		if (have_found_operand && char_is_inline_whitespace(str[i])) {
			right_index = i;
			have_found_end_of_operand = true;
			break;
		}
	}
	if (!have_found_end_of_operand) {
		right_index = rightmost_bound;
	}
	range->index = left_index;
	range->length = right_index - left_index;
	return range;
}

void copy_range_to_string(char *buf, char *target, size_t start_index, size_t length) {
	if (length == 0) { return; }
	for (size_t i = 0; i < length; i++) {
		buf[i] = target[start_index + i];
	}
	buf[length - 1] = '\0';
}

void postfixify_expression(DynArray *postfix_ranges, char *expression) {
    size_t length = strlen(expression);
//    printf("%s\n", expression);
	DynArray *operator_stack = dyn_array_create(16);
    for (size_t e_i = 0; e_i < length; ++e_i) {
        char c = expression[e_i];
//        printf("%zu:%c\n", e_i, c);
        if (c == '(') {
            dyn_array_push(operator_stack, range_create_and_init(e_i, 1));
        } else if (c == ')') {
            // Repeatedly pop from stack and add it to the postfix expression until a "(" is encountered.
            // Discard the "(". That is, remove the "(" from stack and do not add it to the postfix expression
            while (!dyn_array_is_empty(operator_stack)) {
                Range *r = dyn_array_pop(operator_stack);
                if (expression[range_get_index(r)] == '(') {
                    break;
                }
                char operator = expression[r->index];
//                printf("\n )popped: %c\n", operator);
                dyn_array_push(postfix_ranges, r);
            }
        } else if (is_operator(c)) {
            // IF an operator O is encountered, then
            //   Repeatedly pop from stack and add each operator popped from the stack to the
            //       postfix expression which has equal or higher precedence than O
            //   Push the operator O to the stack.
            int precedence = precedence_for_operator(c);
            
//            for (size_t i = 0; i < dyn_array_count(operator_stack); ++i) {
//                Range *r = dyn_array_element_at_index(operator_stack, i);
//                printf("operator %zu\n", i);
//                for (size_t j = r->index; j < r->index + r->length; ++j) {
//                    printf("%c", expression[j]);
//                }
//                printf("\n");
//            }
            Range *r = dyn_array_peek(operator_stack);
            char operator = expression[range_get_index(r)];
            while (!dyn_array_is_empty(operator_stack) && precedence_for_operator(operator) >= precedence) {
                Range *r = dyn_array_pop(operator_stack);
                size_t o_i = range_get_index(r);
                char operator = expression[o_i]; // assue operator is 1 char
//                printf("%c", operator);
//                printf("\n op popped: %c\n", operator);
                dyn_array_push(postfix_ranges, r);
                r = dyn_array_peek(operator_stack);
                operator = expression[range_get_index(r)];
            }
            dyn_array_push(operator_stack, range_create_and_init(e_i, 1));
        } else if (is_digit(c) || c == '.' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) {
            // push operand to postfix
            // add the whole operand
            // jump i ahead
            
            // go through the chars until we reach either and operator or a paren
            size_t index_after_operand = -1;
            char tmp_c;
            for (size_t o_i = e_i; o_i < length; ++o_i) {
                tmp_c = expression[o_i];
                if (tmp_c == ')' || char_is_inline_whitespace(tmp_c) || tmp_c == '\0') {
                    index_after_operand = o_i;
                    break;
                }
            }
            if (index_after_operand == (size_t) -1) {
                // something went wrong
                printf("SHIT!\n");
                continue;
            }
            size_t length = index_after_operand - e_i;
            Range *r = range_create_and_init(e_i, length);
            dyn_array_push(postfix_ranges, r);
            e_i = index_after_operand - 1;
        } else if (char_is_inline_whitespace(expression[e_i])) {
            continue;
        } else if (c == '\0') {
            break;
        } else{
			goto cleanup;
		}
	}

	while (!dyn_array_is_empty(operator_stack)) {
		Range *r = dyn_array_pop(operator_stack);
		dyn_array_push(postfix_ranges, r);
	}
    dyn_array_reverse_in_place(postfix_ranges);

//    printf("final (%zu):", dyn_array_count(postfix_ranges));
//    for (size_t i = 0; i < dyn_array_count(postfix_ranges); ++i) {
//        Range *r = dyn_array_element_at_index(postfix_ranges, i);
//        for (size_t j = r->index; j < r->index + r->length; ++j) {
//            printf("%c", expression[j]);
//        }
//        printf(" ");
//    }
//    printf("\n");
    
	cleanup:
	dyn_array_free(operator_stack);
}

DiceRollInstructionStack *dice_roll_instruction_stack_from_expression(char *expression) {
	DiceRollInstructionStack *instruction_stack = dice_roll_instruction_stack_create(32);
	DynArray *postfix_ranges = dyn_array_create(32);
	size_t opening_index = index_of_innermost_opening_paren(expression, 0, strlen(expression));
	size_t closing_index;
	if (opening_index == SIZE_MAX) { // No parens
		opening_index = 0;
		closing_index = strlen(expression) - 1;
	} else {
		opening_index++;
		closing_index = index_of_next_closing_param(expression, opening_index);
	}
    
    size_t length = strlen(expression) + 3;
    char *working_expression = malloc(sizeof(char) * length);
    snprintf(working_expression, length, "(%s)", expression);
//     printf("%s\n", working_expression);
    postfixify_expression(postfix_ranges, working_expression);
	
//    dyn_array_print(postfix_ranges, &range_print);
//	  for (size_t i = 0; i < dyn_array_count(postfix_ranges); i++) {
//	 	  if (i != 0) {
//	 	  	printf(", ");
//	 	  }
//	 	  Range *range = dyn_array_element_at_index(postfix_ranges, i);
//	 	  size_t start_index = range_get_index(range);
//	 	  for (size_t j = start_index; j < start_index + range_get_length(range); j++) {
//	 		  printf("%c", working_expression[j]);
//	 	  }
//	   }
//     printf("\n");
	
	size_t max_op_length = 8;
	char *op_as_string = malloc(sizeof(char) * max_op_length);
	for (size_t i = 0; i < dyn_array_count(postfix_ranges); i++) {
		Range *range = dyn_array_element_at_index(postfix_ranges, i);
		size_t range_start = range_get_index(range);
		size_t op_strlen = range_get_length(range) + 1;
		if (op_strlen > max_op_length) {
			op_as_string = realloc(op_as_string, sizeof(char) * op_strlen);
			max_op_length = op_strlen;
		}
		copy_range_to_string(op_as_string, working_expression, range_start, op_strlen);

		DiceRollInstruction *instruction = dice_roll_instruction_from_string(op_as_string);
		dice_roll_instruction_stack_push(instruction_stack, instruction);
	}
	free(op_as_string);
	op_as_string = NULL;

	return instruction_stack;
}
