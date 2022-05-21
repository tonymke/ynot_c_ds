#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct list* list_alloc()
{
	struct list *l;

	l = malloc(sizeof(struct list));
	l->next = NULL;
	l->prev = NULL;
	l->data = NULL;
	return l;
}

struct list *list_append(struct list *l, void *data)
{
	if (l == NULL) {
		l = list_alloc();
		l->data = data;
		return l;
	}

	while (l->next != NULL)
		l = l->next;

	l->next = list_alloc();
	l->next->prev = l;
	l->next->data = data;
	return l->next;
}

struct list *list_end(struct list *l)
{
	if (l == NULL)
		return l;

	while (l != NULL && l->next != NULL)
		l = l->next;

	return l;
}

void list_free_full(struct list *l, void (*data_free)(void *data))
{
	struct list *buf, *left_entry;
	if (l == NULL)

		return;

	left_entry = l->prev;
	while (l != NULL) {
		if (data_free != NULL)
			data_free(l->data);

		buf = l;
		l = l->next;
		free(buf);
	}

	l = left_entry;
	while (l != NULL) {
		if (data_free != NULL)
			data_free(l->data);

		buf = l;
		l = l->prev;
		free(buf);
	}
}

struct list *list_insert(struct list *l, void *data, size_t i)
{
	size_t j;
	struct list *new;

	new = list_alloc();
	new->data = data;

	j = 0;
	while (l != NULL && j < i) {
		new->prev = l;
		new->next = l = l->next;
		j++;
	}

	if (new->next != NULL)
		new->next->prev = new;
	if (new->prev != NULL)
		new->prev->next = new;

	return new;
}

struct list *list_prepend(struct list *l, void *data)
{
	if (l == NULL) {
		l = list_alloc();
		l->data = data;
		return l;
	}

	while (l->prev != NULL)
		l = l->prev;

	l->prev = list_alloc();
	l->prev->next = l;
	l->prev->data = data;

	return l->prev;
}

struct list *list_start(struct list *l)
{
	if (l == NULL)
		return NULL;

	while (l->prev != NULL)
		l = l->prev;

	return l;
}
