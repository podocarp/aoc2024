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
#undef LIST_NEW

#define LIST_RESIZE JOIN(METHOD_PREFIX, resize)
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
static void LIST_INSERT(LIST *lst, T item, int index) {
    if (lst->len >= lst->capacity) {
        size_t new_cap = lst->capacity * 2;
        LIST_RESIZE(lst, new_cap);
        lst->items = realloc(lst->items, new_cap * sizeof(T));
        lst->capacity = new_cap;
    }
    for (size_t i = index; i < lst->len; i++) {
        lst->items[i + 1] = lst->items[i];
    }
    lst->items[index] = item;
}

#define LIST_PUSH JOIN(METHOD_PREFIX, push)
/** Appends to the end of the list.
 */
inline static void LIST_PUSH(LIST *lst, T item) {
    LIST_INSERT(lst, item, lst->len++);
}
#undef LIST_RESIZE
#undef LIST_INSERT
#undef LIST_PUSH

#define LIST_POP JOIN(METHOD_PREFIX, pop)
/** Returns the last element in the list. No bounds checking is performed. To
 * prevent serious damage, check `lst->len > 0` before calling.
 */
static T LIST_POP(LIST *lst) { return lst->items[--lst->len]; }

#define LIST_SET JOIN(METHOD_PREFIX, set)
static void LIST_SET(LIST *lst, T item, int index) { lst->items[index] = item; }
#undef LIST_SET

#define LIST_GET JOIN(METHOD_PREFIX, get)
/** Gets the item stored at `index`. Negative indexing is allowed.
 */
static T LIST_GET(LIST *lst, int index) {
    int real_index = index;
    if (index < 0) {
        real_index += lst->len;
    }
    if (real_index < 0 || (unsigned int)real_index >= lst->len) {
        fprintf(stderr, "List index %d out of bounds.", index);
        abort();
    }
    return lst->items[real_index];
}
#undef LIST_GET

#undef T
#undef STRUCT_NAME
#undef LIST
#undef METHOD_PREFIX
#undef ZERO_VALUE
