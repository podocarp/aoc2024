#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

/** Return 64-bit FNV-1a hash for key (NUL-terminated).
 */
static unsigned long hash(char *key) {
    unsigned long hash = FNV_OFFSET;
    for (char *p = key; *p; p++) {
        hash ^= (unsigned long)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

#define T char *
#define LIST list_s
#define ZERO_VALUE ""
#include "list.h"

typedef struct hashmap {
    // a list of keys in insert order
    list_s *ordered_keys;
    // buckets for keys
    char **keys;
    // buckets for their values
    char **values;
    uint len;
    uint capacity;
} hashmap;

hashmap *hashmap_new() {
    hashmap *h = malloc(sizeof(hashmap));
    h->ordered_keys = list_s_new(10);
    h->keys = calloc(10, sizeof(char *));
    h->values = calloc(10, sizeof(char *));
    h->len = 0;
    h->capacity = 10;
    return h;
}

void hashmap_free(hashmap *hm) {
    for (uint i = 0; i < hm->len; i++) {
        if (hm->keys[i] != NULL) {
            free(hm->keys[i]);
        }
        if (hm->values[i] != NULL) {
            free(hm->values[i]);
        }
    }
    free(hm->ordered_keys);
    free(hm->keys);
    free(hm->values);
    free(hm);
}

/** Helper function to inserts an item, but does not expand the hashmap.
 */
static void set(hashmap *hm, char *key, char *value) {
    ulong h = hash(key);
    ulong index = h % (hm->capacity - 1);
    while (hm->keys[index] != NULL) {
        if (strcmp(key, hm->keys[index]) == 0) {
            free(hm->values[index]);
            hm->values[index] = value;
            return;
        }
        index++;
        if (index >= hm->capacity) {
            index = 0;
        }
    }
    hm->keys[index] = key;
    hm->values[index] = value;
    list_s_push(hm->ordered_keys, key);
    hm->len++;
}

/** Doubles the space and reinserts all current keys and values into the bigger
 * hashmap.
 */
static void hashmap_expand(hashmap *hm) {
    uint new_cap = hm->capacity * 2;
    char **new_keys = calloc(new_cap, sizeof(char *));
    char **new_values = calloc(new_cap, sizeof(char *));
    char **old_keys = hm->keys;
    char **old_values = hm->values;
    uint old_cap = hm->capacity;
    hm->keys = new_keys;
    hm->values = new_values;
    hm->capacity = new_cap;

    hm->len = 0;
    for (uint i = 0; i < old_cap; i++) {
        char *key = old_keys[i];
        char *value = old_values[i];
        if (key != NULL) {
            set(hm, key, value);
        }
    }

    free(old_keys);
    free(old_values);
}

/** Sets the mapping between the two 0-terminated strings `key` and `value`. No
 * checking is performed on the two strings.
 */
void hashmap_set(hashmap *hm, char *key, char *value) {
    char *key_copy = strdup(key);
    char *value_copy = strdup(value);
    if (hm->len >= hm->capacity / 2) {
        hashmap_expand(hm);
    }
    set(hm, key_copy, value_copy);
}

/** Returns the corresponding value for this key if present, otherwise returns
 * NULL.
 */
char *hashmap_get(hashmap *hm, char *key) {
    if (key == NULL) {
        return NULL;
    }

    ulong h = hash(key);
    ulong index = h % (hm->capacity - 1);
    while (hm->keys[index] != NULL) {
        if (strcmp(hm->keys[index], key) == 0) {
            return hm->values[index];
        }

        index++;
        if (index >= hm->capacity) {
            index = 0;
        }
    }

    return NULL;
}

/** Returns the key of the i-th element in the hashmap. The order is not by
 * insertion order, but remains the same unless inserts have been performed. You
 * can use this to iterate through all elements, by using hashmap_len to get the
 * upper bound. But note that any inserts during your iteration will mess it up!
 */
inline char *hashmap_get_i(hashmap *hm, int i) {
    return hm->ordered_keys->items[i];
}

inline int hashmap_len(hashmap *hm) { return hm->len; }

void printhm(hashmap *hm) {
    for (uint i = 0; i < hm->capacity; i++) {
        char *key = hm->keys[i];
        char *value = hm->values[i];
        if (key != NULL) {
            printf("%s: %s, ", key, value);
        }
    }
    puts("");
}
