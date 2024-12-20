#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>

#define DIM 150

char matrix[DIM][DIM];

bool check_left(int i, int j) {
    return matrix[i - 1][j] == 'M' && matrix[i - 2][j] == 'A' &&
           matrix[i - 3][j] == 'S';
}
bool check_right(int i, int j) {
    return matrix[i + 1][j] == 'M' && matrix[i + 2][j] == 'A' &&
           matrix[i + 3][j] == 'S';
}
bool check_up(int i, int j) {
    return matrix[i][j - 1] == 'M' && matrix[i][j - 2] == 'A' &&
           matrix[i][j - 3] == 'S';
}
bool check_down(int i, int j) {
    return matrix[i][j + 1] == 'M' && matrix[i][j + 2] == 'A' &&
           matrix[i][j + 3] == 'S';
}
bool check_nw(int i, int j) {
    return matrix[i - 1][j - 1] == 'M' && matrix[i - 2][j - 2] == 'A' &&
           matrix[i - 3][j - 3] == 'S';
}
bool check_ne(int i, int j) {
    return matrix[i - 1][j + 1] == 'M' && matrix[i - 2][j + 2] == 'A' &&
           matrix[i - 3][j + 3] == 'S';
}
bool check_sw(int i, int j) {
    return matrix[i + 1][j - 1] == 'M' && matrix[i + 2][j - 2] == 'A' &&
           matrix[i + 3][j - 3] == 'S';
}
bool check_se(int i, int j) {
    return matrix[i + 1][j + 1] == 'M' && matrix[i + 2][j + 2] == 'A' &&
           matrix[i + 3][j + 3] == 'S';
}

void part1() {
    int cnt = 0;
    for (int i = 3; i < DIM; i++) {
        for (int j = 3; j < DIM; j++) {
            if (matrix[i][j] != 'X')
                continue;

            if (check_up(i, j))
                cnt++;
            if (check_down(i, j))
                cnt++;
            if (check_left(i, j))
                cnt++;
            if (check_right(i, j))
                cnt++;
            if (check_nw(i, j))
                cnt++;
            if (check_ne(i, j))
                cnt++;
            if (check_sw(i, j))
                cnt++;
            if (check_se(i, j))
                cnt++;
        }
    }
    printf("%d\n", cnt);
}

bool check_forward_slash(int i, int j) {
    char left = matrix[i - 1][j - 1];
    char right = matrix[i + 1][j + 1];
    return (left == 'M' && right == 'S') || (left == 'S' && right == 'M');
}

bool check_back_slash(int i, int j) {
    char left = matrix[i + 1][j - 1];
    char right = matrix[i - 1][j + 1];
    return (left == 'M' && right == 'S') || (left == 'S' && right == 'M');
}

void part2() {
    int cnt = 0;
    for (int i = 3; i < DIM; i++) {
        for (int j = 3; j < DIM; j++) {
            if (matrix[i][j] != 'A')
                continue;
            if (check_forward_slash(i, j) && check_back_slash(i, j))
                cnt++;
        }
    }
    printf("%d\n", cnt);
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
            matrix[i][j] = '-';
        }
    }

    // pad the thing sufficiently so we have easy life later
    for (int i = 3; i < DIM; i++) {
        if (fgets(matrix[i] + 3, DIM, file) == NULL) {
            break;
        }
    }

    // part1();
    part2();
}
