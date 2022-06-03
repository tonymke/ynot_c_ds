#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"
#include "common.h"
#include "ynot_c_ds.h"

START_TEST(test_alloc_free)
{
	pqueue *pq = NULL;
	char *input = ynot_malloc(1, sizeof(*input));
	ck_assert_ptr_nonnull(input);
	*input = 'a';

	pqueue_free(pq);
	pqueue_free_full(pq, free);

	pq = pqueue_alloc();
	ck_assert_ptr_nonnull(pq);

	ck_assert_int_eq(YNOT_OK, pqueue_push(pq, 0, input));
	pqueue_free(pq);

	pq = pqueue_alloc();
	ck_assert_ptr_nonnull(pq);

	ck_assert_int_eq(YNOT_OK, pqueue_push(pq, 0, input));
	pqueue_free_full(pq, free);
}
END_TEST

START_TEST(test_peek_pop_push)
{
	pqueue *pq = NULL;
	int input[] = { 1, 2, 11 };
	ck_assert_ptr_nonnull(input);

	pq = pqueue_alloc();
	ck_assert_ptr_nonnull(pq);

	ck_assert_ptr_null(pqueue_peek(pq));
	ck_assert_ptr_null(pqueue_pop(pq));

	ck_assert_int_eq(YNOT_OK, pqueue_push(pq, 1, &input[0]));
	ck_assert_int_eq(YNOT_OK, pqueue_push(pq, 1, &input[1]));
	ck_assert_int_eq(YNOT_OK, pqueue_push(pq, 2, &input[2]));

	ck_assert_int_eq(input[2], *(int *)pqueue_pop(pq));
	/* ck_assert_ptr_eq(&input[2], pqueue_peek(pq)); */

	ck_assert_int_eq(input[0], *(int *)pqueue_pop(pq));
	/* ck_assert_ptr_eq(&input[0], pqueue_peek(pq)); */

	ck_assert_int_eq(input[1], *(int *)pqueue_pop(pq));
	/* ck_assert_ptr_eq(&input[1], pqueue_peek(pq)); */

	ck_assert_ptr_null(pqueue_pop(pq));

	pqueue_free(pq);
}
END_TEST

TCase *pqueue_case(void)
{
	TCase *tc = tcase_create("pqueue");
	if (tc == NULL) {
		return NULL;
	}
	tcase_add_test(tc, test_alloc_free);
	tcase_add_test(tc, test_peek_pop_push);

	return tc;
}
