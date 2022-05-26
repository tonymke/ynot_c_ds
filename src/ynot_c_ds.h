#ifndef YNOT_C_DS_H
#define YNOT_C_DS_H

#include <stdlib.h>

enum ynot_error {
	YNOT_OK = 0,

	YNOT_EINVALIDPARAM = -1,
	YNOT_ENOMEM        = -2,
	YNOT_EOUTOFRANGE   = -3
};

struct array;
struct list;
struct map;

typedef struct array array;
typedef struct list list;
typedef struct map map;


int    array_add             (array *arr, void *value);
array *array_alloc_capacity  (size_t initial_capacity);
void   array_clear           (array *arr);
int    array_ensure_capacity (array *arr, size_t min_capacity);
void   array_free_full       (array *arr, void (*free_value)(void *value));
void  *array_get             (array *arr, size_t i);
int    array_insert          (array *arr, size_t i, void *value);
size_t array_len             (array *arr);
void  *array_remove_at       (array *arr, size_t i);
void  *array_set             (array *arr, size_t i, void *value);
int    array_trim_capacity   (array *arr);

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

map *map_alloc_capacity    (size_t (*hash_fn)(void* key),
							int (*key_eq)(void *key_a, void*key_b),
							size_t initial_capacity);
void map_clear             (map *mp);
void map_free_full         (map *mp,
							void (*free_key)(void *key),
							void (*free_value)(void *value));
void *map_get              (map *mp, void *key);
void *map_remove           (map *mp, void *key);
void *map_set              (map *mp, void *key, void *value);

#define array_alloc()       (array_alloc_capacity(0))
#define array_free(A)       (array_free_full((A), NULL))
#define list_free(L)        (list_free_full((L), NULL))
#define map_alloc(HASH,EQ)  (map_alloc_capacity((HASH), (EQ), 0))
#define map_free(MP)        (map_free_full((MP), NULL, NULL))

#endif /* YNOT_C_DS_H */
