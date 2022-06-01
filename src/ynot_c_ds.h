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
struct queue;

typedef struct array array;
typedef struct list list;
typedef struct map map;
typedef struct queue queue;
typedef struct array pqueue;

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
void   array_swap            (array *arr, size_t a_i, size_t b_i);
int    array_trim_capacity   (array *arr);

size_t hash_str_djb2 (void *str);

list  *list_alloc     (void);
int    list_append    (list *lst, void *value);
void **list_find      (list *lst, void *value, int (*is_eq)(void *a, void *b));
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
void  *list_remove_val(list *lst, void *value, int (*is_eq)(void *a, void *b));

map    *map_alloc_capacity     (size_t (*hash_fn)(void* key),
				int (*key_eq)(void *key_a, void*key_b),
				size_t initial_capacity);
void    map_free_full          (map *mp,
				void (*free_key)(void *key),
				void (*free_value)(void *value));
int     map_ensure_capacity    (map *mp, size_t min_capacity);
void   *map_get                (map *mp, void *key);
size_t  map_len                (map *mp);
void   *map_remove             (map *mp, void *key);
void   *map_set                (map *mp, void *key, void *value);

void  pqueue_free_full(pqueue *pq, void (*free_value)(void *value));
int   pqueue_push     (pqueue *pq, int priority, void *val);
void *pqueue_peek     (pqueue *pq);
void *pqueue_pop      (pqueue *pq);

int ptr_eq(void *a, void *b);

queue *queue_alloc     (size_t max_size, void (*free_value)(void *value));
int    queue_append    (queue *q, void *value);
void **queue_find      (queue *q, void *value, int (*is_eq)(void *a, void *b));
void   queue_free      (queue *q);
size_t queue_len       (queue *q);
size_t queue_max_size  (queue *q);
void  *queue_peek      (queue *q);
void  *queue_peek_at   (queue *q, size_t i);
void  *queue_peek_left (queue *q);
void  *queue_pop       (queue *q);
void  *queue_pop_left  (queue *q);
int    queue_prepend   (queue *q, void *value);

int str_eq(void *a, void *b);

#define array_alloc()                   (array_alloc_capacity(0))
#define array_free(A)                   (array_free_full((A), NULL))
#define list_free(L)                    (list_free_full((L), NULL))
#define map_alloc(H,E)                  (map_alloc_capacity((H), (E), 0))
#define map_free(MP)                    (map_free_full((MP), NULL, NULL))
#define pqueue_alloc()                  (array_alloc())
#define pqueue_free(PQ)                 (pqueue_free_full((PQ), NULL))
#define pqueue_len(PQ)                  (array_len((PQ)))
#define pqueue_trim_capacity(PQ)        (array_trim_capacity((PQ)))

#endif /* YNOT_C_DS_H */
