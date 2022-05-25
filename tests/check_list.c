#include <stdlib.h>

#include <check.h>

#include "ynot_c_ds.h"

START_TEST(test_alloc_free)
{
	char a, *b;
	list *lst;

	a = 'a';
	b = malloc(sizeof(*b));
	/* GCC's static analyzer doesn't see the abort in this.*/
	ck_assert_ptr_nonnull(b);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wanalyzer-null-dereference"
	*b = 'b';
	#pragma GCC diagnostic pop

	lst = list_alloc();
	ck_assert_ptr_nonnull(lst);
	list_free(lst);

	lst = list_alloc();
	ck_assert_ptr_nonnull(lst);
	ck_assert_int_eq(YNOT_OK, list_append(lst, &a));
	list_free(lst);

	lst = list_alloc();
	ck_assert_ptr_nonnull(lst);
	ck_assert_int_eq(YNOT_OK, list_append(lst, b));
	/* rely on valgrind as our "assertion" here */
	list_free_full(lst, free);
}
END_TEST

START_TEST(test_insert)
{
	list *lst;
	int a, b, c, ok;

	a = 1;
	b = 2;
	c = 3;

	lst = NULL;
	ok = list_append(lst, &a);
	ck_assert_int_eq(YNOT_EINVALIDPARAM, ok);

	lst = list_alloc();
	ok = list_append(lst, &c);
	ck_assert_int_eq(YNOT_OK, ok);
	ck_assert_uint_eq(1U, list_len(lst));
	ck_assert_ptr_eq(&c, list_peek_at(lst, 0));
	ck_assert_ptr_eq(&c, list_peek(lst));
	ck_assert_ptr_eq(&c, list_peek_left(lst));

	ok = list_prepend(lst, &a);
	ck_assert_int_eq(YNOT_OK, ok);
	ck_assert_uint_eq(2U, list_len(lst));
	ck_assert_ptr_eq(&a, list_peek_at(lst, 0));
	ck_assert_ptr_eq(&c, list_peek_at(lst, 1));
	ck_assert_ptr_eq(&c, list_peek(lst));
	ck_assert_ptr_eq(&a, list_peek_left(lst));

	ok = list_insert(lst, 3, &b);
	ck_assert_int_eq(YNOT_EOUTOFRANGE, ok);

	ok = list_insert(lst, 1, &b);
	ck_assert_int_eq(YNOT_OK, ok);
	ck_assert_uint_eq(3U, list_len(lst));
	ck_assert_ptr_eq(&a, list_peek_at(lst, 0));
	ck_assert_ptr_eq(&b, list_peek_at(lst, 1));
	ck_assert_ptr_eq(&c, list_peek_at(lst, 2));
	ck_assert_ptr_eq(&c, list_peek(lst));
	ck_assert_ptr_eq(&a, list_peek_left(lst));

	list_free(lst);
}
END_TEST

START_TEST(test_len)
{
	list *lst;
	int ok;

	lst = NULL;
	ok = list_append(lst, NULL);
	ck_assert_int_eq(YNOT_EINVALIDPARAM, ok);

	lst = list_alloc();
	ok = list_append(lst, NULL);
	ck_assert_int_eq(YNOT_OK, ok);
	ck_assert_uint_eq(1U, list_len(lst));

	ok = list_append(lst, NULL);
	ck_assert_int_eq(YNOT_OK, ok);
	ck_assert_uint_eq(2U, list_len(lst));

	list_pop(lst);
	ck_assert_uint_eq(1U, list_len(lst));

	list_pop(lst);
	ck_assert_uint_eq(0U, list_len(lst));

	list_free(lst);
}
END_TEST

START_TEST(test_peek)
{
	list *lst;
	int a, b, c;

	a = 1;
	b = 2;
	c = 3;

	lst = NULL;
	ck_assert_ptr_null(list_peek(lst));
	ck_assert_ptr_null(list_peek_left(lst));
	ck_assert_ptr_null(list_peek_at(lst, 0));
	ck_assert_ptr_null(list_peek_at(lst, 50));

	/* [ ] */
	lst = list_alloc();
	ck_assert_ptr_null(list_peek(lst));
	ck_assert_ptr_null(list_peek_left(lst));
	ck_assert_ptr_null(list_peek_at(lst, 0));
	ck_assert_ptr_null(list_peek_at(lst, 50));

	/* [ a ] */
	list_append(lst, &a);
	ck_assert_ptr_eq(&a, list_peek(lst));
	ck_assert_ptr_eq(&a, list_peek_left(lst));
	ck_assert_ptr_eq(&a, list_peek_at(lst, 0));
	ck_assert_ptr_eq(NULL, list_peek_at(lst, 1));

	/* [ a b ] */
	list_append(lst, &b);
	ck_assert_ptr_eq(&b, list_peek(lst));
	ck_assert_ptr_eq(&a, list_peek_left(lst));
	ck_assert_ptr_eq(&a, list_peek_at(lst, 0));
	ck_assert_ptr_eq(&b, list_peek_at(lst, 1));
	ck_assert_ptr_eq(NULL, list_peek_at(lst, 2));

	/* [ a b c ] */
	list_append(lst, &c);
	ck_assert_ptr_eq(&c, list_peek(lst));
	ck_assert_ptr_eq(&a, list_peek_left(lst));
	ck_assert_ptr_eq(&a, list_peek_at(lst, 0));
	ck_assert_ptr_eq(&b, list_peek_at(lst, 1));
	ck_assert_ptr_eq(&c, list_peek_at(lst, 2));
	ck_assert_ptr_eq(NULL, list_peek_at(lst, 3));

	list_free(lst);
}

START_TEST(test_removal)
{
	list *lst;
	int a, b, c;

	a = 1;
	b = 2;
	c = 3;

	lst = NULL;
	ck_assert_ptr_null(list_pop(lst));
	ck_assert_ptr_null(list_pop_left(lst));
	ck_assert_ptr_null(list_remove_at(lst, 50));

	lst = list_alloc();
	ck_assert_ptr_null(list_pop(lst));
	ck_assert_ptr_null(list_pop_left(lst));
	ck_assert_ptr_null(list_remove_at(lst, 50));

	ck_assert_int_eq(YNOT_OK, list_append(lst, &a));
	ck_assert_int_eq(YNOT_OK, list_append(lst, &b));
	ck_assert_int_eq(YNOT_OK, list_append(lst, &c));

	ck_assert_ptr_eq(&b, list_remove_at(lst, 1));
	ck_assert_uint_eq(2U, list_len(lst));
	ck_assert_int_eq(YNOT_OK, list_insert(lst, 1, &b));

	ck_assert_ptr_eq(&a, list_pop_left(lst));
	ck_assert_uint_eq(2U, list_len(lst));

	ck_assert_ptr_eq(&c, list_pop(lst));
	ck_assert_uint_eq(1U, list_len(lst));

	list_free(lst);
}

TCase *list_case(void)
{
	TCase *tc = tcase_create("list");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_alloc_free);
	tcase_add_test(tc, test_insert);
	tcase_add_test(tc, test_len);
	tcase_add_test(tc, test_peek);
	tcase_add_test(tc, test_removal);

	return tc;
}
