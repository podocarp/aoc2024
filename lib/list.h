#define STRUCT_PREFIX list_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x##y

#ifndef LIST_H
#define LIST_H
#endif

#ifndef T
#error "Use #define T before including this header."
#define T int // just to remove lsp errors below
#endif

#ifndef LIST
#define STRUCT_NAME JOIN(LIST_INTERNAL_DONT_USE_DIRECTLY, T)
#define LIST JOIN(list_, T)
#warning                                                                       \
    "Autogenerating struct name. This can lead to some problems sometimes, esp. with pointers. To prevent any issues define the name beforehand with #define LIST"
#else
#define STRUCT_NAME JOIN(LIST_INTERNAL_, LIST)
#endif

#ifndef ZERO_VALUE
#warning                                                                       \
    "Zero value not defined. This disables some safety features. Use #define ZERO_VALUE to define a sensible zero value for your given value of T."
#endif

struct STRUCT_NAME {
    T *items;
    size_t len;
    size_t capacity;
};

typedef struct STRUCT_NAME LIST;

#define METHOD_PREFIX JOIN(LIST, _)
#define LIST_NEW JOIN(METHOD_PREFIX, new)
static LIST *LIST_NEW(int capacity) {
    LIST *l = malloc(sizeof(LIST));
    if (capacity <= 0) {
        capacity = 1;
    }
    l->items = malloc(capacity * sizeof(T));
    l->len = 0;
    l->capacity = capacity;
#ifdef ZERO_VALUE
    for (int i = 0; i < capacity; i++) {
        l->items[i] = ZERO_VALUE;
    }
#endif
    return l;
}

#define LIST_CONVERT_INDEX JOIN(METHOD_PREFIX, zz_internal_convert_index)
static int LIST_CONVERT_INDEX(LIST *lst, int index) {
    int real_index = index;
    if (index < 0) {
        real_index += lst->len;
    }
    if (real_index < 0 || (unsigned int)real_index >= lst->len) {
        fprintf(stderr, "List index %d out of bounds.", index);
        abort();
    }
    return real_index;
}

#define LIST_RESIZE JOIN(METHOD_PREFIX, zz_internal_resize)
/** Resizes the list. If `cap` is not enough to contain all the elements in the
 * list, nothing is done.
 */
inline static void LIST_RESIZE(LIST *lst, size_t cap) {
    if (cap >= lst->len) {
        lst->items = realloc(lst->items, cap * sizeof(T));
#ifdef ZERO_VALUE
        size_t old_cap = lst->capacity;
        for (size_t i = old_cap - 1; i < cap; i++) {
            lst->items[i] = ZERO_VALUE;
        }
#endif
        lst->capacity = cap;
    }
}

#define LIST_INSERT JOIN(METHOD_PREFIX, insert)
/** Inserts `item` at 0-based position `index`.
 */
__attribute__((unused)) static void LIST_INSERT(LIST *lst, T item, int index) {
    if (lst->len >= lst->capacity) {
        LIST_RESIZE(lst, lst->capacity * 2);
    }
    for (size_t i = index; i < lst->len; i++) {
        lst->items[i + 1] = lst->items[i];
    }
    lst->items[index] = item;
}

#define LIST_PUSH JOIN(METHOD_PREFIX, push)
/** Appends to the end of the list.
 */
__attribute__((unused)) inline static void LIST_PUSH(LIST *lst, T item) {
    LIST_INSERT(lst, item, lst->len++);
}

#define LIST_DELETE JOIN(METHOD_PREFIX, delete)
__attribute__((unused)) static void LIST_DELETE(LIST *lst, int index) {
    int real_index = LIST_CONVERT_INDEX(lst, index);
    for (unsigned int i = real_index; i < lst->len; i++) {
        lst->items[i] = lst->items[i + 1];
    }
    lst->len--;
}

#define LIST_POP JOIN(METHOD_PREFIX, pop)
/** Returns the last element in the list. No bounds checking is performed. To
 * prevent serious damage, check `lst->len > 0` before calling.
 */
__attribute__((unused)) static T LIST_POP(LIST *lst) {
    return lst->items[--lst->len];
}

#define LIST_SET JOIN(METHOD_PREFIX, set)
__attribute__((unused)) static void LIST_SET(LIST *lst, T item, int index) {
    int real_index = LIST_CONVERT_INDEX(lst, index);
    lst->items[real_index] = item;
}

#define LIST_GET JOIN(METHOD_PREFIX, get)
/** Gets the item stored at `index`. Negative indexing is allowed.
 */
__attribute__((unused)) static T LIST_GET(LIST *lst, int index) {
    int real_index = LIST_CONVERT_INDEX(lst, index);
    return lst->items[real_index];
}

#define LIST_FREE JOIN(METHOD_PREFIX, free)
__attribute__((unused)) static void LIST_FREE(LIST *lst) {
    free(lst->items);
    free(lst);
}

#undef LIST_NEW
#undef LIST_DELETE
#undef LIST_SET
#undef LIST_GET
#undef LIST_FREE
#undef LIST_RESIZE
#undef LIST_INSERT
#undef LIST_PUSH
#undef LIST_CONVERT_INDEX
#undef T
#undef STRUCT_NAME
#undef LIST
#undef METHOD_PREFIX
#undef ZERO_VALUE
