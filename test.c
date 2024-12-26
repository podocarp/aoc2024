#include "aoc.h"
#include <assert.h>
#include <stdio.h>
#include <strings.h>

#define TEST(fun)                                                              \
    {                                                                          \
        fprintf(stderr, "Testing %s... ", #fun);                               \
        fun();                                                                 \
        fprintf(stderr, "Done.\n");                                            \
    }
#define INFO(msg)                                                              \
    fprintf(stderr, "info: %s@%s:%d: %s\n", __func__, __FILE__, __LINE__, info);

void test_strprefix() {
    bool res;
    res = strprefix("test one", "test", 8);
    assert(res);

    res = strprefix("test two", "est", 8);
    assert(!res);

    res = strprefix("test three", "test", 3);
    assert(res);

    res = strprefix("test", "test four", 8);
    assert(!res);

    // empty string is prefix of all strings as vacuous case
    res = strprefix("test five", "", 8);
    assert(res);

    res = strprefix("", "test six", 8);
    assert(!res);
}

#define T int
#define LIST list_int
#define ZERO_VALUE 0
#include "list.h"

void test_list_int() {
    list_int *l = list_int_new(1);
    for (int i = 0; i < 1000; i++) {
        list_int_push(l, i);
    }
    for (int i = 999; i >= 0; i--) {
        assert(list_int_pop(l) == i);
    }
}

#define T list_int *
#define LIST list_list_int
#define ZERO_VALUE NULL
#include "list.h"

void test_list_list_int() {
    list_list_int *ll = list_list_int_new(2);
    for (int i = 0; i < 100; i++) {
        list_int *l = list_int_new(1);
        for (int j = 0; j < 1000; j++) {
            list_int_push(l, j);
        }
        list_list_int_push(ll, l);
    }
    for (int i = 99; i >= 0; i--) {
        list_int *l = list_list_int_pop(ll);
        for (int j = 999; j >= 0; j--) {
            assert(list_int_pop(l) == j);
        }
    }
}

void test_hashmap() {
    struct hashmap *hm = hashmap_new();
    char *testsuite[][2] = {
        {"asd", "hello world"},
        {"asd 2", "hello world 2"},
        {"test 1", "hai"},
        {"test 2", "testing 2"},
        {"test 3", "testing"},
        {"test 4", ""},
        {"", "empty"},
        {"a", "b"},
        {"c", "d"},
        {"e", "f"},
        {"g", "h"},
    };
    int len = sizeof(testsuite) / sizeof(testsuite[0]);

    for (int i = 0; i < len; i++) {
        hashmap_set(hm, testsuite[i][0], testsuite[i][1]);
    }

    for (int i = 0; i < len; i++) {
        char *res = hashmap_get(hm, testsuite[i][0]);
        assert(res != NULL);
        assert(strcmp(res, testsuite[i][1]) == 0);
    }

    hashmap_free(hm);
}

void test_graph_fully_connected() {
    graph *g = graph_new();
    char *nodes[] = {"A", "B", "C", "D"};
    int numNodes = sizeof(nodes) / sizeof(nodes[0]);

    for (int i = 0; i < numNodes; i++) {
        graph_insert_node(g, nodes[i]);
    }

    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i != j) {
                assert(graph_insert_edge(g, nodes[i], nodes[j]));
            }
        }
    }

    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            assert(graph_dfs(g, nodes[i], nodes[j]));
        }
    }

    graph_free(g);
}

void test_graph() {
    graph *g = graph_new();
    graph_insert_node(g, "A");
    assert(graph_contains(g, "A"));
    graph_insert_node(g, "B");
    assert(graph_contains(g, "B"));
    graph_insert_node(g, "C");
    assert(graph_contains(g, "C"));
    graph_insert_node(g, "D");
    assert(graph_contains(g, "D"));
    graph_insert_node(g, "E");
    assert(graph_contains(g, "E"));
    graph_insert_node(g, "F");
    assert(graph_contains(g, "F"));

    graph_insert_edge(g, "A", "B");
    graph_insert_edge(g, "A", "C");
    graph_insert_edge(g, "B", "D");
    graph_insert_edge(g, "C", "E");
    graph_insert_edge(g, "D", "A");
    graph_insert_edge(g, "F", "B");
    graph_insert_edge(g, "F", "C");

    assert(graph_dfs(g, "A", "D"));
    assert(!graph_dfs(g, "E", "A"));
    assert(!graph_dfs(g, "A", "F"));

    graph_free(g);
}

void test_subgraph() {
    graph *g = graph_new();
    graph_insert_node(g, "A");
    graph_insert_node(g, "B");
    graph_insert_node(g, "C");
    graph_insert_node(g, "D");
    graph_insert_node(g, "E");
    graph_insert_node(g, "F");

    graph_insert_edge(g, "A", "C");
    graph_insert_edge(g, "B", "D");
    graph_insert_edge(g, "D", "E");
    graph_insert_edge(g, "F", "A");

    char *nodes[] = {"A", "B", "C"};
    graph *s = graph_subgraph(g, nodes, sizeof(nodes) / sizeof(nodes[0]));

    assert(graph_contains(s, "A"));
    assert(graph_contains(s, "B"));
    assert(graph_contains(s, "C"));
    assert(!graph_contains(s, "D"));
    assert(!graph_contains(s, "E"));
    assert(!graph_contains(s, "F"));
    assert(graph_dfs(s, "A", "C"));
    assert(!graph_dfs(s, "A", "B"));

    graph_free(g);
    graph_free(s);
}

void test_graph_sinks_sources() {
    graph *g = graph_new();
    graph_insert_node(g, "A");
    graph_insert_node(g, "B");
    graph_insert_node(g, "C");
    graph_insert_node(g, "D");
    graph_insert_node(g, "E");
    graph_insert_node(g, "F");

    graph_insert_edge(g, "A", "B");
    graph_insert_edge(g, "B", "D");
    graph_insert_edge(g, "D", "E");
    graph_insert_edge(g, "F", "A");

    char *ids[6];
    int cnt = graph_find_sinks(g, ids);
    assert(cnt == 2);
    graph *sg = graph_subgraph(g, ids, cnt);
    assert(graph_contains(sg, "C"));
    assert(graph_contains(sg, "E"));
    assert(!graph_contains(sg, "A"));
    assert(!graph_contains(sg, "B"));

    cnt = 0;
    cnt = graph_find_sources(g, ids);
    assert(cnt == 2);
    sg = graph_subgraph(g, ids, cnt);
    assert(graph_contains(sg, "C"));
    assert(graph_contains(sg, "F"));
    assert(!graph_contains(sg, "A"));
    assert(!graph_contains(sg, "B"));

    graph_free(g);
    graph_free(sg);
}

void test_graph_toposort() {
    graph *g = graph_new();
    graph_insert_node(g, "A");
    graph_insert_node(g, "B");
    graph_insert_node(g, "C");
    graph_insert_node(g, "D");
    graph_insert_node(g, "E");
    graph_insert_node(g, "F");

    graph_insert_edge(g, "A", "B");
    graph_insert_edge(g, "B", "D");
    graph_insert_edge(g, "D", "C");
    graph_insert_edge(g, "C", "E");
    graph_insert_edge(g, "E", "F");

    char *ans[] = {"A", "B", "D", "C", "E", "F"};
    char *res[6];
    assert(graph_topo_sort(g, res));

    for (int i = 0; i < 6; i++) {
        assert(strcmp(ans[i], res[i]) == 0);
    }
    graph_free(g);
}

int main() {
    TEST(test_strprefix)
    TEST(test_list_int)
    TEST(test_list_list_int)
    TEST(test_graph_fully_connected)
    TEST(test_hashmap)
    TEST(test_graph)
    TEST(test_subgraph)
    TEST(test_graph_sinks_sources)
    TEST(test_graph_toposort)
}
