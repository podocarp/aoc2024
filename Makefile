CC = clang
CFLAGS = -Wall -Wextra -Werror -Ilib

SHARED_SRC = lib/sort.c
SHARED_HEADERS = lib/aoc.h

SRC_DIRS := $(wildcard problems/*/)
SRC_FILES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)*.c))
TARGETS := $(SRC_FILES:.c=)

all: $(TARGETS)
	
$(TARGETS): % : %.c $(SHARED_SRC) $(SHARED_HEADERS)
	$(CC) $(CFLAGS) -o $@ $< $(SHARED_SRC)

clean:
	rm -f $(TARGETS)

.PHONY: all clean
