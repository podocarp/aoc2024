#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 20

int list_a[MAX_LINES], list_b[MAX_LINES];

void part1() {
    quicksort_int(list_a, MAX_LINES);
    quicksort_int(list_b, MAX_LINES);

    int sum = 0;
    for (int i = 0; i < MAX_LINES; i++) {
        sum += abs(list_a[i] - list_b[i]);
    }

    printf("%d\n", sum);
}

void part2() {
    quicksort_int(list_a, MAX_LINES);
    quicksort_int(list_b, MAX_LINES);

    int score = 0;
    int i, j;
    for (i = j = 0; i < MAX_LINES; i++) {
        int target = list_a[i];
        for (; j < MAX_LINES; j++) {
            int b = list_b[j];
            if (b < target) {
                continue;
            } else if (list_b[j] == target) {
                score += target;
            } else {
                break;
            }
        }
    }

    printf("%d\n", score);
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

    char line[MAX_LINE_LENGTH];
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%d\t%d", list_a + i, list_b + i);
        i++;
    }

    // part1();
    part2();

    return 0;
}
