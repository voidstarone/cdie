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

void expression_to_infix(DynArray *array, char *exp) {
	
	size_t len = strlen(exp);
	size_t part_start = 0;
	char c;
	for (size_t i = 0; i < len; i++) {
		c = exp[i];
		if (is_operator(c) && part_start == i) {
			dyn_array_push(array, c);
		}
	}
	
}


int main() {
    char exp[] = "1 +  2d6 * 10";
    
    // split_string(array, "1 + 2d6 * 10, hello world", ",");
    // DynArray *array = printWords("1 + 2d6 * 10", " ");
        
    // const char delimiter[] = " ";

    // DynArray *strings = split_string(exp, delimiter);

    // for (size_t i = 0; i < dyn_array_count(strings); i++)
    // {
    //     printf("%s\n", dyn_array_element_at_index(strings, i));
    // }
    
	DynArray *infix = dyn_array_create(32);
    expression_to_infix(infix, "1 +  2d6 * 10");

	for (size_t i = 0; i < dyn_array_count(infix); i++)
    {
        printf("%s\n", dyn_array_element_at_index(infix, i));
    }

	printf("%d\n", could_be_number("1"));
	printf("%d\n", could_be_number("200"));
	printf("%d\n", could_be_number("1.1"));
	printf("%d\n", could_be_number("1e"));
	printf("%d\n", could_be_number("+"));
	printf("%d\n", could_be_number("."));
	printf("%d\n", could_be_number("10d6"));
	printf("%d\n", could_be_number("10..6"));
	printf("---\n");
	printf("%d\n", could_be_dice_collection("1"));
	printf("%d\n", could_be_dice_collection("200"));
	printf("%d\n", could_be_dice_collection("1.1"));
	printf("%d\n", could_be_dice_collection("1e"));
	printf("%d\n", could_be_dice_collection("+"));
	printf("%d\n", could_be_dice_collection("."));
	printf("%d\n", could_be_dice_collection("10d6"));
	printf("%d\n", could_be_dice_collection("d6"));
	printf("%d\n", could_be_dice_collection("1d96"));
	printf("%d\n", could_be_dice_collection("10dd6"));
	printf("%d\n", could_be_dice_collection("10d1d6"));
    return 0;
}