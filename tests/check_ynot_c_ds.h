#ifndef YNOT_C_DS_CHECK_H
#define YNOT_C_DS_CHECK_H

#include <check.h>

typedef TCase *(*test_case_factory)();
#define TEST_CASE_FACTORIES \
	array_case, \
	list_case, \
	map_case, \
	queue_case, \
	pqueue_case

TCase *array_case(void);
TCase *list_case(void);
TCase *map_case(void);
TCase *queue_case(void);
TCase *pqueue_case(void);

#endif /* YNOT_C_DS_CHECK_H */
