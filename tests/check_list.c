#include <stdlib.h>

#include <check.h>

#include "ynot_c_ds.h"

START_TEST(test_lifecycle)
{
	list *l;
	l = list_alloc();
	ck_assert_ptr_nonnull(l);
	list_free(l);
}
END_TEST

Suite *list_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("list");
	tc = tcase_create("Core");

	tcase_add_test(tc, test_lifecycle);

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
