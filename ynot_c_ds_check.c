#include <stdlib.h>

#include <check.h>

#include "list_check.h"

int main()
{
	int num_failed;
	Suite *suite;
	SRunner *runner;


	suite = suite_create("ynot_c_ds");
	runner = srunner_create(suite);
	suite_add_tcase(suite, list_tcase_init());

	srunner_run_all(runner, CK_ENV);
	num_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
