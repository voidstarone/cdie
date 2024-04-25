#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
// #include "DynArray.h"
// #include "DiceRollInstructionStack.h"
// #include "DiceRollInstructionStack.h"
typedef struct {
  void **array;
  size_t used;
  size_t size;
} DynArray;

typedef struct {
	size_t index;
	size_t length;
} Range;

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

DynArray *dyn_array_create(size_t initial_size);

size_t dyn_array_count(DynArray *a);

bool dyn_array_is_empty(DynArray *a);

void dyn_array_push(DynArray *a, void *element);

void *dyn_array_peek(DynArray *a);

void *dyn_array_pop(DynArray *a);

void *dyn_array_element_at_index(DynArray *a, size_t index);

void dyn_array_set_element_at_index(DynArray *a, size_t index, void *new_element);

void dyn_array_free(DynArray *a);

DynArray *dyn_array_create(size_t initial_size) {
	DynArray *a = (DynArray *) malloc(sizeof(DynArray));
	if (a == NULL) {
		return NULL;
	}
	a->array = (void **) malloc(initial_size * sizeof(void *));
	if (a->array == NULL) {
		free(a);
		return NULL;
	}
	a->used = 0;
	a->size = initial_size;
	return a;
}

size_t dyn_array_count(DynArray *a) {
	return a->used;
}

bool dyn_array_is_empty(DynArray *a) {
	return dyn_array_count(a) == 0;
}

void dyn_array_push(DynArray *a, void *element) {
	if (a->used == a->size) {
		a->size *= 2;
		void **tmp_array = (void **) realloc(a->array, a->size * sizeof(void *));
		if (tmp_array == NULL) {
			printf("ERROR: dyn_array(%p) failed to grow\n", a);
			return;
		}
		a->array = tmp_array;
	}
	a->array[a->used++] = element;
}

void *dyn_array_peek(DynArray *a) {
	if (a->used == 0) {
		return NULL;
	}
	return a->array[a->used-1];
}

void *dyn_array_pop(DynArray *a) {
	if (a->used == 0) {
		return NULL;
	}
	void *element = a->array[--a->used];
	if (a->used < (a->size / 4)) {
		a->size/= 2;
		void **tmp_array = realloc(a->array, a->size * sizeof(void *));
		if (tmp_array == NULL) {
			printf("WARNING: dyn_array(%p) failed to shrink\n", a);
		} else {
			a->array = tmp_array;
		}
	}
	return element;
}

void *dyn_array_element_at_index(DynArray *a, size_t index) {
	size_t real_index = index < 0 ? a->used - index : index;
	if (real_index >= a->used) {
		printf("WARNING: dyn_array(%p) index(%ld -> %ld) out of bounds\n", a, index, real_index);
		return NULL;
	}
	return a->array[real_index];
}

void dyn_array_set_element_at_index(DynArray *a, size_t index, void *new_element) {
	size_t real_index = index < 0 ? a->used - index : index;
	if (real_index >= a->used) {
		printf("WARNING: dyn_array(%p) index(%ld -> %ld) out of bounds\n", a, index, real_index);
		return;
	}
	a->array[index] = new_element;
}

void dyn_array_free(DynArray *a) {
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
	free(a);
	a = NULL;
}

//sss

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

bool is_digit(char c) {
	return c >= '0' && c <= '9';
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

int main() {
    char exp[] = "(1 + ((2d6) * ((10 + 1 * 80) + 1)))";
	printf("%s\n", exp);
	DynArray *postfix = dyn_array_create(32);
	size_t opening_index = index_of_innermost_opening_paren(exp, 0, strlen(exp) ) + 1;
	size_t closing_index = index_of_next_closing_param(exp, opening_index);
	size_t length = closing_index - opening_index;
	char part[64];
	for (size_t i = 0; i < length; i++)
	{
		part[i] = exp[opening_index + i];
	}
	part[length] = '\0';
	printf("part: %s\n", part);
	size_t pairs = count_pairs_of_parens_in_range(exp, opening_index + 1, length);
	printf("pairs: %ld\n", pairs);
	for (size_t pair = 0; pair < pairs; pair++) {
		// do this after going up a level
	}
	DynArray *operators = dyn_array_create(8);
	// create list of operator indices in priority order
	printf("opening_index: %ld, length: %ld\n", opening_index, length);
	for (size_t i = opening_index; i < closing_index; i++) {
		char c = exp[i];
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

	size_t first_op = index_of_next_operator_in_range(exp, opening_index, closing_index);
	

	
	

    return 0;
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