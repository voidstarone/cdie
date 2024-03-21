#include "DynArray.h"
#include <stdio.h>
#include <stdbool.h>

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