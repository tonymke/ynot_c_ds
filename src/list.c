#include <stdio.h>
#include <stdlib.h>

#include "ynot_c_ds.h"

struct list {
	int a;
	int b;
};

list *foo(int a, int b)
{
	list *l;
	if ((l = malloc(sizeof(list))) == NULL) {
		perror("foo: malloc");
		return NULL;
	}

	l->a = a;
	l->b = b;

	return l;
}

int bar(list *l)
{
	if (l == NULL)
		return 0;

	return l->a + l->b;
}
