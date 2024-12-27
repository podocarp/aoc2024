#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>

#define DIM 131
#define BOUNDARY '?'
char map[DIM][DIM];
char map_bkp[DIM][DIM];

enum dir {
    N = 1,
    E = 2,
    S = 3,
    W = 4,
};

typedef struct guard {
    int x;
    int y;
    int dir;
} status;
status guard_status;

void map_backup() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            map_bkp[i][j] = map[i][j];
        }
    }
}

void map_restore() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            map[i][j] = map_bkp[i][j];
        }
    }
}

void print_map() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (j == guard_status.x && i == guard_status.y) {
                switch (guard_status.dir) {
                case N:
                    printf("%c", '^');
                    break;
                case S:
                    printf("%c", 'v');
                    break;
                case E:
                    printf("%c", '>');
                    break;
                case W:
                    printf("%c", '<');
                    break;
                }
            } else {
                printf("%c", map[i][j]);
            }
        }
        puts("");
    }
    puts("");
}

bool bounds_check(int x, int y) {
    if (x < 0 || x >= DIM) {
        return false;
    }
    if (y < 0 || y >= DIM) {
        return false;
    }
    return true;
}

void change_dir() {
    guard_status.dir++;
    if (guard_status.dir > W) {
        guard_status.dir = N;
    }
}

void hm_add(hashmap *hm) {
    char buf[15];
    snprintf(buf, 15, "%d,%d,%d", guard_status.x, guard_status.y,
             guard_status.dir);
    hashmap_set(hm, buf, "");
}

bool hm_exists(hashmap *hm) {
    char buf[15];
    snprintf(buf, 15, "%d,%d,%d", guard_status.x, guard_status.y,
             guard_status.dir);
    return hashmap_get(hm, buf) != NULL;
}

/** Either returns the number of unique tiles the guard takes or -1 if there is
 * an infinite loop
 */
int guard_walk() {
    int tiles = 0;
    int newx, newy;
    hashmap *visited = hashmap_new();
    while (1) {
        if (hm_exists(visited)) {
            tiles = -1;
            goto DONE;
        }
        hm_add(visited);

        newx = guard_status.x;
        newy = guard_status.y;
        switch (guard_status.dir) {
        case N:
            newy--;
            break;
        case S:
            newy++;
            break;
        case E:
            newx++;
            break;
        case W:
            newx--;
            break;
        }

        if (!bounds_check(newx, newy)) {
            goto DONE;
        }
        char newtile = map[newy][newx];
        switch (newtile) {
        case '.':
            tiles++;
            map[newy][newx] = 'X';
            __attribute__((fallthrough));
        case '^':
            __attribute__((fallthrough));
        case 'X':
            guard_status.x = newx;
            guard_status.y = newy;
            break;
        case '#':
            change_dir();
            break;
        case BOUNDARY:
            goto DONE;
        default:
            printf("unknown char %c", newtile);
        }
    }
DONE:
    hashmap_free(visited);
    return tiles;
}

void part1() {
    // the current tile starts off as marked
    map[guard_status.y][guard_status.x] = 'X';
    int res = 1;
    res += guard_walk();
    printf("%d\n", res);
}

int test_obstacle(int x, int y) {
    map_backup();
    map[y][x] = '#';
    int res = guard_walk();
    map_restore();
    return res == -1 ? 1 : 0;
}

void part2() {
    status starting_status = guard_status;
    // generate path taken by guard first
    // then for every spot of the path, try placing an obstacle
    guard_walk();
    int res = 0;
    for (int y = 0; y < DIM; y++) {
        for (int x = 0; x < DIM; x++) {
            if (map[y][x] != 'X') {
                continue;
            }
            guard_status = starting_status;
            res += test_obstacle(x, y);
        }
    }
    printf("%d\n", res);
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

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            map[i][j] = BOUNDARY;
        }
    }

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            char c = fgetc(file);
            if (c == '\n' || c == EOF) {
                break;
            }

            map[i][j] = c;
            if (c == '^') {
                guard_status.y = i;
                guard_status.x = j;
                guard_status.dir = N;
            }
        }
    }

    // part1();
    part2();
}
