#include <stdio.h>
#include <stdlib.h>

#include "ynot_c_ds.h"

struct pqueue_node {
	int priority;
	void *value;
};

static int node_cmp(struct pqueue_node *a, struct pqueue_node *b);
static size_t parent_index(size_t i);
static void pqueue_position_for_heap(pqueue *pq, size_t i);

int node_cmp(struct pqueue_node *a, struct pqueue_node *b)
{
	if (a->priority == b->priority) {
		return 0;
	}
	return a->priority > b->priority ? 1 : -1;
}

size_t parent_index(size_t i) {
	return (i - 1) / 2;
}

void pqueue_free_full(pqueue *pq, void (*free_value)(void *value))
{
	if (pq == NULL) {
		return;
	}

	if (free_value == NULL) {
		array_free_full(pq, free);
		return;
	}

	{
		size_t i;
		size_t len = array_len(pq);
		for (i = 0; i < len; i++) {
			struct pqueue_node *node = (struct pqueue_node *)array_get(pq, i);
			free_value(node->value);
			free(node);
		}
	}
	free(pq);
}

void *pqueue_peek(pqueue *pq)
{
	struct pqueue_node *node = array_get(pq, 0);
	if (node == NULL) {
		return NULL;
	}

	return node->value;
}

void *pqueue_pop(pqueue *pq)
{
	size_t len = pqueue_len(pq);
	struct pqueue_node *node;
	void *popped_value;

	if (len == 0) {
		return NULL;
	}

	node = array_get(pq, 0);
	popped_value = node->value;
	free(node);

	array_swap(pq, 0, len - 1);
	array_remove_at(pq, len - 1);

	pqueue_position_for_heap(pq, 0);
	return popped_value;
}

void pqueue_position_for_heap(pqueue *pq, size_t i)
{
	size_t len = pqueue_len(pq);
	int started = 0;
	size_t largest_i = i;

	while (i != largest_i || started == 0) {
		struct pqueue_node *largest_node = (struct pqueue_node *)array_get(
				pq, largest_i);

		size_t left_child_i = largest_i * 2 + 1;
		struct pqueue_node *left_child_node = (struct pqueue_node *)array_get(
				pq, left_child_i);

		size_t right_child_i = largest_i * 2 + 2;
		struct pqueue_node *right_child_node = (struct pqueue_node *)array_get(
				pq, right_child_i);

		if (len == 0 || i >= len) {
			return;
		}

		if (!started) {
			started = 1;
		}

		largest_node = array_get(pq, largest_i);

		if (left_child_node != NULL) {
			if (node_cmp(left_child_node, largest_node) > 0) {
				largest_node = left_child_node;
				largest_i = left_child_i;
			}
		}

		if (right_child_node != NULL) {
			if (node_cmp(right_child_node, largest_node) > 0) {
				largest_node = right_child_node;
				largest_i = right_child_i;
			}
		}

		if (largest_i != i) {
			array_swap(pq, largest_i, i);
		}
	}
}

int pqueue_push(pqueue *pq, int priority, void *val)
{
	if (pq == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	/* alloc a new node and place it and the end of the array */
	{
		struct pqueue_node *node = malloc(sizeof(*node));
		if (node == NULL) {
			perror("pqueue_push: node malloc");
			return YNOT_ENOMEM;
		}
		node->priority = priority;
		node->value = val;

		{
			int err;
			err = array_add(pq, node);
			if (err != YNOT_OK) {
				free(node);
				return err;
			}
		}
	}

	/* continually swap with our parents until we find one that's bigger
	 * than us. */
	{
		size_t i, n;
		n = array_len(pq);
		i = n - 1;
		while (
				i != 0
				&& node_cmp(
					array_get(pq, parent_index(i)),
					array_get(pq, i)
				) == -1
		) {
			array_swap(pq, i, parent_index(i));
			i = parent_index(i);
		}
	}

	return YNOT_OK;
}
