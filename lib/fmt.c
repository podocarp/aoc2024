#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Formats an array with sprintf.
 *
 * @param fmt An sprintf flag like %d or %s describing how to print each
 * element.
 * @param arr The array.
 * @param elem_size The size of each element.
 * @return [TODO:return]
 */
void printarr(char *fmt, void *arr, size_t elem_size, size_t elem_len) {
    fputs("[", stdout);
    for (size_t i = 0; i < elem_len; i++) {
        void *current_elem = (char *)arr + i * elem_size;
        printf(fmt, *(void **)current_elem);

        if (i != elem_len - 1) {
            printf(" ");
        }
    }
    puts("]");
}

void printstr(char *str, int maxlen) {
    int i = 0;
    fputs("\"", stdout);
    while (*str != '\0' && i < maxlen) {
        switch (*str) {
        case '\n':
            printf("\\n");
            break;
        default:
            printf("%c", *str);
        }
        str++;
        i++;
    }
    puts("\"");
}