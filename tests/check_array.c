#include <stdlib.h>

#include <check.h>

#include "ynot_c_ds.h"

START_TEST(test_alloc_free)
{
	char a, *b;
	array *lst;

	a = 'a';
	b = malloc(sizeof(*b));
	/* GCC's static analyzer doesn't see the abort in this.*/
	ck_assert_ptr_nonnull(b);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wanalyzer-null-dereference"
	*b = 'b';
	#pragma GCC diagnostic pop

	lst = array_alloc();
	ck_assert_ptr_nonnull(lst);
	array_free(lst);

	lst = array_alloc();
	ck_assert_ptr_nonnull(lst);
	ck_assert_int_eq(YNOT_OK, array_add(lst, &a));
	array_free(lst);

	lst = array_alloc();
	ck_assert_ptr_nonnull(lst);
	ck_assert_int_eq(YNOT_OK, array_add(lst, b));
	/* rely on valgrind as our "assertion" here */
	array_free_full(lst, free);
}
END_TEST

START_TEST(test_clear)
{
	array *arr;
	size_t i;

	arr = array_alloc_capacity(12);
	ck_assert_ptr_nonnull(arr);

	for(i = 0; i < 12; i++) {
		array_add(arr, NULL);
	}

	array_clear(arr);
	ck_assert_uint_eq(0L, array_len(arr));
}

START_TEST(test_add_insert)
{
	array *arr;
	int a, b, c, err;
	a = 1;
	b = 2;
	c = 3;

	err = array_insert(NULL, 0, &a);
	ck_assert_int_eq(YNOT_EINVALIDPARAM, err);

	arr = array_alloc();
	ck_assert_ptr_nonnull(arr);
	err = array_insert(arr, 50, &a);
	ck_assert_int_eq(YNOT_EOUTOFRANGE, err);
	ck_assert_uint_eq(0L, array_len(arr));

	array_add(arr, &c);
	ck_assert_uint_eq(1U, array_len(arr));
	ck_assert_ptr_eq(&c, array_get(arr, 0));

	array_insert(arr, 0, &a);
	ck_assert_uint_eq(2U, array_len(arr));
	ck_assert_ptr_eq(&a, array_get(arr, 0));
	ck_assert_ptr_eq(&c, array_get(arr, 1));

	array_insert(arr, 1, &b);
	ck_assert_uint_eq(3U, array_len(arr));
	ck_assert_ptr_eq(&a, array_get(arr, 0));
	ck_assert_ptr_eq(&b, array_get(arr, 1));
	ck_assert_ptr_eq(&c, array_get(arr, 2));

	array_free(arr);
}
END_TEST

START_TEST(test_remove_at)
{
	array *arr;
	void *removed;
	int a, b, c;

	a = 1;
	b = 2;
	c = 3;

	removed = array_remove_at(NULL, 0);
	ck_assert_ptr_null(removed);

	arr = array_alloc();
	ck_assert_ptr_nonnull(arr);
	removed = array_set(arr, 0, &a);
	ck_assert_ptr_null(removed);

	array_add(arr, &a);
	array_add(arr, &b);
	array_add(arr, &c);

	removed = array_remove_at(arr, 0);
	ck_assert_ptr_eq(&a, removed);
	ck_assert_uint_eq(2L, array_len(arr));

	removed = array_remove_at(arr, 1);
	ck_assert_ptr_eq(&c, removed);
	ck_assert_uint_eq(1L, array_len(arr));

	removed = array_remove_at(arr, 0);
	ck_assert_ptr_eq(&b, removed);
	ck_assert_uint_eq(0L, array_len(arr));

	array_free(arr);
}
END_TEST

START_TEST(test_set)
{
	array *arr;
	void *replaced;
	int a, b, err;

	a = 1;
	b = 2;

	replaced = array_set(NULL, 0, &a);
	ck_assert_ptr_null(replaced);

	arr = array_alloc();
	ck_assert_ptr_nonnull(arr);
	replaced = array_set(arr, 0, &a);
	ck_assert_ptr_null(replaced);

	err = array_add(arr, &a);
	ck_assert_int_eq(YNOT_OK, err);
	replaced = array_set(arr, 0, &b);
	ck_assert_ptr_eq(&a, replaced);

	ck_assert_ptr_eq(&b, array_get(arr, 0));

	array_free(arr);
}
END_TEST

TCase *array_case(void)
{
	TCase *tc = tcase_create("suite");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_alloc_free);
	tcase_add_test(tc, test_add_insert);
	tcase_add_test(tc, test_clear);
	tcase_add_test(tc, test_remove_at);
	tcase_add_test(tc, test_set);

	return tc;
}
