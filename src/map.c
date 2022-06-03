#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "ynot_c_ds.h"

#ifndef YNOT_C_DS_MAP_MIN_CAPACITY
#define YNOT_C_DS_MAP_MIN_CAPACITY 128
#endif /* YNOT_C_DS_MAP_MIN_CAPACITY */

#ifndef YNOT_C_DS_MAP_SCALE_IN_THRESHOLD
#define YNOT_C_DS_MAP_SCALE_IN_THRESHOLD 0.1
#endif /* YNOT_C_DS_MAP_SCALE_IN_THRESHOLD */

#ifndef YNOT_C_DS_MAP_SCALE_OUT_THRESHOLD
#define YNOT_C_DS_MAP_SCALE_OUT_THRESHOLD 0.7
#endif /* YNOT_C_DS_MAP_SCALE_OUT_THRESHOLD */

struct map {
	size_t capacity;
	size_t min_capacity;
	size_t len;

	size_t (*hash_fn)  (void* key);
	int    (*key_eq_fn)(void *key_a, void*key_b);

	list *buckets;
};

struct map_kvp {
	size_t key_hash;
	void   *key;
	void   *value;
};

struct map_get_node_result {
	size_t bucket_i;
	list *bucket;
	struct list_node *node;
	struct map_kvp *kvp;
};

static struct map_get_node_result  map_get_node        (map *mp, void *key);

map *map_alloc_capacity(size_t (*hash_fn)(void* key),
			int (*key_eq_fn)(void *key_a, void *key_b),
			size_t initial_capacity)
{
	int err;
	map *mp;

	if (hash_fn == NULL || key_eq_fn == NULL) {
		fprintf(stderr, "map_alloc_capacity: invalid parameters\n");
		return NULL;
	}

	mp = ynot_malloc(1, sizeof(*mp));
	if (mp == NULL) {
		perror("map_alloc_capacity: map malloc");
		return NULL;
	}
	mp->capacity = mp->len = 0;
	mp->min_capacity = YNOT_C_DS_MAP_MIN_CAPACITY;
	mp->hash_fn = hash_fn;
	mp->key_eq_fn = key_eq_fn;
	mp->buckets = NULL;

	err = map_ensure_capacity(mp, initial_capacity);
	if (err != YNOT_OK) {
		free(mp);
		return NULL;
	}

	return mp;
}

int map_ensure_capacity(map *mp, size_t min_capacity) {
	list *new_buckets;
	size_t copied, i, new_capacity;

	if (mp == NULL) {
		fprintf(stderr, "map_ensure_capacity: invalid param\n");
		return YNOT_EINVALIDPARAM;
	}

	/* set new minimum capacity */
	if (min_capacity > 0) {
		if (min_capacity < YNOT_C_DS_MAP_MIN_CAPACITY) {
			min_capacity = YNOT_C_DS_MAP_MIN_CAPACITY;
		} else {
			min_capacity = next_power_of_2(min_capacity);
		}
		mp->min_capacity = min_capacity;
	}

	/* calc new capacity */
	new_capacity = mp->capacity;
	if (mp->len > new_capacity * YNOT_C_DS_MAP_SCALE_OUT_THRESHOLD) {
		new_capacity = next_power_of_2(new_capacity * 2);
	} else if (mp->len < new_capacity * YNOT_C_DS_MAP_SCALE_IN_THRESHOLD) {
		new_capacity = next_power_of_2(mp->len*2);
	}
	if (new_capacity < mp->min_capacity) {
		new_capacity = mp->min_capacity;
	}

	if (new_capacity == mp->capacity) {
		/* Within acceptable utilziation range; no change */
		return YNOT_OK;
	}

	/* alloc and zero a new buckets array */
	new_buckets = ynot_malloc(new_capacity, sizeof(*new_buckets));
	if (new_buckets == NULL) {
		perror("map_ensure_capacity: new_buckets malloc");
		return YNOT_ENOMEM;
	}
	{
		const list new_list = { 0, NULL, NULL };
		for(i = 0; i < new_capacity; i++) {
			new_buckets[i] = new_list;
		}
	}

	/* copy data to new_buckets */
	for (i = copied = 0; i < mp->capacity && copied < mp->len; i++) {
		list *src = &mp->buckets[i];
		struct list_node *cur;
		/* for each bucket, copy list node values and "steal"
		 * the list nodes to avoid unnecessary malloc/free */
		for (cur = src->start; cur !=NULL; cur = cur->next) {
			list *dest;
			struct map_kvp *map_node;

			map_node = (struct map_kvp *)cur->value;
			dest = &new_buckets[map_node->key_hash % new_capacity];
			if (dest->end == NULL) {
				/* we are the first node */
				dest->start = dest->end = cur;
				cur->prev = cur->next = NULL;
			} else {
				/* we are appending to the list*/
				dest->end->next = cur;
				cur->prev = dest->end;
				cur->next = NULL;
				dest->end = cur;
			}
			copied++;
		}
	}

	/* free the old buckets array, update struct */
	if (mp->buckets != NULL) {
		free(mp->buckets);
		mp->buckets = NULL;
	}
	mp->capacity = new_capacity;
	mp->buckets = new_buckets;

	return YNOT_OK;
}

void map_free_bucket_list_nodes (list *bucket,
				void (*free_key)(void *key),
				void (*free_value)(void *value))
{
	struct list_node *cur, *next;

	if (bucket == NULL) {
		return;
	}

	cur = bucket->start;
	while (cur != NULL) {
		struct map_kvp *map_node;

		next = cur->next;
		map_node = (struct map_kvp *)cur->value;
		if (map_node != NULL) {
			if (free_key != NULL) {
				free_key(map_node->key);
			}
			map_node->key = NULL;

			if (free_value != NULL) {
				free_value(map_node->value);
			}
			map_node->value = NULL;

			free(map_node);
			cur->value = NULL;
		}
		free(cur);
		cur = next;
	}
}

void map_free_full     (map *mp,
			void (*free_key)(void *key),
			void (*free_value)(void *value))
{
	size_t i;

	if (mp == NULL) {
		return;
	}

	for (i = 0; i < mp->capacity; i++) {
		struct list_node *cur, *next;
		list *bucket = &(mp->buckets[i]);

		cur = bucket->start;
		while (cur != NULL) {

			next = cur->next;
			if (cur->value != NULL) {
				struct map_kvp *map_node = cur->value;
				if (free_key != NULL) {
					free_key(map_node->key);
				}
				map_node->key = NULL;

				if (free_value != NULL) {
					free_value(map_node->value);
				}
				map_node->value = NULL;

				free(map_node);
				cur->value = NULL;
			}
			free(cur);
			cur = next;
		}
	}
	free(mp->buckets);
	mp->buckets = NULL;
	free(mp);
}

void *map_get(map *mp, void *key)
{
	struct map_get_node_result res;
	res = map_get_node(mp, key);

	if (res.kvp == NULL) {
		return NULL;
	}

	return res.kvp->value;
}

struct map_get_node_result map_get_node(map *mp, void *key)
{
	struct list_node *node;
	struct map_get_node_result res = { 0, NULL, NULL, NULL };

	if (mp == NULL) {
		return res;
	}

	res.bucket_i = mp->hash_fn(key) % mp->capacity;
	res.bucket = &mp->buckets[res.bucket_i];
	for (node = res.bucket->start; node != NULL; node = node->next) {
		struct map_kvp *kvp = (struct map_kvp *)node->value;
		if (mp->key_eq_fn(key, kvp->key)) {
			res.node = node;
			res.kvp = kvp;
			return res;
		}
	}

	return res;
}

size_t map_len(map *mp)
{
	if (mp == NULL) {
		return 0;
	}

	return mp->len;
}

void *map_remove(map *mp, void *key)
{
	void *old_value;
	struct map_get_node_result existing = map_get_node(mp, key);

	if (existing.kvp == NULL) {
		return NULL;
	}

	old_value = existing.kvp->value;
	free(list_remove_node(existing.bucket, existing.node));
	mp->len--;

	return old_value;
}

void *map_set(map *mp, void *key, void *value)
{
	int err;
	struct map_get_node_result existing = map_get_node(mp, key);
	struct map_kvp *kvp;
	void *old_value;

	if (existing.kvp != NULL) {
		old_value = existing.kvp->value;
		existing.kvp->value = value;
		return old_value;
	}

	kvp = ynot_malloc(1, sizeof(*kvp));
	if (kvp == NULL) {
		perror("map_set: kvp malloc\n");
		return NULL;
	}
	kvp->key = key;
	kvp->value = value;
	kvp->key_hash = mp->hash_fn(kvp->key);

	err = list_append(existing.bucket, kvp);
	if (err != YNOT_OK) {
		fprintf(stderr, "map_set: list append failed (%d)\n", err);
		free(kvp);
		return NULL;
	}
	mp->len++;

	map_ensure_capacity(mp, 0);

	return NULL;
}
