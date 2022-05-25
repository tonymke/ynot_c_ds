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

START_TEST(test_add_insert)
{
	array *arr;
	int a, b, c, ok;
	a = 1;
	b = 2;
	c = 3;

	ok = array_insert(NULL, 0, &a);
	ck_assert_int_eq(YNOT_EINVALIDPARAM, ok);


	arr = array_alloc();
	ck_assert_ptr_nonnull(arr);
	ok = array_insert(arr, 50, &a);
	ck_assert_int_eq(YNOT_EOUTOFRANGE, ok);
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

TCase *array_case(void)
{
	TCase *tc = tcase_create("suite");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_alloc_free);
	tcase_add_test(tc, test_add_insert);

	return tc;
}
