#include "aoc.h"
#include <stddef.h>
#include <stdlib.h>

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
    hashmap *id_to_name;
    hashmap *name_to_id;
    list_list_int *adj_list;
} graph;

graph *graph_new() {
    graph *g = malloc(sizeof(graph));
    g->num_nodes = 0;
    g->id_to_name = hashmap_new();
    g->name_to_id = hashmap_new();
    g->adj_list = list_list_int_new(10);

    return g;
}

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

    list_int *succs = list_list_int_get(g->adj_list, from_id);
    list_int_push(succs, to_id);

    return true;
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

bool graph_bfs(graph *g, char *start, char *target) {
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
    free(stack);
    return found;
}
