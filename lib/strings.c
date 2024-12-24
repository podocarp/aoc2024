#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Checks if `str` starts with `match` up to `maxlen` characters.
 */
bool strprefix(char *str, char *match, int maxlen) {
    while (maxlen > 0) {
        if (*match == '\0') {
            return true;
        }
        if (*str == '\0') {
            return false;
        }
        if (*str != *match) {
            return false;
        }
        str++;
        match++;
        maxlen--;
    }
    return true;
}
