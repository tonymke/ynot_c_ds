#include <stdio.h>
#include <stdlib.h>

#include "ynot_c_ds.h"

struct list {
	size_t len;

	struct list_node *start;
	struct list_node *end;
};

struct list_node {
	void *value;

	struct list_node *prev;
	struct list_node *next;
};

static
void list_node_free(struct list_node *node, void (*free_value)(void *value));

list *list_alloc(void)
{
	list *lst;
	lst = malloc(sizeof(*lst));
	if (lst == NULL) {
		perror("list_alloc: malloc");
		return NULL;
	}

	lst->start = lst->end = NULL;

	return lst;
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

void list_node_free(struct list_node *node, void (*free_value)(void *value))
{
	if (node != NULL && free_value != NULL) {
		free_value(node->value);
	}

	free(node);
}
