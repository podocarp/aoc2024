#include "aoc.h"
#include <assert.h>

void test_strprefix() {
    bool res;
    res = strprefix("test one", "test", 8);
    assert(res);

    res = strprefix("test two", "est", 8);
    assert(!res);

    res = strprefix("test three", "test", 3);
    assert(res);

    res = strprefix("test", "test four", 8);
    assert(!res);

    // empty string is prefix of all strings as vacuous case
    res = strprefix("test five", "", 8);
    assert(res);

    res = strprefix("", "test six", 8);
    assert(!res);
}

int main() { test_strprefix(); }
