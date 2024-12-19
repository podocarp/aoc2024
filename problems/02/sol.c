#include "aoc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 100
#define MAX_REPORTS 10

enum {
    UNKNOWN = 0,
    INCREASING,
    DECREASING,
};

#define IGNORE_THIS_ELEMENT -1

int reports[MAX_LINES][MAX_REPORTS];

bool check_row_safe(int row[MAX_REPORTS]) {
    int prev, i = 0;
    for (; i < MAX_REPORTS; ++i) {
        prev = row[i];
        if (prev != IGNORE_THIS_ELEMENT) {
            break;
        }
    }
    i++;

    int direction = UNKNOWN;
    for (; i < MAX_REPORTS; i++) {
        int curr = row[i];
        if (curr == IGNORE_THIS_ELEMENT) {
            continue;
        }

        int curr_direction = UNKNOWN;
        if (curr > prev) {
            curr_direction = INCREASING;
        } else if (curr < prev) {
            curr_direction = DECREASING;
        } else {
            return false;
        }

        if (direction == UNKNOWN) {
            direction = curr_direction;
        } else if (curr_direction != direction) {
            return false;
        }

        int diff = abs(prev - curr);
        if (diff > 3) {
            return false;
        }

        prev = curr;
    }
    return true;
}

bool check_safe_ignoring(int row[MAX_REPORTS], int index_to_ignore) {
    if (index_to_ignore < 0 || index_to_ignore >= MAX_REPORTS) {
        return false;
    }
    int tmp = row[index_to_ignore];
    row[index_to_ignore] = IGNORE_THIS_ELEMENT;
    bool result = check_row_safe(row);
    row[index_to_ignore] = tmp;
    return result;
}

// - Equal levels can be removed on first sight.
//
// - If diff is larger than 3, have to test either removing prev or current.
//
// - If direction is different, test removing the 3 points at and around the
// direction change.
bool check_safe_with_damping(int row[MAX_REPORTS]) {
    int prev = row[0];
    if (prev == IGNORE_THIS_ELEMENT) {
        return false;
    }

    int direction = UNKNOWN;
    for (int i = 1; i < MAX_REPORTS; i++) {
        int curr = row[i];
        if (curr == IGNORE_THIS_ELEMENT) {
            break;
        }

        int curr_direction = UNKNOWN;
        if (curr > prev) {
            curr_direction = INCREASING;
        } else if (curr < prev) {
            curr_direction = DECREASING;
        } else {
            return check_safe_ignoring(row, i);
        }

        if (direction == UNKNOWN) {
            direction = curr_direction;
        } else if (curr_direction != direction) {
            return check_safe_ignoring(row, i) ||
                   check_safe_ignoring(row, i - 1) ||
                   check_safe_ignoring(row, i - 2);
        }

        int diff = abs(prev - curr);
        if (diff > 3) {
            return check_safe_ignoring(row, i) ||
                   check_safe_ignoring(row, i - 1);
        }

        prev = curr;
    }

    return true;
}

void part1() {
    int safe_cnt = 0;
    for (int i = 0; i < MAX_LINES; i++) {
        if (check_row_safe(reports[i])) {
            safe_cnt++;
        }
    }
    printf("%d\n", safe_cnt);
}

void part2() {
    int safe_cnt = 0;
    for (int i = 0; i < MAX_LINES; i++) {
        if (check_safe_with_damping(reports[i])) {
            safe_cnt++;
        }
    }
    printf("%d\n", safe_cnt);
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
    int lineno = 0;
    for (int i = 0; i < MAX_LINES; i++) {
        for (int j = 0; j < MAX_REPORTS; j++) {
            reports[i][j] = IGNORE_THIS_ELEMENT;
        }
    }

    while (fgets(line, sizeof(line), file)) {
        int reportno = 0;
        char *ptr = line;
        int offset = 0;
        while (sscanf(ptr, "%d %n", &(reports[lineno][reportno]), &offset) &&
               *ptr) {
            ptr += offset;
            reportno++;
        }

        lineno++;
    }

    part2();
}
