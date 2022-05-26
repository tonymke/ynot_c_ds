#include <string.h>

#include "ynot_c_ds.h"

size_t next_power_of_2(size_t x)
{
	static const size_t metasize = sizeof(metasize) * 8 / 2;
	size_t shift;

	if (x == 0) {
		return 1;
	}

	/*
	 * If we are a power of 2, take a shortcut.
	 *
	 * https://stackoverflow.com/a/600306/929861
	 * Excerpt author: Greg Hewgill
	 */
	if ((x & (x - 1)) == 0) {
		return x<<1;
	}

	/*
	 * Derived from public domain work by Sean Anderson
	 * <seander@cs.stanford.edu>
	 * https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	 */
	x--;
	for (shift = 1; shift < metasize; shift *= 2) {
		x |= x >> shift;
	}
	x++;

	return x;
}

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
