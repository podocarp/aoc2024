#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define NUM_NODES 100
#define IGNORE_THIS_NODE -1

static char tmpbuf[20];
static char tmpbuf2[20];
static void graph_insert_i(graph *g, int id) {
    itoa(id, tmpbuf);
    graph_insert_node(g, tmpbuf);
}
static void graph_insert_edge_ii(graph *g, int from, int to) {
    itoa(from, tmpbuf);
    itoa(to, tmpbuf2);
    graph_insert_edge(g, tmpbuf, tmpbuf2);
}
graph *graph_subgraph_i(graph *g, int nodes[]) {
    char *ids[NUM_NODES] = {};
    int i = 0;
    for (; i < NUM_NODES; i++) {
        if (nodes[i] == IGNORE_THIS_NODE) {
            break;
        }
        char buf[100];
        itoa(nodes[i], buf);
        ids[i] = strdup(buf);
    }
    for (int j = 0; j < NUM_NODES; j++) {
        if (ids[j] == NULL) {
            break;
        }
    }
    graph *s = graph_subgraph(g, ids, i);
    return s;
}
bool dfs(graph *g, int from, int to) {
    itoa(from, tmpbuf);
    itoa(to, tmpbuf2);
    return graph_dfs(g, tmpbuf, tmpbuf2);
}

bool test_order(graph *g, int nodes[]) {
    graph *sg = graph_subgraph_i(g, nodes);
    int prev = nodes[0];
    for (int i = 1; i < NUM_NODES; i++) {
        int curr = nodes[i];
        if (curr == IGNORE_THIS_NODE) {
            break;
        }
        if (!dfs(sg, prev, curr)) {
            return false;
        }
        prev = curr;
    }

    return true;
}

int fix_order(graph *g, int nodes[]) {
    graph *sg = graph_subgraph_i(g, nodes);
    int len = graph_num_nodes(sg);
    char *sort[100];
    graph_topo_sort(sg, sort);
    char *mid = sort[len / 2];
    return atoi(mid);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Usage: sol input_file");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file.");
        return EXIT_FAILURE;
    }

    char buf[100];
    int sum = 0;
    int sum2 = 0;
    graph *g = graph_new();
    while (fgets(buf, 100, file) != NULL) {
        if (buf[0] == '\n') {
            break;
        }

        int before, after;
        sscanf(buf, "%d|%d", &before, &after);
        graph_insert_i(g, before);
        graph_insert_i(g, after);
        graph_insert_edge_ii(g, before, after);
    }

    while (fgets(buf, 100, file) != NULL) {
        int nodes[NUM_NODES];
        for (int i = 0; i < NUM_NODES; i++) {
            nodes[i] = IGNORE_THIS_NODE;
        }
        char *ptr = buf;
        int offset = 0, cnt = 0;
        while (sscanf(ptr, "%d%*[,]%n", nodes + cnt, &offset) && *ptr) {
            cnt++;
            ptr += offset;
        }

        if (test_order(g, nodes)) {
            sum += nodes[cnt / 2];
        } else {
            sum2 += fix_order(g, nodes);
        }
    }

    printf("%d\n%d\n", sum, sum2);
}
