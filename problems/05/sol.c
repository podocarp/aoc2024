#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_NODES 100
#define IGNORE_THIS_NODE -1

bool adj_matrix[NUM_NODES][NUM_NODES];

void add_edge(int from, int to) {
    if (to == IGNORE_THIS_NODE || from == IGNORE_THIS_NODE) {
        return;
    }
    adj_matrix[from][to] = true;
}

bool has_edge(int from, int to) {
    if (to == IGNORE_THIS_NODE || from == IGNORE_THIS_NODE) {
        return false;
    }
    return adj_matrix[from][to];
}

bool is_connected(int from, int to) {
    int stack[NUM_NODES];
    int *ptr = stack;
    for (int i = 0; i < NUM_NODES; i++) {
        stack[i] = IGNORE_THIS_NODE;
    }
    *ptr++ = from;

    while (ptr != stack) {
        int node = *--ptr;
        if (node == to) {
            return true;
        }

        for (int i = 0; i < NUM_NODES; i++) {
            if (has_edge(node, i)) {
                *ptr++ = i;
            }
        }
    }
    return false;
}

// TODO: What about those that you should ignore?
bool test_order(int *nodes) {
    int prev = nodes[0];
    for (int i = 1; i < NUM_NODES; i++) {
        int curr = nodes[i];
        if (curr == IGNORE_THIS_NODE) {
            break;
        }
        if (!is_connected(prev, curr)) {
            return false;
        }
    }

    return true;
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

    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = 0; j < NUM_NODES; j++) {
            adj_matrix[i][j] = false;
        }
    }

    char buf[100];
    int mode = 1;
    int sum = 0;
    while (fgets(buf, 100, file) != NULL) {
        if (buf[0] == '\n') {
            mode = 2;
            continue;
        }

        if (mode == 1) {
            int before, after;
            sscanf(buf, "%d|%d", &before, &after);
            add_edge(before, after);
        } else if (mode == 2) {
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

            if (test_order(nodes)) {
                sum += nodes[cnt / 2];
            }
        }
    }

    printf("%d\n", sum);
}
