#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "ynot_c_ds.h"

struct node;

struct match {
	struct node *parent, *node;
};

struct node {
	char *prefix;
	size_t prefix_len;
	int is_terminating;

	array *edges;
};

static struct node  *node_add                  (struct node *nd,
						char *pfx,
						size_t pfx_n);
static struct node  *node_alloc                (char *pfx,
						size_t pfx_n,
						int is_terminating);
static struct node  *node_contains             (struct node *nd,
						char *v,
						size_t v_n);
static struct match  node_contains_exact       (struct node *p,
						struct node *nd,
						char *v,
						size_t v_n);
static void          node_free                 (struct node *nd);
static void          node_free_generic         (void *nd);
static int           node_merge_with_child     (struct node *parent,
						struct node *child);
static size_t        prefix_match_len          (char *a,
						size_t a_n,
						char *b,
						size_t b_n);

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
		char *buf;
		if (1 > SIZE_MAX - match_n) {
			fprintf(stderr,
				"node_add: parent shorten realloc: overflow\n");
			return NULL;
		}
		buf = ynot_realloc(nd->prefix, 1, match_n + 1);
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

	nd = ynot_malloc(1, sizeof(*nd));
	if (nd == NULL) {
		perror("node_alloc: node malloc");
		return NULL;
	}
	nd->prefix_len = pfx_n;
	nd->is_terminating = is_terminating;

	nd->prefix = ynot_malloc(pfx_n + 1, sizeof(*nd->prefix));
	if (nd->prefix == NULL) {
		perror("node_alloc: prefix malloc");
		free(nd);
		return NULL;
	}
	nd->prefix[pfx_n] = '\0';
	strncpy(nd->prefix, pfx, pfx_n);

	nd->edges = array_alloc();
	if (nd->edges == NULL) {
		free(nd->prefix);
		nd->prefix = NULL;
		free(nd);
		return NULL;
	}

	return nd;
}

struct node *node_contains(struct node *nd, char *v, size_t v_n)
{
	size_t i;
	size_t match_n = prefix_match_len(v, v_n, nd->prefix, nd->prefix_len);
	/* If the full prefix isn't in val, we are not a match */
	if (match_n < nd->prefix_len) {
		return NULL;
	}
	/* If we're terminating, we're a match */
	if (nd->is_terminating) {
		return nd;
	}

	/* If we're not terminating, inspect further into the branch */
	for (i = 0; i < array_len(nd->edges); i++) {
		struct node *edge = array_get(nd->edges, i);
		edge = node_contains(edge, &v[match_n], v_n - match_n);
		if (edge != NULL) {
			return edge;
		}
	}

	return NULL;
}

struct match node_contains_exact       (struct node *p,
					struct node *nd,
					char *v,
					size_t v_n)
{
	size_t i, match_n;
	struct match res = { NULL, NULL };
	res.parent = p;

	if (nd == NULL || v == NULL || v_n == 0) {
		return res;
	}

	match_n = prefix_match_len(v, v_n, nd->prefix, nd->prefix_len);

	/* If the full prefix isn't in val, we are not a match */
	if (match_n < nd->prefix_len) {
		return res;
	}

	/* If we're terminating and an exact match, we're the match */
	if (match_n == nd->prefix_len && match_n == v_n) {
		if (nd->is_terminating) {
			res.node = nd;
			return res;
		} else {
			return res;
		}
	}

	/* inspect further into the branch */
	res.parent = nd;
	for (i = 0; i < array_len(nd->edges); i++) {
		res.node = array_get(nd->edges, i);
		res = node_contains_exact(
				nd,
				res.node,
				&v[match_n],
				v_n - match_n);
		if (res.node != NULL) {
			return res;
		}
	}

	return res;
}

void node_free(struct node *nd)
{
	if (nd == NULL) {
		return;
	}

	array_free_full(nd->edges, node_free_generic);
	nd->edges = NULL;

	free(nd->prefix);
	nd->prefix = NULL;

	free(nd);
}

void node_free_generic(void *nd)
{
	node_free((struct node *)nd);
}
int node_merge_with_child(struct node *parent, struct node *child)
{
	size_t i;
	char *buf;
	if (parent == NULL || child == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	{
		/* prevent an overflow */
		size_t tot = parent->prefix_len;
		if (child->prefix_len > SIZE_MAX - tot) {
			fprintf(stderr, "node_merge_with_child: overflow\n");
			return YNOT_ENOMEM;
		}
		tot += child->prefix_len;
		if (1 > SIZE_MAX - tot) {
			fprintf(stderr, "node_merge_with_child: overflow\n");
			return YNOT_ENOMEM;
		}
		tot += 1;
		buf = ynot_malloc(1, tot);
		if (buf == NULL) {
			perror("node_merge_with_child: buf malloc");
			return YNOT_ENOMEM;
		}
	}
	strncpy(buf, parent->prefix, parent->prefix_len);
	strncpy(&buf[parent->prefix_len], child->prefix, child->prefix_len);
	buf[parent->prefix_len + child->prefix_len] = '\0';

	free(parent->prefix);
	parent->prefix = buf;
	parent->prefix_len += child->prefix_len;
	parent->is_terminating = child->is_terminating;
	/* remove child from parent's edges array */
	for (i = 0; i < array_len(parent->edges); i++) {
		struct node *nd = array_get(parent->edges, i);
		if (nd == child) {
			array_remove_at(parent->edges, i);
			break;
		}
	}

	/* Add grandchildren into parent's edge's array */
	for (i = array_len(child->edges) - 1; i < SIZE_MAX; i--) {
		array_add(parent->edges, array_remove_at(child->edges, i));
	}
	node_free(child);

	return YNOT_OK;
}

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

int ptrie_contains(ptrie *tri, char *value)
{
	size_t i, val_n;

	if (tri == NULL || value == NULL) {
		return 0;
	}

	val_n = strlen(value);
	if (val_n == 0) {
		return 0;
	}

	for (i = 0; i < array_len(tri); i++) {
		struct node *nd = array_get(tri, i);
		if (node_contains(nd, value, val_n)) {
			return 1;
		}
	}

	return 0;
}

void ptrie_free(ptrie *tri)
{
	array_free_full(tri, node_free_generic);
}

int ptrie_remove(ptrie *tri, char *pfx)
{
	size_t pfx_n, i, node_n, p_n;
	array *parent_edges = tri;
	struct match m;
	struct node *nd;

	/* validate input */
	if (tri == NULL || pfx == NULL) {
		return YNOT_EINVALIDPARAM;
	}

	pfx_n = strlen(pfx);
	if (pfx_n == 0) {
		return YNOT_EINVALIDPARAM;
	}

	/* find the exact node in question */
	p_n = array_len(tri);
	for (i = 0; i < p_n; i++) {
		struct node *nd = array_get(tri, i);
		m = node_contains_exact(NULL, nd, pfx, pfx_n);
		if (m.node != NULL) {
			parent_edges = (m.parent == NULL ? tri : m.parent->edges);
			p_n = array_len(parent_edges);
			break;
		}
	}

	/* if not found, we're done */
	if (m.node == NULL) {
		return YNOT_OK;
	}

	/* adjust our immediate relatives */
	node_n = array_len(m.node->edges);
	switch (node_n) {
	case 0:
		/* If we don't have children, remove ourselves from our
		 * parent */
		for (i = 0; i < p_n; i++) {
			nd = array_get(parent_edges, i);
			if (nd == m.node) {
				node_free(array_remove_at(parent_edges, i));
				break;
			}
		}

		/* If the parent is not the root node, not terminating, and
		 * only has one other child: merge that child into it */
		if (m.parent != NULL && !m.parent->is_terminating) {
			if (array_len(parent_edges) == 1) {
				struct node *sibling = array_get(m.parent->edges, 0);
				return node_merge_with_child(m.parent, sibling);
			}
		}
		break;
	case 1:
		/* If we have exactly one child, merge it into us */
		nd = array_get(m.node->edges, 0);
		return node_merge_with_child(m.node, nd);
		break;
	default:
		/* if we have more than one child, just remove our
		 * terminating flag */
		m.node->is_terminating = 0;
		break;
	}

	return YNOT_OK;
}
