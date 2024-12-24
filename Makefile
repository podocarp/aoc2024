CC = clang
CFLAGS = -Wall -Wextra -Werror -Wno-unused-function -Ilib -g -O0

SHARED_SRC = $(wildcard lib/*.c)
SHARED_HEADERS = lib/aoc.h lib/list.h

SRC_DIRS := $(wildcard problems/*/)
SRC_FILES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)sol.c))
TARGETS := $(SRC_FILES:.c=)

all: $(TARGETS)
	
$(TARGETS): % : %.c $(SHARED_SRC) $(SHARED_HEADERS)
	$(CC) $(CFLAGS) -o $@ $< $(SHARED_SRC)

test: test.c $(SHARED_SRC) $(SHARED_HEADERS)
	$(CC) $(CFLAGS) -o test $(SHARED_SRC) test.c

clean:
	rm -f $(TARGETS)

.PHONY: all clean
