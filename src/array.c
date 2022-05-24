#include <stdio.h>
#include <stdlib.h>

#include "ynot_c_ds.h"

struct array {
	size_t capacity;
	size_t elements;

	void **data;
};

array *array_alloc_capacity(size_t initial_capacity)
{
	static const array new_array = { 0, 0, NULL };
	array *arr;
	arr = malloc(sizeof(*arr));
	if (arr == NULL) {
		perror("array_alloc_capacity: struct malloc");
		return NULL;
	}

	*arr = new_array;

	if (initial_capacity > 0) {
		arr->capacity = initial_capacity;
		arr->data = malloc(sizeof(*arr->data) * initial_capacity);
		if (arr->data == NULL) {
			perror("array_alloc_capacity: array malloc");
			free(arr);
			return NULL;
		}
	}

	return arr;
}

void array_free_full(array *arr, void (*free_value)(void *value))
{
	size_t i;

	if (arr == NULL) {
		return;
	}

	if (arr->data != NULL) {
		if (free_value != NULL) {
			for (i = 0; i < arr->elements; i++) {
				free_value(arr->data[i]);
			}
		}
		free(arr->data);
	}
	free(arr);
}
