#include <stdio.h>
#include <stdlib.h>

#include "ynot_c_ds.h"

struct queue {
	size_t max_size;
	list *lst;
	void (*free_value_fn)(void *value);
};

queue *queue_alloc(size_t max_size, void (*free_value)(void *value))
{
	queue *q = malloc(sizeof(*q));
	if (q == NULL) {
		perror("queue_alloc: queue malloc");
		return NULL;
	}

	q->max_size = max_size;
	q->free_value_fn = free_value;

	q->lst = list_alloc();
	if (q->lst == NULL) {
		perror("queue_alloc: list malloc");
		return NULL;
	}

	return q;
}

int queue_append(queue *q, void *value)
{
	if (q == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	if (list_len(q->lst) >= q->max_size)
	{
		void *removed = list_pop_left(q->lst);
		if (removed != NULL && q->free_value_fn != NULL) {
			q->free_value_fn(removed);
		}
	}

	return list_append(q->lst, value);
}

void **queue_find(queue *q, void *value, int (*is_eq)(void *a, void *b))
{
	if (q == NULL) {
		return NULL;
	}

	return list_find(q->lst, value, is_eq);
}

void queue_free(queue *q)
{
	if (q == NULL) {
		return;
	}

	list_free_full(q->lst, q->free_value_fn);
	free(q);
}

size_t queue_len(queue *q)
{
	if (q == NULL) {
		return 0;
	}

	return list_len(q->lst);
}

size_t queue_max_size(queue *q)
{
	if (q == NULL) {
		return 0;
	}

	return q->max_size;
}

void *queue_peek(queue *q)
{
	if (q == NULL) {
		return NULL;
	}

	return list_peek(q->lst);
}

void *queue_peek_at(queue *q, size_t i)
{
	if (q == NULL) {
		return NULL;
	}

	return list_peek_at(q->lst, i);
}

void *queue_peek_left(queue *q)
{
	if (q == NULL) {
		return NULL;
	}

	return list_peek_left(q->lst);
}

void *queue_pop(queue *q)
{
	if (q == NULL) {
		return NULL;
	}

	return list_pop(q->lst);
}

void *queue_pop_left(queue *q)
{
	if (q == NULL) {
		return NULL;
	}

	return list_pop_left(q->lst);
}

int queue_prepend(queue *q, void *value)
{
	if (q == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	if (list_len(q->lst) >= q->max_size) {
		void *removed = list_pop(q->lst);
		if (removed != NULL && q->free_value_fn != NULL) {
			q->free_value_fn(removed);
		}
	}

	return list_prepend(q->lst, value);
}
