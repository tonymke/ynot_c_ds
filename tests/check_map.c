#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"
#include "common.h"
#include "ynot_c_ds.h"

static size_t phony_hash(void *size_t_val)
{
	return *(size_t *)(size_t_val);
}

static int sizet_ptr_eq(void *a, void *b)
{
	return *(size_t *)a == *(size_t *)b;
}

START_TEST(test_alloc_free)
{
	map *mp;
	mp = map_alloc(hash_str_djb2, str_eq);
	ck_assert_ptr_nonnull(mp);
	ck_assert_uint_eq(0, map_len(mp));
	map_free(mp);

}
END_TEST

START_TEST(test_automatic_capacity_adjustment)
{
	const size_t input_len = 1024;

	map *mp;
	size_t i;
	size_t *keys, *values;

	mp = map_alloc_capacity(phony_hash, sizet_ptr_eq, 256);
	ck_assert_ptr_nonnull(mp);

	keys = ynot_malloc(input_len, sizeof(*keys));
	ck_assert_ptr_nonnull(keys);
	values = ynot_malloc(input_len, sizeof(*values));
	ck_assert_ptr_nonnull(values);
	for (i = 0; i < input_len; i++) {
		keys[i] = i;
		values[i] = i;
		map_set(mp, &keys[i], &values[i]);
	}
	for (i = 0; i <input_len; i++) {
		ck_assert_uint_eq(i, *(size_t *)map_get(mp, &i));
		ck_assert_uint_eq(i, *(size_t *)map_remove(mp, &i));
	}
	ck_assert_uint_eq(0, map_len(mp));
	map_free_full(mp, free, free);
	free(keys);
	free(values);
}
END_TEST

START_TEST(test_get_set_remove)
{
	map *mp;
	char *key, *val;
	void *buf;
	key = ynot_malloc(32, sizeof(*key));
	val = ynot_malloc(32, sizeof(*key));
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
	free(buf);
	free(key);

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
	tcase_add_test(tc, test_automatic_capacity_adjustment);
	tcase_add_test(tc, test_get_set_remove);

	return tc;
}
