#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"
#include "ynot_c_ds.h"

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

START_TEST(test_add_prefix)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foo"));
	ck_assert(ptrie_contains(tri, "foo"));
	ck_assert(ptrie_contains(tri, "foobar"));
	ck_assert(ptrie_contains(tri, "foobat"));

	ptrie_free(tri);
}
END_TEST

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

START_TEST(test_remove_multi_sibling_suffix)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobaz"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobat"));
	ck_assert_int_eq(YNOT_OK, ptrie_remove(tri, "foobar"));
	ck_assert(!ptrie_contains(tri, "foobar"));
	ck_assert(ptrie_contains(tri, "foobaz"));
	ck_assert(ptrie_contains(tri, "foobat"));

	ptrie_free(tri);
}
END_TEST

START_TEST(test_remove_prefix_no_siblings)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foo"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert_int_eq(YNOT_OK, ptrie_remove(tri, "foo"));
	ck_assert(!ptrie_contains(tri, "foo"));
	ck_assert(ptrie_contains(tri, "foobar"));

	ptrie_free(tri);
}
END_TEST

START_TEST(test_remove_prefix_with_siblings)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foo"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobaz"));
	ck_assert_int_eq(YNOT_OK, ptrie_remove(tri, "foo"));
	ck_assert(!ptrie_contains(tri, "foo"));
	ck_assert(ptrie_contains(tri, "foobar"));
	ck_assert(ptrie_contains(tri, "foobaz"));

	ptrie_free(tri);
}
END_TEST

START_TEST(test_remove_only_suffix)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foo"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert_int_eq(YNOT_OK, ptrie_remove(tri, "foobar"));
	ck_assert(ptrie_contains(tri, "foo"));

	ptrie_free(tri);
}
END_TEST

START_TEST(test_remove_single_sibling_suffix)
{
	ptrie *tri = ptrie_alloc();
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobar"));
	ck_assert_int_eq(YNOT_OK, ptrie_add(tri, "foobaz"));
	ck_assert_int_eq(YNOT_OK, ptrie_remove(tri, "foobar"));
	ck_assert(!ptrie_contains(tri, "foobar"));
	ck_assert(ptrie_contains(tri, "foobaz"));

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
	tcase_add_test(tc, test_add_prefix);
	tcase_add_test(tc, test_alloc_free);
	tcase_add_test(tc, test_remove_multi_sibling_suffix);
	tcase_add_test(tc, test_remove_only_suffix);
	tcase_add_test(tc, test_remove_prefix_no_siblings);
	tcase_add_test(tc, test_remove_prefix_with_siblings);
	tcase_add_test(tc, test_remove_single_sibling_suffix);

	return tc;
}
