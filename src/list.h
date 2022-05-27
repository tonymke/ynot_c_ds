#ifndef YNOT_C_DS_LIST_H
#define YNOT_C_DS_LIST_H

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

#endif /* YNOT_C_DS_LIST_H */
