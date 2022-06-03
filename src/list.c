#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "ynot_c_ds.h"

static struct list_node *list_find_node        (list *lst,
						void *value,
						int (*is_eq)(void *a, void *b));
static struct list_node *list_node_alloc       (void *value);
static void              list_node_free        (struct list_node *node,
						void (*free_value)(void *val));
static struct list_node *list_peek_at_node     (list* lst, size_t i);

list *list_alloc(void)
{
	static const struct list new_value = { 0, NULL, NULL };

	list *lst;
	lst = ynot_malloc(1, sizeof(*lst));
	if (lst == NULL) {
		perror("list_alloc: malloc");
		return NULL;
	}

	*lst = new_value;

	return lst;
}

int list_append(list *lst, void *value)
{
	if (lst == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	return list_insert(lst, lst->len, value);
}

void **list_find(list *lst, void *value, int (*is_eq)(void *a, void *b))
{
	struct list_node *node;

	node = list_find_node(lst, value, is_eq);
	if (node != NULL) {
		return &node->value;
	}

	return NULL;
}

struct list_node *list_find_node       (list *lst,
					void *value,
					int (*is_eq)(void *a, void *b))
{
	struct list_node *node;

	if (lst == NULL || is_eq == NULL) {
		return NULL;
	}

	for (node = lst->start; node != NULL; node = node->next) {
		if (is_eq(value, node->value)) {
			return node;
		}
	}

	return NULL;
}

void list_free_full(list *lst, void (*free_value)(void *value))
{
	struct list_node *node, *next_node;

	if (lst == NULL) {
		return;
	}

	node = lst->start;
	while (node != NULL) {
		next_node = node->next;
		list_node_free(node, free_value);
		node = next_node;
	}

	free(lst);
}

int list_insert(list *lst, size_t i, void *value)
{
	struct list_node *cur_node, *new_node;

	if (lst == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	if (i > lst->len) {
		return YNOT_EOUTOFRANGE;
	}

	new_node = list_node_alloc(value);
	if (new_node == NULL) {
		return YNOT_ENOMEM;
	}

	/* [ ... cur_node ... ] --> [ ... new_node cur_node ... ] */
	cur_node = list_peek_at_node(lst, i);
	new_node->next = cur_node;
	if (cur_node == NULL) {
		/* We are the new end node */
		new_node->prev = lst->end;
		if (lst->end != NULL) {
			lst->end->next = new_node;
		}
		lst->end = new_node;
	} else {
		new_node->prev = cur_node->prev;
		cur_node->prev = new_node;
		if (new_node->prev != NULL) {
			new_node->prev->next = new_node;
		}
	}

	if (new_node->prev == NULL) {
		/* We are the new start node */
		lst->start = new_node;
	}

	lst->len++;
	return YNOT_OK;
}

size_t list_len(list *lst)
{
	if (lst == NULL) {
		return 0;
	}

	return lst->len;
}

struct list_node *list_node_alloc(void *value)
{
	static const struct list_node new_value = { NULL, NULL, NULL };

	struct list_node *node;
	node = ynot_malloc(1, sizeof(*node));
	if (node == NULL) {
		perror("list_node_alloc: malloc");
		return NULL;
	}

	*node = new_value;
	node->value = value;

	return node;
}

void list_node_free(struct list_node *node, void (*free_value)(void *value))
{
	if (node != NULL && free_value != NULL) {
		free_value(node->value);
	}

	free(node);
}

void *list_peek(list *lst)
{
	if (lst == NULL) {
		return NULL;
	}

	return list_peek_at(lst, lst->len - 1);
}

void *list_peek_at(list *lst, size_t i)
{
	struct list_node *cursor;

	cursor = list_peek_at_node(lst, i);
	if (cursor == NULL) {
		return NULL;
	}

	return cursor->value;
}

struct list_node *list_peek_at_node(list* lst, size_t i)
{
	struct list_node *cursor;
	size_t cursor_i;

	if (lst == NULL || i >= lst->len) {
		return NULL;
	}

	if (i <= lst->len / 2) {
		for (
				cursor_i = 0, cursor = lst->start;
				cursor != NULL && cursor_i < i;
				cursor_i++, cursor = cursor->next) {
		}
	} else {
		for (
				cursor_i = lst->len-1, cursor = lst->end;
				cursor != NULL && cursor_i > i;
				cursor_i++, cursor = cursor->prev) {
		}
	}

	return cursor;
}

void *list_peek_left(list *lst)
{
	return list_peek_at(lst, 0);
}

void *list_pop(list *lst)
{
	if (lst == NULL) {
		return NULL;
	}
	return list_remove_at(lst, lst->len - 1);
}

void *list_pop_left(list *lst)
{
	return list_remove_at(lst, 0);
}

int list_prepend(list *lst, void *value)
{
	return list_insert(lst, 0, value);
}

void *list_remove_at(list *lst, size_t i)
{
	return list_remove_node(lst, list_peek_at_node(lst, i));
}

void *list_remove_node(list *lst, struct list_node *node)
{
	void *value;
	if (lst == NULL || node == NULL) {
		return NULL;
	}

	value = node->value;
	if (node->prev == NULL) {
		lst->start = node->next;
	} else {
		node->prev->next = node->next;
	}

	if (node->next == NULL) {
		lst->end = node->prev;
	} else {
		node->next->prev = node->prev;
	}

	lst->len--;
	list_node_free(node, NULL);

	return value;
}

void *list_remove_val(list *lst, void *value, int (*is_eq)(void *a, void *b))
{
	return list_remove_node(lst, list_find_node(lst, value, is_eq));
}
