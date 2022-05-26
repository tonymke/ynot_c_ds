#include <string.h>

#include "ynot_c_ds.h"

int ptr_eq(void *a, void *b)
{
	return a == b;
}

int str_eq(void *a, void *b)
{
	if (a == NULL || b == NULL) {
		return a == b;
	}

	return !strcmp(a, b);
}
