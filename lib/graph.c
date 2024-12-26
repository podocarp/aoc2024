#include "aoc.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define T int
#define LIST list_int
#define ZERO_VALUE 0
#include "list.h"

#define T list_int *
#define LIST list_list_int
#define ZERO_VALUE NULL
#include "list.h"

/** Nodes are internally just numbered 0, 1, ..., etc.
 * The adj_list is a list of list of ints, such that adj_list[id] gives a list
 * representing the successors to node `id`.
 */
typedef struct graph {
    int num_nodes;
    int num_edges;
    hashmap *id_to_name;
    hashmap *name_to_id;
    list_list_int *adj_list;
    list_list_int *adj_list_rev;
} graph;

graph *graph_new() {
    graph *g = malloc(sizeof(graph));
    g->num_nodes = 0;
    g->id_to_name = hashmap_new();
    g->name_to_id = hashmap_new();
    g->adj_list = list_list_int_new(10);
    g->adj_list_rev = list_list_int_new(10);

    return g;
}

void graph_free(graph *g) {
    hashmap_free(g->id_to_name);
    hashmap_free(g->name_to_id);
    for (uint i = 0; i < g->adj_list->len; i++) {
        list_int_free(g->adj_list->items[i]);
        list_int_free(g->adj_list_rev->items[i]);
    }
    list_list_int_free(g->adj_list);
    list_list_int_free(g->adj_list_rev);
    free(g);
}

inline int graph_num_nodes(graph *g) { return g->num_nodes; }

static char tmpbuf[20];
static void hashmap_set_is(hashmap *hm, int key, char *value) {
    itoa(key, tmpbuf);
    hashmap_set(hm, tmpbuf, value);
}
static void hashmap_set_si(hashmap *hm, char *key, int value) {
    itoa(value, tmpbuf);
    hashmap_set(hm, key, tmpbuf);
}

static bool hashmap_contains(hashmap *hm, int key) {
    itoa(key, tmpbuf);
    return hashmap_get(hm, tmpbuf) != NULL;
}

static char *id_to_name(graph *g, int id) {
    itoa(id, tmpbuf);
    return hashmap_get(g->id_to_name, tmpbuf);
}

static int name_to_id(graph *g, char *name) {
    char *tmp = hashmap_get(g->name_to_id, name);
    if (tmp == NULL) {
        return -1;
    }
    return atoi(tmp);
}

graph *graph_copy(graph *g) {
    graph *copy = graph_new();
    for (uint from = 0; from < g->adj_list->len; from++) {
        char *from_name = id_to_name(g, from);
        graph_insert_node(copy, from_name);
        list_int *succs = list_list_int_get(g->adj_list, from);
        for (uint i = 0; i < succs->len; i++) {
            int to = succs->items[i];
            char *to_name = id_to_name(g, to);
            graph_insert_node(copy, to_name);
            graph_insert_edge(copy, from_name, to_name);
        }
    }

    return copy;
}

void graph_print(graph *g) {
    for (uint i = 0; i < g->adj_list->len; i++) {
        char *name = id_to_name(g, i);
        list_int *succs = list_list_int_get(g->adj_list, i);
        if (succs->len > 0) {
            printf("[%s] -> ", name);
        }
        for (uint j = 0; j < succs->len; j++) {
            int succ_id = list_int_get(succs, j);
            char *succ_name = id_to_name(g, succ_id);
            printf("[%s] ", succ_name);
        }
        puts("");
    }
}

bool graph_contains(graph *g, char *name) {
    return hashmap_get(g->name_to_id, name) != NULL;
}

/** Adds the node with `name` into the graph. Nothing is done if it already
 * exists. Returns the id of the node.
 */
int graph_insert_node(graph *g, char *name) {
    int id = name_to_id(g, name);
    if (id >= 0) {
        return id;
    }
    id = g->num_nodes++;
    hashmap_set_is(g->id_to_name, id, name);
    hashmap_set_si(g->name_to_id, name, id);
    list_list_int_push(g->adj_list, list_int_new(0));
    list_list_int_push(g->adj_list_rev, list_int_new(0));
    return id;
}

/** Adds an edge from `from` to `to`. If either `from` or `to` does not exist
 * yet, nothing will happen. Returns true on success.
 */
bool graph_insert_edge(graph *g, char *from, char *to) {
    if (!graph_contains(g, from) || !graph_contains(g, to)) {
        return false;
    }

    int from_id = name_to_id(g, from);
    int to_id = name_to_id(g, to);

    g->num_edges++;
    list_int *succs = list_list_int_get(g->adj_list, from_id);
    list_int_push(succs, to_id);
    list_int *preds = list_list_int_get(g->adj_list_rev, to_id);
    list_int_push(preds, from_id);

    return true;
}

static void graph_remove_edge_i(graph *g, int from, int to) {
    list_int *succs = list_list_int_get(g->adj_list, from);
    for (uint i = 0; i < succs->len; i++) {
        if (succs->items[i] == to) {
            g->num_edges--;
            list_int_delete(succs, i);
        }
    }
    list_int *preds = list_list_int_get(g->adj_list_rev, to);
    for (uint i = 0; i < preds->len; i++) {
        if (preds->items[i] == from) {
            list_int_delete(preds, i);
        }
    }
}

void graph_remove_edge(graph *g, char *from, char *to) {
    if (!graph_contains(g, from) || !graph_contains(g, to)) {
        return;
    }

    int from_id = name_to_id(g, from);
    int to_id = name_to_id(g, to);
    graph_remove_edge_i(g, from_id, to_id);
}

/** Returns the subgraph of `g` formed by the relations between the nodes with
 * ids contained in `ids`. The ids of nodes in the subgraph will not be the
 * same as before. Invalid ids will be silently ignored.
 */
graph *graph_subgraph(graph *g, char *names[], int cnt) {
    graph *subgraph = graph_new();
    for (int i = 0; i < cnt; i++) {
        char *name = names[i];
        if (graph_contains(g, name)) {
            graph_insert_node(subgraph, name);
        }
    }
    for (int i = 0; i < cnt; i++) {
        char *name = names[i];
        int id = name_to_id(g, name);
        if (id < 0) {
            continue;
        }

        list_int *succs = list_list_int_get(g->adj_list, id);
        for (uint j = 0; j < succs->len; j++) {
            char *other_name = id_to_name(g, succs->items[j]);
            graph_insert_edge(subgraph, name, other_name);
        }
    }
    return subgraph;
}

bool graph_dfs(graph *g, char *start, char *target) {
    int start_id = name_to_id(g, start);
    int target_id = name_to_id(g, target);
    if (target_id < 0 || start_id < 0) {
        return false;
    }

    hashmap *visited = hashmap_new();
    list_int *stack = list_int_new(10);
    list_int_push(stack, start_id);

    bool found = false;
    while (stack->len > 0) {
        int node = list_int_pop(stack);
        if (node == target_id) {
            found = true;
            goto DONE;
        }
        if (hashmap_contains(visited, node)) {
            continue;
        }
        hashmap_set_is(visited, node, "");

        list_int *succs = list_list_int_get(g->adj_list, node);
        if (succs != NULL) {
            for (uint i = 0; i < succs->len; i++) {
                list_int_push(stack, succs->items[i]);
            }
        }
    }

DONE:
    hashmap_free(visited);
    list_int_free(stack);
    return found;
}

inline static bool graph_is_source_i(graph *g, int id) {
    return g->adj_list_rev->items[id]->len == 0;
}

bool graph_is_source(graph *g, char *name) {
    int id = name_to_id(g, name);
    return graph_is_source_i(g, id);
}

inline static bool graph_is_sink_i(graph *g, int id) {
    return g->adj_list->items[id]->len == 0;
}

bool graph_is_sink(graph *g, char *name) {
    int id = name_to_id(g, name);
    return graph_is_sink_i(g, id);
}

/** Finds all source nodes (no incoming edges) and puts them into the
 * array `names`. The array must have enough space to store all nodes. Returns
 * the number of nodes found.
 */
int graph_find_sources(graph *g, char *names[]) {
    int cnt = 0;
    for (uint i = 0; i < g->adj_list_rev->len; i++) {
        if (graph_is_source_i(g, i)) {
            char *name = id_to_name(g, i);
            names[cnt++] = name;
        }
    }
    return cnt;
}

static int graph_find_sources_i(graph *g, int ids[]) {
    int cnt = 0;
    for (uint i = 0; i < g->adj_list_rev->len; i++) {
        if (graph_is_source_i(g, i)) {
            ids[cnt++] = i;
        }
    }
    return cnt;
}

/** Finds all sink nodes (no outgoing edges) and puts them into the array
 * `names`. The array must have enough space to store all nodes. Returns the
 * number of nodes found.
 */
int graph_find_sinks(graph *g, char *names[]) {
    int cnt = 0;
    for (uint i = 0; i < g->adj_list->len; i++) {
        if (graph_is_sink_i(g, i)) {
            char *name = id_to_name(g, i);
            names[cnt++] = name;
        }
    }
    return cnt;
}

/** Finds the topological sort of the graph and stores the ids into `ids`.
 * The array must be large enough to contain all nodes.
 * Returns true on success, returns false if there are cycles in the graph.
 */
bool graph_topo_sort(graph *g, char *ids[]) {
    g = graph_copy(g);
    int sources[g->num_nodes];
    int cnt = graph_find_sources_i(g, sources);
    int index = cnt - 1;
    char **ptr = ids;
    while (cnt) {
        int node = sources[index--];
        cnt--;
        *ptr++ = strdup(id_to_name(g, node));

        list_int *succs = list_list_int_get(g->adj_list, node);
        for (uint i = 0; succs->len;) {
            int to_id = succs->items[i];
            graph_remove_edge_i(g, node, to_id);
            if (graph_is_source_i(g, to_id)) {
                sources[++index] = to_id;
                cnt++;
            }
        }
    }

    bool res = g->num_edges == 0;
    graph_free(g);
    return res;
}
