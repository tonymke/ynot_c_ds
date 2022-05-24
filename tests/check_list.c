#include <stdlib.h>

#include <check.h>

#include "ynot_c_ds.h"

START_TEST(test_placeholder)
{
	ck_assert(1);
}
END_TEST

Suite *list_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("list");
	tc = tcase_create("Core");

	tcase_add_test(tc, test_placeholder);

	suite_add_tcase(s, tc);

	return s;
}

int main()
{
	int num_failed;
	Suite *s;
	SRunner *sr;

	s = list_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_ENV);
	num_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
