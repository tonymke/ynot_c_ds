#ifndef YNOT_C_DS_COMMON_H
#define YNOT_C_DS_COMMON_H

#include "ynot_c_ds.h"

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)-1)
#endif /* SIZE_MAX */

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

void  *list_remove_node(list *lst, struct list_node *node);
size_t next_power_of_2 (size_t x);
void  *ynot_malloc(size_t nmem, size_t n);
void  *ynot_realloc(void *ptr, size_t nmem, size_t n);

#endif /* YNOT_C_DS_COMMON_H */
