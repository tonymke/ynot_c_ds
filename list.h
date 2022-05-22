#include <stdlib.h>

#ifndef YNOT_C_DS_LIST_H
#define YNOT_C_DS_LIST_H

struct list {
	struct list *next;
	struct list *prev;

	void *data;
};

struct list* list_alloc();
#define list_free(l) (list_free_full((l), NULL))
void list_free_full(struct list *l, void (*data_free)(void *data));

size_t list_len(struct list *l);
struct list *list_end(struct list *l);
struct list *list_start(struct list *l);

struct list *list_append(struct list *l, void *data);
struct list *list_insert(struct list *l, void *data, size_t i);
struct list *list_prepend(struct list *l, void *data);

struct list *list_delete(struct list *l);
void *list_pop(struct list *l);
void *list_pop_left(struct list *l);

#endif /* YNOT_C_DS_LIST_H */
