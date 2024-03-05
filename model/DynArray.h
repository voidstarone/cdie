#include <stdlib.h>

#ifndef DYN_ARRAY_H_FA033981
#define DYN_ARRAY_H_FA033981

typedef struct {
  void **array;
  size_t used;
  size_t size;
} DynArray;

DynArray *dyn_array_create(size_t initial_size);

size_t dyn_array_count(DynArray *a);

void dyn_array_push(DynArray *a, void *element);

void *dyn_array_peek(DynArray *a);

void *dyn_array_pop(DynArray *a);

void *dyn_array_element_at_index(DynArray *a, size_t index);

void dyn_array_set_element_at_index(DynArray *a, size_t index, void *new_element);

void dyn_array_free(DynArray *a);

#endif /* DYN_ARRAY_H_FA033981 */