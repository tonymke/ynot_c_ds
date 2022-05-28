#ifndef YNOT_C_DS_COMMON_H
#define YNOT_C_DS_COMMON_H

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

void  *list_remove_node(list *lst, struct list_node *node);
size_t next_power_of_2 (size_t x);

#endif /* YNOT_C_DS_COMMON_H */
