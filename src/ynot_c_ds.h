#include <stdlib.h>

#ifndef YNOT_C_DS_H
#define YNOT_C_DS_H

struct list;

typedef struct list list;

list  *list_alloc    (void);
int    list_append   (list *lst, void *value);
void   list_free_full(list *lst, void (*free_value)(void *value));
void  *list_get      (list *lst, size_t i);
int    list_insert   (list *lst, void *value, size_t i);
size_t list_len      (list *lst);
void  *list_pop      (list *lst);
void  *list_pop_left (list *lst);
int    list_prepend  (list *lst, void *value);
void  *list_remove   (list *lst, size_t i);

#define list_free(l) (list_free_full((l), NULL))

#endif /* YNOT_C_DS_H */
