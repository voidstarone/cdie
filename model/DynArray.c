#include "DynArray.h"
#include <stdio.h>
#include <stdbool.h>

void quicksort(
	void **array, 
    size_t low_index, 
    size_t high_index,
	int (*compare)(void *, void *)
) {
	void *pivot_element = array[(low_index + high_index) / 2];
	size_t low = low_index;
	size_t high = high_index;
	void *tmp_element;

	while (low <= high) {
		while (compare(array[low], pivot_element) < 0) {
			low++;
		}
		while (compare(array[high], pivot_element) > 0) {
			high--;
		}
		if (low <= high) {
			tmp_element = array[low];
			array[low] = array[high];
			array[high] = tmp_element;
			low++;
			high = high == 0 ? 0 : high - 1;
		}
	}

	if (low_index < high) quicksort(array, low_index, high, compare);
	if (low < high_index) quicksort(array, low, high_index, compare);
}

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
	for (size_t i = 0; i < initial_size; i++) {
		a->array[i] = NULL;
	}
	return a;
}

size_t dyn_array_count(DynArray *a) {
	if (a == NULL) return 0;
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
	return a->array[--a->used];;
}

void *dyn_array_element_at_index(DynArray *a, size_t index) {
	if (index > a->used) {
		//printf("WARNING: dyn_array(%p) index(%zu -> %zu) out of bounds. Size=%zu, Used=%zu\n", a, index, a->size, a->used);
		return NULL;
	}
	return a->array[index];
}

void dyn_array_set_element_at_index(DynArray *a, size_t index, void *new_element) {
	size_t real_index = (index < 0) ? a->used - index : index;
	
	if (real_index >= a->size) {
		a->size = real_index + 1;
		void **tmp_array = (void **) realloc(a->array, a->size * sizeof(void *));
		if (tmp_array == NULL) { return; }
		a->array = tmp_array;
		for (size_t i = a->used+1; i < index; i++) {
			a->array[i] = NULL;
		}
	}
	a->array[index] = new_element;
	a->used = index + 1;
}

void dyn_array_filter_out_null(DynArray *a) {
    size_t write_index = 0;

    for (size_t read_index = 0; read_index < a->used; read_index++) {
        if (a->array[read_index] != NULL) {
            a->array[write_index++] = a->array[read_index];
        }
    }

    a->used = write_index;
}

void dyn_array_sort_in_place(
	DynArray *a, 
	int (*compare)(void *, void *)
) {
	if (a->used == 0) { return; }
	quicksort(a->array, 0, a->used-1, compare);
}

void dyn_array_map_in_place(
	DynArray *a, 
	void *(*callback)(void *)
) {
	for (size_t i = 0; i < a->used; i++) {
		callback(a->array[i]);
	}
}

bool dyn_array_does_match(
	DynArray *a, 
	bool (*does_match)(void *)
) {
	for (size_t i = 0; i < a->used; i++) {
		if (does_match(a->array[i])) {
			return true;
		}
	}
	return false;
}


bool dyn_array_contains(
	DynArray *a, 
	bool (*compare)(void *, void *),
	void *element
) {
	for (size_t i = 0; i < a->used; i++) {
		if (compare(a->array[i], element)) {
			return true;
		}
	}
	return false;
}

void dyn_array_reverse_in_place(DynArray *a) {
    void *tmp;
	size_t forward_index = 0, backward_index = a->used - 1;
	void **array = a->array;
	while (forward_index < backward_index) {
		tmp = array[forward_index];
		array[forward_index] = array[backward_index];
		array[backward_index] = tmp;
		forward_index++;
		backward_index--;
	}
}

void dyn_array_print(DynArray *a, void (*print_element)(void *)) {
	printf("DynArray(used: %zu, size: %zu): [", a->used, a->size);
    for (size_t i = 0; i < dyn_array_count(a); i++)
    {
        if (i != 0) {
            printf(", ");
        }
        print_element(dyn_array_element_at_index(a, i));
    }
    printf("]\n");
}

void dyn_array_free(DynArray *a) {
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
	free(a);
	a = NULL;
}

	// shrink if not much is used
	// if (a->used < (a->size / 4)) {
	// 	a->size/= 2;
	// 	void **tmp_array = realloc(a->array, a->size * sizeof(void *));
	// 	if (tmp_array == NULL) {
	// 		printf("WARNING: dyn_array(%p) failed to shrink\n", a);
	// 	} else {
	// 		a->array = tmp_array;
	// 	}
	// }