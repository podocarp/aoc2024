#ifndef AOC
#include <stdbool.h>
#include <stddef.h>

typedef unsigned int uint;
typedef unsigned long ulong;

#define ARRAYSIZE(x) (sizeof(x) / sizeof((x)[0]))

void quicksort_int(int *arr, int len);

void printarr(char *fmt, void *arr, size_t elem_size, size_t elem_len);
void printstr(char *str, int maxlen);
void reverse(char s[]);
void itoa(int n, char s[]);

bool strprefix(char *str, char *match, int maxlen);

/** A hashmap from char * to char *.
 */
typedef struct hashmap hashmap;

hashmap *hashmap_new();
/** Sets the mapping between the two 0-terminated strings `key` and `value`. No
 * checking is performed on the two strings.
 */
void hashmap_set(hashmap *hm, char *key, char *value);
/** Returns the corresponding value for this key if present, otherwise returns
 * NULL.
 */
char *hashmap_get(hashmap *hm, char *key);
void hashmap_free(hashmap *hm);
int hashmap_len(hashmap *hm);
/** Returns the key of the i-th element in the hashmap. The order is not by
 * insertion order, but remains the same unless inserts have been performed. You
 * can use this to iterate through all elements, by using hashmap_len to get the
 * upper bound. But note that any inserts during your iteration will mess it up!
 */
char *hashmap_get_i(hashmap *hm, int i);
/** Helper function to print all key value pairs stored in the hashmap.
 */
void printhm(hashmap *hm);

typedef struct graph graph;
graph *graph_new();
void graph_print(graph *g);
int graph_num_nodes(graph *g);
bool graph_contains(graph *g, char *name);
int graph_insert_node(graph *g, char *name);
bool graph_insert_edge(graph *g, char *from, char *to);
graph *graph_subgraph(graph *g, char *ids[], int cnt);
bool graph_dfs(graph *g, char *start, char *target);
int graph_find_sources(graph *g, char *ids[]);
int graph_find_sinks(graph *g, char *ids[]);
bool graph_topo_sort(graph *g, char *ids[]);
void graph_free(graph *g);

#define AOC
#endif
