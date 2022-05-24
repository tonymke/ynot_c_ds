#include <stdlib.h>

#ifndef YNOT_C_DS_H
#define YNOT_C_DS_H

enum ynot_return_status {
	YNOT_OK,

	YNOT_EINVALIDPARAM,
	YNOT_ENOMEM,
	YNOT_EOUTOFRANGE
};

struct list;

typedef struct list list;

list  *list_alloc     (void);
int    list_append    (list *lst, void *value);
void   list_free_full (list *lst, void (*free_value)(void *value));
int    list_insert    (list *lst, void *value, size_t i);
size_t list_len       (list *lst);
void  *list_peek      (list *lst);
void  *list_peek_at   (list *lst, size_t i);
void  *list_peek_left (list *lst);
void  *list_pop       (list *lst);
void  *list_pop_left  (list *lst);
int    list_prepend   (list *lst, void *value);
void  *list_remove_at (list *lst, size_t i);

#define list_free(L) (list_free_full((L), NULL))

#endif /* YNOT_C_DS_H */
