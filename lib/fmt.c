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

void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j++) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[]) {
    int sign;
    if ((sign = n) < 0) {
        n = -n;
    }

    int i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0) {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}
