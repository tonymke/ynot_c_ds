#ifndef YNOT_C_DS_H
#define YNOT_C_DS_H

#include <stdlib.h>

enum ynot_return_status {
	YNOT_OK,

	YNOT_EINVALIDPARAM,
	YNOT_ENOMEM,
	YNOT_EOUTOFRANGE
};

struct array;
struct list;

typedef struct array array;
typedef struct list list;

array *array_alloc_capacity (size_t initial_capacity);
int    array_append         (array *arr, void *value);
void   array_free_full      (array *arr, void (*free_value)(void *value));
int    array_insert         (array *arr, void *value, size_t i);
size_t array_len            (array *arr);
void  *array_peek           (array *arr);
void  *array_peek_at        (array *arr, size_t i);
void  *array_peek_left      (array *arr);
void  *array_pop            (array *arr);
void  *array_pop_left       (array *arr);
int    array_prepend        (array *arr, void *value);
void  *array_remove_at      (array *arr, size_t i);
void  *array_set            (array *arr, void *value, size_t i);

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

#define array_alloc()   (array_alloc_capacity(0))
#define array_get(A, I) (array_peek_at((A) (I))
#define array_free(A)   (array_free_full((A), NULL))
#define list_free(L)    (list_free_full((L), NULL))

#endif /* YNOT_C_DS_H */
