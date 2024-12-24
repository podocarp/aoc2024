#ifndef AOC
#include <stdbool.h>
#include <stddef.h>

typedef unsigned int uint;
typedef unsigned long ulong;

#define putsi(i) printf("%d\n", i);

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
void hashmap_set(hashmap *hm, char *key, char *value);
char *hashmap_get(hashmap *hm, char *key);
void hashmap_free(hashmap *hm);
void printhm(hashmap *hm);

typedef struct graph graph;
graph *graph_new();
bool graph_contains(graph *g, char *name);
int graph_insert_node(graph *g, char *name);
bool graph_insert_edge(graph *g, char *from, char *to);
graph *graph_subgraph(graph *g, char *ids[], int cnt);
bool graph_bfs(graph *g, char *start, char *target);

#define AOC
#endif
