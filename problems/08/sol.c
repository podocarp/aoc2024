#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 50
// extra space for newline and null character
#define LINELEN (DIM + 2)
#define COORDSLEN 30

char map[DIM][DIM] = {0};

hashmap *letters_done;
hashmap *antinodes;

typedef struct coord {
    int x, y;
} coord;

bool has_antinode(int x, int y) {
    char key[10];
    sprintf(key, "%d %d", x, y);
    return hashmap_get(antinodes, key) != NULL;
}

void add_antinode(int x, int y) {
    char key[10];
    sprintf(key, "%d %d", x, y);
    hashmap_set(antinodes, key, "");
}

void printmap() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (has_antinode(i, j)) {
                printf("#");
            } else {
                printf("%c", map[i][j]);
            }
        }
        puts("");
    }
}

bool check_bounds(coord c) {
    if (c.x < 0 || c.x >= DIM) {
        return false;
    }
    if (c.y < 0 || c.y >= DIM) {
        return false;
    }
    if (map[c.x][c.y] == '\0') {
        return false;
    }
    return true;
}

void register_antinodes(coord c1, coord c2) {
    int deltax = c2.x - c1.x;
    int deltay = c2.y - c1.y;
    coord antinode1 = {.x = c1.x - deltax, .y = c1.y - deltay};
    coord antinode2 = {.x = c2.x + deltax, .y = c2.y + deltay};
    if (check_bounds(antinode1)) {
        add_antinode(antinode1.x, antinode1.y);
    }
    if (check_bounds(antinode2)) {
        add_antinode(antinode2.x, antinode2.y);
    }
}

void register_resonant_antinodes(coord c1, coord c2) {
    int deltax = c2.x - c1.x;
    int deltay = c2.y - c1.y;
    coord antinode1 = c1;
    coord antinode2 = c2;
    while (check_bounds(antinode1)) {
        add_antinode(antinode1.x, antinode1.y);
        antinode1.x = antinode1.x - deltax;
        antinode1.y = antinode1.y - deltay;
    }
    while (check_bounds(antinode2)) {
        add_antinode(antinode2.x, antinode2.y);
        antinode2.x = antinode2.x + deltax;
        antinode2.y = antinode2.y + deltay;
    }
}

void find_antinodes(char c, bool resonant) {
    char key[2] = {c, '\0'};
    if (hashmap_get(letters_done, key) != NULL) {
        return;
    }

    coord coords[COORDSLEN] = {0};
    int z = 0;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (map[i][j] == c) {
                coord tmp = {.x = i, .y = j};
                coords[z] = tmp;
                z++;
            }
        }
    }

    for (int i = 0; i < z; i++) {
        for (int j = i + 1; j < z; j++) {
            if (resonant) {
                register_resonant_antinodes(coords[i], coords[j]);
            } else {
                register_antinodes(coords[i], coords[j]);
            }
        }
    }

    hashmap_set(letters_done, key, "");
}

void part1() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            char c = map[i][j];
            if (c == '.' || c == '\0') {
                continue;
            }
            find_antinodes(c, false);
        }
    }

    printf("%d\n", hashmap_len(antinodes));
}

void part2() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            char c = map[i][j];
            if (c == '.' || c == '\0') {
                continue;
            }
            find_antinodes(c, true);
        }
    }

    printf("%d\n", hashmap_len(antinodes));
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

    letters_done = hashmap_new();
    antinodes = hashmap_new();

    int i = 0;
    char buf[LINELEN] = {0};
    while (fgets(buf, LINELEN, file) != NULL) {
        for (int j = 0; j < DIM; j++) {
            if (buf[j] == '\n') {
                break;
            }
            map[i][j] = buf[j];
        }
        i++;
    }

    // part1();
    part2();
}
