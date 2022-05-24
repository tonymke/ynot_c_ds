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

TCase *array_case(void)
{
	TCase *tc = tcase_create("suite");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_alloc_free);

	return tc;
}
