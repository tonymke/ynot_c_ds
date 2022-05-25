#ifndef YNOT_C_DS_H
#define YNOT_C_DS_H

#include <stdlib.h>

enum ynot_errors {
	YNOT_OK = 0,

	YNOT_EUNKNOWN      = -1,
	YNOT_EINVALIDPARAM = -2,
	YNOT_ENOMEM        = -3,
	YNOT_EOUTOFRANGE   = -4
};

struct array;
struct list;

typedef struct array array;
typedef struct list list;

int    array_add             (array *arr, void *value);
array *array_alloc_capacity  (size_t initial_capacity);
void   array_clear           (array *arr);
void   array_free_full       (array *arr, void (*free_value)(void *value));
int    array_insert          (array *arr, size_t i, void *value);
int    array_ensure_capacity (array *arr, size_t capacity);
size_t array_len             (array *arr);
void  *array_get             (array *arr, size_t i);
void  *array_remove_at       (array *arr, size_t i);
void  *array_set             (array *arr, size_t i, void *value);
int    array_trim            (array *arr);

list  *list_alloc     (void);
int    list_append    (list *lst, void *value);
void   list_free_full (list *lst, void (*free_value)(void *value));
int    list_insert    (list *lst, size_t i, void *value);
size_t list_len       (list *lst);
void  *list_peek      (list *lst);
void  *list_peek_at   (list *lst, size_t i);
void  *list_peek_left (list *lst);
void  *list_pop       (list *lst);
void  *list_pop_left  (list *lst);
int    list_prepend   (list *lst, void *value);
void  *list_remove_at (list *lst, size_t i);

#define array_alloc()       (array_alloc_capacity(0))
#define array_free(A)       (array_free_full((A), NULL))
#define list_free(L)        (list_free_full((L), NULL))

#endif /* YNOT_C_DS_H */
