#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"
#include "ynot_c_ds.h"

START_TEST(test_alloc_free)
{
	/* TODO */
}
END_TEST

TCase *ptrie_case(void)
{
	TCase *tc = tcase_create("ptrie");
	if (tc == NULL) {
		return NULL;
	}

	tcase_add_test(tc, test_alloc_free);

	return tc;
}
