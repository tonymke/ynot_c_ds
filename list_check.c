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

void test_list_delete()
{
	struct list *a, *b, *c;
	int a_data, b_data, c_data;

	assert(list_delete(NULL) == NULL);
	assert(list_delete(list_alloc()) == NULL);

	a_data = 1;
	b_data = 2;
	c_data = 3;

	a = list_append(NULL, &a_data);
	b = list_append(a, &b_data);
	c = list_append(b, &c_data);

	assert(list_delete(b) == a);
	assert(a->next == c);
	assert(list_len(a) == 2);

	assert(list_delete(a) == c);
	assert(list_len(c) == 1);
}

void test_list_end()
{
	struct list *l, *start;
	size_t i;
	int end_data, other_data;

	start = l = NULL;
	end_data = 1234;
	other_data = 5678;

	start = l = list_end(l);
	assert(l == NULL);
	assert(start == NULL);

	start = l = list_append(l, &other_data);
	for (i = 0; i < 4; i++) {
		l = list_append(l, &other_data);
		assert(list_end(start) == l);
	}
	l = list_append(l, &end_data);
	assert((list_end(l))->data == &end_data);

	assert((list_end(start))->data == &end_data);

	list_free(l);
}

void test_list_get()
{
	struct list *a, *b, *c;

	assert(list_get(NULL, 0) == NULL);

	a = list_append(NULL, NULL);
	assert(list_get(a, 0) == a);
	assert(list_get(a, 1) == NULL);

	b = list_append(a, NULL);
	assert(list_get(a, 0) == a);
	assert(list_get(a, 1) == b);
	assert(list_get(a, 2) == NULL);

	assert(list_get(b, 0) == a);
	assert(list_get(b, 1) == b);
	assert(list_get(b, 2) == NULL);

	c = list_append(b, NULL);
	assert(list_get(a, 0) == a);
	assert(list_get(a, 1) == b);
	assert(list_get(a, 2) == c);
	assert(list_get(a, 3) == NULL);

	assert(list_get(b, 0) == a);
	assert(list_get(b, 1) == b);
	assert(list_get(b, 2) == c);
	assert(list_get(b, 3) == NULL);

	assert(list_get(c, 0) == a);
	assert(list_get(c, 1) == b);
	assert(list_get(b, 2) == c);
	assert(list_get(b, 3) == NULL);

	list_free(a);
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

void test_list_len() {
	struct list *start, *mid, *end;

	assert(list_len(NULL) == 0);
	start = list_append(NULL, NULL);
	assert(list_len(start) == 1);

	mid = list_append(start, NULL);
	assert(list_len(start) == 2);
	assert(list_len(mid) == 2);
	end = list_append(mid, NULL);

	assert(list_len(start) == 3);
	assert(list_len(end) == 3);
	assert(list_len(mid) == 3);

	list_free(start);
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

void test_list_pop()
{
	struct list *l;
	int a_data, b_data, c_data;

	l = NULL;
	assert(list_pop(l) == NULL);

	a_data = 1;
	l = list_append(l, &a_data);
	b_data = 2;
	list_append(l, &b_data);
	c_data = 3;
	list_append(l, &c_data);

	assert(list_pop(l) == &c_data);
	assert(list_len(l) == 2);
	assert(list_pop(l) == &b_data);
	assert(list_len(l) == 1);
	assert(list_pop(l) == &a_data);
}

void test_list_pop_left()
{
	struct list *l;
	int a_data, b_data, c_data;

	l = NULL;
	assert(list_pop(l) == NULL);

	a_data = 1;
	l = list_append(l, &a_data);
	b_data = 2;
	l = list_append(l, &b_data);
	c_data = 3;
	l = list_append(l, &c_data);

	assert(list_pop_left(l) == &a_data);
	assert(list_len(l) == 2);
	assert(list_pop_left(l) == &b_data);
	assert(list_len(l) == 1);
	assert(list_pop_left(l) == &c_data);
}

void test_list_start()
{
	struct list *l;
	size_t i;
	int start_data, other_data;

	l = NULL;
	start_data = 1234;
	other_data = 5678;

	assert(list_start(l) == NULL);

	l = list_append(NULL, &start_data);
	assert((list_start(l))->data == &start_data);
	for (i = 0; i < 5; i++) {
		l = list_append(l, &other_data);
	}

	assert((list_start(l))->data == &start_data);
	list_free(l);
}

int main()
{
	test_list_append();
	test_list_delete();
	test_list_end();
	test_list_get();
	test_list_insert();
	test_list_len();
	test_list_lifecycle();
	test_list_pop();
	test_list_pop_left();
	test_list_prepend();
	test_list_start();

	return EXIT_SUCCESS;
}
