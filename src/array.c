#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "ynot_c_ds.h"

struct array {
	size_t capacity;
	size_t len;

	void **data;
};

int array_add(array *arr, void *value)
{
	if (arr == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	return array_insert(arr, arr->len, value);
}

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

void array_clear(array *arr)
{
	if (arr == NULL || arr->len == 0) {
		return;
	}

	arr->len = 0;
}

int array_ensure_capacity(array *arr, size_t min_capacity)
{
	void **new_data = NULL;

	if (arr == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	if (arr->capacity >= min_capacity) {
		return YNOT_OK;
	}

	min_capacity = next_power_of_2(min_capacity);
	new_data = realloc(
		arr->data,
		sizeof(*arr->data) * min_capacity
	);
	if (new_data == NULL) {
		perror("array_ensure_capacity: realloc");
		return YNOT_ENOMEM;
	}
	arr->data = new_data;

	return YNOT_OK;
}

void array_free_full(array *arr, void (*free_value)(void *value))
{
	if (arr == NULL) {
		return;
	}

	if (arr->data != NULL) {
		if (free_value != NULL) {
			size_t i;

			for (i = 0; i < arr->len; i++) {
				free_value(arr->data[i]);
			}
		}
		free(arr->data);
		arr->data = NULL;
	}
	free(arr);
}

void *array_get(array *arr, size_t i)
{
	if (arr == NULL || i >= arr->len) {
		return NULL;
	}

	return arr->data[i];
}

int array_insert(array *arr, size_t i, void *value)
{
	int err;
	if (arr == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	if (i > arr->len) {
		return YNOT_EOUTOFRANGE;
	}

	err = array_ensure_capacity(arr, arr->len+1);
	if (err != YNOT_OK) {
		return err;
	}

	if (i < arr->len) {
		size_t copy_i;
		for (copy_i = arr->len; copy_i > i; copy_i--) {
			arr->data[copy_i] = arr->data[copy_i-1];
		}
	}

	arr->data[i] = value;
	arr->len++;

	return YNOT_OK;
}

size_t array_len(array *arr) {
	if (arr == NULL) {
		return 0;
	}

	return arr->len;
}

void *array_remove_at(array *arr, size_t i)
{
	void *removed;

	if (arr == NULL) {
		return NULL;
	}

	if (i >= arr->len) {
		return NULL;
	}

	removed = arr->data[i];
	if (i < arr->len) {
		size_t copy_i;

		for(copy_i = i; copy_i < arr->len-1; copy_i++) {
			arr->data[copy_i] = arr->data[copy_i + 1];
		}
	}

	arr->len--;
	return removed;
}

void *array_set(array *arr, size_t i, void *value)
{
	void *replaced;

	if (arr == NULL) {
		return NULL;
	}

	if (i >= arr->len) {
		return NULL;
	}

	replaced = arr->data[i];
	arr->data[i] = value;

	return replaced;
}

int array_trim_capacity(array *arr)
{
	void *new_data;
	if (arr == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	if (arr->len == arr->capacity) {
		return YNOT_OK;
	}

	if (arr->len == 0) {
		free(arr->data);
		arr->data = NULL;
		arr->capacity = 0;
		return YNOT_OK;
	}

	new_data = realloc(
		arr->data,
		sizeof(*arr->data) * arr->len
	);
	if (new_data == NULL) {
		perror("array_trim_capacity: realloc");
		return YNOT_ENOMEM;
	}

	arr->data = new_data;
	arr->capacity = arr->len;

	return YNOT_OK;
}
