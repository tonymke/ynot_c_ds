#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"
#include "ynot_c_ds.h"

START_TEST(test_alloc_free)
{
	char a, *b;
	queue *q;

	a = 'a';
	b = malloc(sizeof(*b));
	/* GCC's static analyzer doesn't see the abort in this.*/
	ck_assert_ptr_nonnull(b);
	*b = 'b';

	q = queue_alloc(5, NULL);
	ck_assert_ptr_nonnull(q);
	queue_free(q);

	q = queue_alloc(5, NULL);
	ck_assert_ptr_nonnull(q);
	ck_assert_int_eq(YNOT_OK, queue_append(q, &a));
	queue_free(q);

	q = queue_alloc(5, NULL);
	ck_assert_ptr_nonnull(q);
	ck_assert_int_eq(YNOT_OK, queue_append(q, b));
	/* rely on valgrind as our "assertion" here */
	queue_free(q);
}
END_TEST

START_TEST(test_insert)
{
	queue *q;
	int *a, *b, ok;

	a = malloc(sizeof(*a));
	ck_assert_ptr_nonnull(a);
	*a = 4;

	b = malloc(sizeof(*b));
	ck_assert_ptr_nonnull(b);
	*b = 6;

	q = NULL;
	ok = queue_append(q, a);
	ck_assert_int_eq(YNOT_EINVALIDPARAM, ok);

	q = queue_alloc(1, free);
	ck_assert_ptr_nonnull(q);

	ck_assert_int_eq(YNOT_OK, queue_append(q, a));
	ck_assert_uint_eq(1, queue_len(q));
	ck_assert_ptr_eq(a, queue_peek(q));

	ck_assert_int_eq(YNOT_OK, queue_prepend(q, &b));
	ck_assert_uint_eq(1, queue_len(q));
	ck_assert_ptr_eq(&b, queue_peek(q));

	queue_free(q);
}
END_TEST

TCase *queue_case(void)
{
	TCase *tc = tcase_create("queue");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_alloc_free);
	tcase_add_test(tc, test_insert);

	return tc;
}
