#include <assert.h>
#include <stdlib.h>

#include "list.h"

void test_list_append()
{
	struct list *l;
	int a_data, b_data;

	l = NULL;
	a_data = 1;
	b_data = 2;
	l = list_append(l, &a_data);
	l = list_append(l, &b_data);


	assert(l->data == &b_data);
	assert(l->next == NULL);

	assert(l->prev->data == &a_data);
	assert(l->prev->next == l);
	assert(l->prev->prev == NULL);

	list_free(l);
}

void test_list_insert()
{
	struct list *a, *b, *c;
	int a_data, b_data, c_data;

	a_data = 1;
	b_data = 2;
	c_data = 3;

	a = list_insert(NULL, &a_data, 1);
	assert(a->data == &a_data);
	assert(a->prev == NULL);
	assert(a->next == NULL);

	c = list_insert(a, &c_data, 50);
	assert(c->data == &c_data);
	assert(c->prev == a);
	assert(c->next == NULL);
	assert(a->prev == NULL);
	assert(a->next == c);

	b = list_insert(a, &b_data, 1);
	assert(b->data == &b_data);
	assert(b->prev == a);
	assert(b->next == c);
	assert(a->prev == NULL);
	assert(a->next == b);
	assert(c->prev == b);
	assert(c->next == NULL);

	list_free(a);
}

void test_list_lifecycle()
{
	struct list *l;

	l = list_alloc();
	list_free(l);

	l = list_alloc();
	l->data = malloc(sizeof(int));
	*(int*)l->data = 400;
	list_free_full(l, free);
}

void test_list_prepend()
{
	struct list *l;
	int a_data, b_data;

	l = NULL;
	a_data = 1;
	b_data = 2;
	l = list_prepend(l, &b_data);
	l = list_prepend(l, &a_data);

	assert(l->data == &a_data);
	assert(l->prev == NULL);

	assert(l->next->data == &b_data);
	assert(l->next->prev == l);
	assert(l->next->next == NULL);

	list_free(l);
}


int main()
{
	test_list_append();
	test_list_insert();
	test_list_lifecycle();
	test_list_prepend();
	return 0;
}
