#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define MAX_OPERANDS 20
#define MAX_LINE_LEN 50
#define NUM_LINES 850

typedef struct line {
    ulong target;
    uint operands[MAX_OPERANDS];
    int noperands;
} line;

line lines[NUM_LINES];

void printlines() {
    for (int i = 0; i < NUM_LINES; i++) {
        line l = lines[i];
        printf("%lu: ", l.target);
        for (int j = 0; j < l.noperands; j++) {
            printf("%u", l.operands[j]);
            if (j != l.noperands - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

bool satisfiable_2(line l) {
    // use bits to encode the different choices for the operators
    // 0 = addition, 1 = multiplication
    int noperators = l.noperands - 1;
    for (int bits = 0; bits < 1 << noperators; bits++) {
        ulong value = l.operands[0];
        int mask = 1;
        for (int i = 1; i < l.noperands; i++) {
            int bit = bits & mask;
            if (bit == 0) {
                value = value + l.operands[i];
            } else {
                value = value * l.operands[i];
            }
            mask = mask << 1;
        }
        if (value == l.target) {
            return true;
        }
    }
    return false;
}

bool satisfiable_3(line l) {
    int noperators = l.noperands - 1;
    int ops[noperators + 1];
    for (int i = 0; i < noperators + 1; i++) {
        ops[i] = 0;
    }

    while (ops[noperators] == 0) {
        ulong value = l.operands[0];
        for (int i = 1; i < l.noperands; i++) {
            int op = ops[i - 1];
            if (op == 0) {
                value = value + l.operands[i];
            } else if (op == 1) {
                value = value * l.operands[i];
            } else {
                char buf[30];
                sprintf(buf, "%lu%d", value, l.operands[i]);
                sscanf(buf, "%lu", &value);
            }
        }

        if (value == l.target) {
            return true;
        }

        int carry = 0;
        int i = 0;
        do {
            ops[i]++;
            if (ops[i] > 2) {
                ops[i] = 0;
                carry = 1;
            } else {
                carry = 0;
            }
            i++;
        } while (carry != 0);
    }

    return false;
}

void part1() {
    ulong total = 0;
    for (int i = 0; i < NUM_LINES; i++) {
        if (satisfiable_2(lines[i])) {
            total += lines[i].target;
        }
    }

    printf("%lu\n", total);
}

void part2() {
    ulong total = 0;
    for (int i = 0; i < NUM_LINES; i++) {
        if (satisfiable_3(lines[i])) {
            total += lines[i].target;
        }
    }

    printf("%lu\n", total);
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

    char buf[MAX_LINE_LEN];
    int i = 0;
    while (fgets(buf, MAX_LINE_LEN, file) != NULL) {
        line l = {0, {}, 0};
        int offset = 0;
        sscanf(buf, "%ld: %n", &l.target, &offset);

        char *ptr = buf + offset;
        char *token;
        int j = 0;
        while ((token = strsep(&ptr, " \n")) != NULL && token[0] != '\0') {
            l.operands[j] = atoi(token);
            j++;
        }
        l.noperands = j;

        lines[i] = l;
        i++;
    }

    // part1();
    part2();
}
