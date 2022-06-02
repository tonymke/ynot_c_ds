#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"
#include "ynot_c_ds.h"

START_TEST(test_alloc_free)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foo"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "barbaz"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "bar"));
	ptrie_free(tri);
}
END_TEST

START_TEST(test_add_contains)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foo"));
	ck_assert(!ptrie_contains(tri, "fo"));
	ck_assert(ptrie_contains(tri, "foo"));
	ck_assert(ptrie_contains(tri, "fooasdf"));

	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert(!ptrie_contains(tri, "fo"));
	ck_assert(ptrie_contains(tri, "foo"));
	ck_assert(ptrie_contains(tri, "fooasdf"));
	ck_assert(ptrie_contains(tri, "foobar"));
	ck_assert(ptrie_contains(tri, "foobar"));

	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "barbaz"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "bar"));

	ck_assert(ptrie_contains(tri, "bar"));
	ck_assert(ptrie_contains(tri, "barasdf"));
	ck_assert(ptrie_contains(tri, "barbaz"));

	ptrie_free(tri);
}
END_TEST

TCase *ptrie_case(void)
{
	TCase *tc = tcase_create("ptrie");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_add_contains);
	tcase_add_test(tc, test_alloc_free);

	return tc;
}
