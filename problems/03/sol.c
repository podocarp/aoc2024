#include "aoc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX_LINE_LENGTH 5000

long parseline(char line[MAX_LINE_LENGTH]) {
    char *ptr = line;
    long sum = 0;
    while ((ptr = strstr(ptr, "mul")) != NULL) {
        int a = 0, b = 0, offset = 0;
        sscanf(ptr, "mul(%d,%d)%n", &a, &b, &offset);
        if (offset != 0) {
            ptr += offset;
            sum += a * b;
        } else {
            // skip "mul"
            ptr += 3;
        }
    }

    return sum;
}

bool domul = true;
long parseline_with_conditionals(char line[MAX_LINE_LENGTH]) {
    long sum = 0;
    char c;
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        if ((c = line[i]) == '\0') {
            break;
        }

        switch (c) {
        case 'd':
            if (strprefix(line + i, "do()", 5)) {
                domul = true;
            }
            if (strprefix(line + i, "don't()", 8)) {
                domul = false;
            }
            break;
        case 'm':
            if (domul && strprefix(line + i, "mul(", 4)) {
                int a = 0, b = 0, offset = 0;
                sscanf(line + i, "mul(%d,%d)%n", &a, &b, &offset);
                if (offset != 0) {
                    sum += a * b;
                }
            }
            break;
        }
    }
    return sum;
}

FILE *file;
void part1() {
    char str[MAX_LINE_LENGTH];
    long total = 0;
    while (fgets(str, MAX_LINE_LENGTH, file) != NULL) {
        total += parseline(str);
    }
    printf("%ld\n", total);
}

void part2() {
    char str[MAX_LINE_LENGTH];
    long total = 0;
    while (fgets(str, MAX_LINE_LENGTH, file) != NULL) {
        total += parseline_with_conditionals(str);
    }
    printf("%ld\n", total);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Usage: sol input_file");
        return EXIT_FAILURE;
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file.");
        return EXIT_FAILURE;
    }

    part2();
}
