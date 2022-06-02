#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ynot_c_ds.h"

struct node {
	char *prefix;
	size_t prefix_len;

	array *edges;
};

/*static struct node *node_add(struct node *nd, char *pfx, size_t pfx_n);*/
/*static struct node *node_alloc(char *pfx, size_t pfx_n);*/
/*static struct node *node_contains (struct node *nd, char *v, size_t v_n);*/
static void         node_free (struct node *nd);
static void         node_free_generic (void *nd);
/*static int          node_remove (struct node *nd, char *v, size_t v_n);*/

struct node *node_alloc(char *pfx, size_t pfx_n)
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

	nd->prefix = malloc(sizeof(*nd->prefix) * pfx_n + 1);
	if (nd->prefix == NULL) {
		perror("node_alloc: prefix malloc");
		free(nd);
		return NULL;
	}
	nd->prefix = strcpy(nd->prefix, pfx);

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

void ptrie_free(ptrie *tri)
{
	array_free_full(tri, node_free_generic);
}
