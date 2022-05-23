#include <stdlib.h>

#include <check.h>

#include "../src/list.h"

START_TEST(test_list_append)
{
	struct list *l;
	int a_data, b_data;

	l = NULL;
	a_data = 1;
	b_data = 2;
	l = list_append(l, &a_data);
	l = list_append(l, &b_data);


	ck_assert(l->data == &b_data);
	ck_assert(l->next == NULL);

	ck_assert(l->prev->data == &a_data);
	ck_assert(l->prev->next == l);
	ck_assert(l->prev->prev == NULL);

	list_free(l);
}
END_TEST

START_TEST(test_list_delete)
{
	struct list *a, *b, *c;
	int a_data, b_data, c_data;

	ck_assert(list_delete(NULL) == NULL);
	ck_assert(list_delete(list_alloc()) == NULL);

	a_data = 1;
	b_data = 2;
	c_data = 3;

	a = list_append(NULL, &a_data);
	b = list_append(a, &b_data);
	c = list_append(b, &c_data);

	ck_assert(list_delete(b) == a);
	ck_assert(a->next == c);
	ck_assert(list_len(a) == 2);

	ck_assert(list_delete(a) == c);
	ck_assert(list_len(c) == 1);
}
END_TEST

START_TEST(test_list_end)
{
	struct list *l, *start;
	size_t i;
	int end_data, other_data;

	start = l = NULL;
	end_data = 1234;
	other_data = 5678;

	start = l = list_end(l);
	ck_assert(l == NULL);
	ck_assert(start == NULL);

	start = l = list_append(l, &other_data);
	for (i = 0; i < 4; i++) {
		l = list_append(l, &other_data);
		ck_assert(list_end(start) == l);
	}
	l = list_append(l, &end_data);
	ck_assert((list_end(l))->data == &end_data);

	ck_assert((list_end(start))->data == &end_data);

	list_free(l);
}
END_TEST

START_TEST(test_list_get)
{
	struct list *a, *b, *c;

	ck_assert(list_get(NULL, 0) == NULL);

	a = list_append(NULL, NULL);
	ck_assert(list_get(a, 0) == a);
	ck_assert(list_get(a, 1) == NULL);

	b = list_append(a, NULL);
	ck_assert(list_get(a, 0) == a);
	ck_assert(list_get(a, 1) == b);
	ck_assert(list_get(a, 2) == NULL);

	ck_assert(list_get(b, 0) == a);
	ck_assert(list_get(b, 1) == b);
	ck_assert(list_get(b, 2) == NULL);

	c = list_append(b, NULL);
	ck_assert(list_get(a, 0) == a);
	ck_assert(list_get(a, 1) == b);
	ck_assert(list_get(a, 2) == c);
	ck_assert(list_get(a, 3) == NULL);

	ck_assert(list_get(b, 0) == a);
	ck_assert(list_get(b, 1) == b);
	ck_assert(list_get(b, 2) == c);
	ck_assert(list_get(b, 3) == NULL);

	ck_assert(list_get(c, 0) == a);
	ck_assert(list_get(c, 1) == b);
	ck_assert(list_get(b, 2) == c);
	ck_assert(list_get(b, 3) == NULL);

	list_free(a);
}
END_TEST

START_TEST(test_list_insert)
{
	struct list *a, *b, *c;
	int a_data, b_data, c_data;

	a_data = 1;
	b_data = 2;
	c_data = 3;

	a = list_insert(NULL, &a_data, 1);
	ck_assert(a->data == &a_data);
	ck_assert(a->prev == NULL);
	ck_assert(a->next == NULL);

	c = list_insert(a, &c_data, 50);
	ck_assert(c->data == &c_data);
	ck_assert(c->prev == a);
	ck_assert(c->next == NULL);
	ck_assert(a->prev == NULL);
	ck_assert(a->next == c);

	b = list_insert(a, &b_data, 1);
	ck_assert(b->data == &b_data);
	ck_assert(b->prev == a);
	ck_assert(b->next == c);
	ck_assert(a->prev == NULL);
	ck_assert(a->next == b);
	ck_assert(c->prev == b);
	ck_assert(c->next == NULL);

	list_free(a);
}
END_TEST

START_TEST(test_list_len) {
	struct list *start, *mid, *end;

	ck_assert(list_len(NULL) == 0);
	start = list_append(NULL, NULL);
	ck_assert(list_len(start) == 1);

	mid = list_append(start, NULL);
	ck_assert(list_len(start) == 2);
	ck_assert(list_len(mid) == 2);
	end = list_append(mid, NULL);

	ck_assert(list_len(start) == 3);
	ck_assert(list_len(end) == 3);
	ck_assert(list_len(mid) == 3);

	list_free(start);
}
END_TEST

START_TEST(test_list_lifecycle)
{
	struct list *l;

	l = list_alloc();
	list_free(l);

	l = list_alloc();
	l->data = malloc(sizeof(int));
	*(int*)l->data = 400;
	list_free_full(l, free);
}
END_TEST

START_TEST(test_list_pop)
{
	struct list *l;
	int a_data, b_data, c_data;

	l = NULL;
	ck_assert(list_pop(l) == NULL);

	a_data = 1;
	l = list_append(l, &a_data);
	b_data = 2;
	list_append(l, &b_data);
	c_data = 3;
	list_append(l, &c_data);

	ck_assert(list_pop(l) == &c_data);
	ck_assert(list_len(l) == 2);
	ck_assert(list_pop(l) == &b_data);
	ck_assert(list_len(l) == 1);
	ck_assert(list_pop(l) == &a_data);
}
END_TEST

START_TEST(test_list_pop_left)
{
	struct list *l;
	int a_data, b_data, c_data;

	l = NULL;
	ck_assert(list_pop(l) == NULL);

	a_data = 1;
	l = list_append(l, &a_data);
	b_data = 2;
	l = list_append(l, &b_data);
	c_data = 3;
	l = list_append(l, &c_data);

	ck_assert(list_pop_left(l) == &a_data);
	ck_assert(list_len(l) == 2);
	ck_assert(list_pop_left(l) == &b_data);
	ck_assert(list_len(l) == 1);
	ck_assert(list_pop_left(l) == &c_data);
}
END_TEST

START_TEST(test_list_prepend)
{
	struct list *l;
	int a_data, b_data;

	l = NULL;
	a_data = 1;
	b_data = 2;
	l = list_prepend(l, &b_data);
	l = list_prepend(l, &a_data);

	ck_assert(l->data == &a_data);
	ck_assert(l->prev == NULL);

	ck_assert(l->next->data == &b_data);
	ck_assert(l->next->prev == l);
	ck_assert(l->next->next == NULL);

	list_free(l);
}
END_TEST


START_TEST(test_list_start)
{
	struct list *l;
	size_t i;
	int start_data, other_data;

	l = NULL;
	start_data = 1234;
	other_data = 5678;

	ck_assert(list_start(l) == NULL);

	l = list_append(NULL, &start_data);
	ck_assert((list_start(l))->data == &start_data);
	for (i = 0; i < 5; i++) {
		l = list_append(l, &other_data);
	}

	ck_assert((list_start(l))->data == &start_data);
	list_free(l);
}
END_TEST

Suite *list_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("list");
	tc = tcase_create("Core");
	tcase_add_test(tc, test_list_append);
	tcase_add_test(tc, test_list_delete);
	tcase_add_test(tc, test_list_end);
	tcase_add_test(tc, test_list_get);
	tcase_add_test(tc, test_list_insert);
	tcase_add_test(tc, test_list_len);
	tcase_add_test(tc, test_list_lifecycle);
	tcase_add_test(tc, test_list_pop);
	tcase_add_test(tc, test_list_pop_left);
	tcase_add_test(tc, test_list_prepend);
	tcase_add_test(tc, test_list_start);
	suite_add_tcase(s, tc);

	return s;
}

int main()
{
	int num_failed;
	Suite *s;
	SRunner *sr;

	s = list_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_ENV);
	num_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
