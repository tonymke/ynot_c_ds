#include <stdio.h>
#include <stdlib.h>

#include <check.h>

#include "check_ynot_c_ds.h"

int apply_test_case_factory(Suite *suite, test_case_factory tcf)
{
	TCase *tc;

	if (suite == NULL) {
		fprintf(stderr, "apply_test_case_factory: null suite");
		return 0;
	}

	tc = tcf();
	if (tc == NULL) {
		fprintf(stderr, "apply_test_case_factory: tcase create");
		return 0;
	}

	suite_add_tcase(suite, tc);

	return 1;
}

int main()
{
	static const test_case_factory tcfs[] = { TEST_CASE_FACTORIES, NULL };

	int num_failed;
	Suite *suite;
	SRunner *runner;
	size_t i;

	suite = suite_create("ynot_c_ds");
	if (suite == NULL) {
		fprintf(stderr, "main: suite_create failed");
		return EXIT_FAILURE;
	}
	runner = srunner_create(suite);
	if (runner == NULL) {
		fprintf(stderr, "main: srunner_create failed");
		return EXIT_FAILURE;
	}

	for (i = 0; tcfs[i] != NULL; i++) {
		if (!apply_test_case_factory(suite, tcfs[i])) {
			return EXIT_FAILURE;
		}
	}

	srunner_run_all(runner, CK_ENV);
	num_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
