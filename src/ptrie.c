#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ynot_c_ds.h"

struct node {
	char *prefix;
	size_t prefix_len;
	int is_terminating;

	array *edges;
};

static struct node *node_add(struct node *nd, char *pfx, size_t pfx_n);
static struct node *node_alloc(char *pfx, size_t pfx_n, int is_terminating);
/*static struct node *node_contains (struct node *nd, char *v, size_t v_n);*/
static void         node_free (struct node *nd);
static void         node_free_generic (void *nd);
/*static int          node_remove (struct node *nd, char *v, size_t v_n);*/
static size_t       prefix_match_len(char *a, size_t a_n, char *b, size_t b_n);

size_t prefix_match_len(char *a, size_t a_n, char *b, size_t b_n)
{
	size_t i, min_n;
	if (a == NULL || a_n == 0 || b == NULL || b_n == 0) {
		return 0;
	}

	i = 0;
	min_n = a_n <= b_n ? a_n : b_n;

	while (i < min_n) {
		if (a[i] != b[i]) {
			break;
		}
		i++;
	}

	return i;
}

struct node *node_add(struct node *nd, char *pfx, size_t pfx_n)
{
	size_t i, fork_cnt, match_n;
	struct node *new = NULL;

	if (nd == NULL || pfx == NULL || pfx_n == 0) {
		return NULL;
	}

	match_n = prefix_match_len(nd->prefix, nd->prefix_len, pfx, pfx_n);
	/* if we aren't a partial match, do nothing */
	if (match_n == 0) {
		return NULL;
	}

	/* If we're a perfect match, declare ourselves terminating and return
	 * ourselves.
	 */
	if (match_n == nd->prefix_len && match_n == pfx_n) {
		nd->is_terminating = 1;
		return nd;
	}

	/* If we're a match up to our prefix_len and they are longer, see if
	 * they match any children.
	 */
	if (match_n == nd->prefix_len && pfx_n > nd->prefix_len) {
		for (i = 0; i < array_len(nd->edges); i++) {
			new = node_add(
					array_get(nd->edges, i),
					&pfx[match_n],
					pfx_n - match_n);
			if (new != NULL) {
				return new;
			}
		}
	}

	/* fork off one or more new nodes
	 *
	 * foobar with foo added will become
	 * foo-
	 *    |- bar
	 *
	 * foo with foobar added will become
	 * foo-
	 *    |-bar
	 *
	 * foobar with foobat added will become
	 * fooba-
	 *      |-r
	 *      |-t
	 */
	fork_cnt = 0;

	/* fork off our split into prefix and suffix, if necessary.
	 * e.g. we are foobar, foo being added
	 * */
	if (nd->prefix_len - match_n > 0) {
		array *buf;
		fork_cnt++;
		new = node_alloc(
				&nd->prefix[match_n],
				nd->prefix_len - match_n,
				nd->is_terminating);
		if (new == NULL) {
			return NULL;
		}

		/* the fork inherits our children */
		buf = new->edges;
		new->edges = nd->edges;
		nd->edges = buf;

		/* record our parentage */
		if (array_add(nd->edges, new) != YNOT_OK) {
			return NULL;
		}
	}

	/* fork off the added value's suffix, if necessary.
	 * e.g. we are foo, foobar was added
	 * */
	if (pfx_n - match_n > 0) {
		fork_cnt++;
		new = node_alloc(&pfx[match_n], pfx_n - match_n, 1);
		if (new == NULL) {
			return NULL;
		}

		/* record our parentage */
		if (array_add(nd->edges, new) != YNOT_OK) {
			return NULL;
		}
	}

	/* if we forked twice, we are no longer a terminating node */
	if (fork_cnt == 2) {
		nd->is_terminating = 0;
	}

	/* shorten ourselves if necessary */
	if (match_n < nd->prefix_len) {
		char *buf = realloc(nd->prefix, match_n + 1);
		if (buf == NULL) {
			perror("node_add: parent shorten realloc");
			return NULL;
		}
		buf[match_n] = '\0';
		nd->prefix = buf;
		nd->prefix_len = match_n;
	}

	/* return ourselves if a prefix of ourselves was added */
	if (pfx_n - match_n == 0) {
		return nd;
	}

	/* otherwise return the last node that was created, which is
	 * the prefix one */
	return new;
}

struct node *node_alloc(char *pfx, size_t pfx_n, int is_terminating)
{
	struct node *nd;

	if (pfx == NULL) {
		return NULL;
	}


	if (pfx_n == 0) {
		pfx_n = strlen(pfx);
		if (pfx_n == 0) {
			return NULL;
		}
	}

	nd = malloc(sizeof(*nd));
	if (nd == NULL) {
		perror("node_alloc: node malloc");
		return NULL;
	}
	nd->prefix_len = pfx_n;
	nd->is_terminating = is_terminating;

	nd->prefix = malloc(sizeof(*nd->prefix) * pfx_n + 1);
	if (nd->prefix == NULL) {
		perror("node_alloc: prefix malloc");
		free(nd);
		return NULL;
	}
	strcpy(nd->prefix, pfx);

	nd->edges = array_alloc();
	if (nd->edges == NULL) {
		free(nd->prefix);
		free(nd);
		return NULL;
	}

	return nd;
}

void node_free(struct node *nd)
{
	if (nd == NULL) {
		return;
	}

	array_free_full(nd->edges, node_free_generic);
	free(nd->prefix);
	free(nd);
}

void node_free_generic(void *nd)
{
	node_free((struct node *)nd);
}

int ptrie_add(ptrie *tri, char *pfx)
{
	int err;
	size_t pfx_n, i;
	struct node *nd;
	if (tri == NULL || pfx == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	pfx_n = strlen(pfx);
	if (pfx_n == 0) {
		return YNOT_EINVALIDPARAM;
	}

	/* if value shares a prefix with any child nodes, add it from there.
	 * */
	for(i = 0, nd = NULL; i < array_len(tri); i++) {
		nd = array_get(tri, i);
		nd = node_add(nd, pfx, pfx_n);
		if (nd != NULL) {
			return YNOT_OK;
		}
	}
	/* otherwise, alloc a new one */
	nd = node_alloc(pfx, pfx_n, 1);
	if (nd == NULL) {
		return YNOT_ENOMEM;
	}
	err = array_add(tri, nd);
	if (err != YNOT_OK) {
		node_free(nd);
		return err;
	}

	return YNOT_OK;
}

void ptrie_free(ptrie *tri)
{
	array_free_full(tri, node_free_generic);
}
