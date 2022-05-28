#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"
#include "ynot_c_ds.h"

START_TEST(test_alloc_free)
{
	map *mp;
	mp = map_alloc(hash_str_djb2, str_eq);
	ck_assert_ptr_nonnull(mp);
	ck_assert_uint_eq(0, map_len(mp));
	map_free(mp);
}
END_TEST

START_TEST(test_get_set_remove)
{
	map *mp;
	char *key, *val;
	void *buf;
	key = malloc(sizeof(*key) * 32);
	val = malloc(sizeof(*key) * 32);
	strcpy(key, "a key");
	strcpy(val, "a val");

	mp = map_alloc_capacity(hash_str_djb2, str_eq, 2);
	ck_assert_ptr_nonnull(mp);
	ck_assert_uint_eq(0, map_len(mp));

	buf = map_set(mp, key, val);
	ck_assert_ptr_null(buf);
	ck_assert_uint_eq(1, map_len(mp));

	buf = map_get(mp, key);
	ck_assert_ptr_nonnull(buf);
	ck_assert_str_eq(buf, val);
	ck_assert_str_eq(buf, map_set(mp, key, val));

	buf = map_remove(mp, key);
	ck_assert_ptr_nonnull(buf);
	ck_assert_str_eq(buf, val);
	ck_assert_uint_eq(0, map_len(mp));

	map_free_full(mp, free, free);
}
END_TEST

TCase *map_case(void)
{
	TCase *tc = tcase_create("map");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_alloc_free);
	tcase_add_test(tc, test_get_set_remove);

	return tc;
}
