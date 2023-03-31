CC=gcc
VERSION=$(shell git describe --tags)
CFLAGS=-I. -Wall -Wextra -DVERSION=\"$(VERSION)\"
LDFLAGS=

HEADERS=$(wildcard *.h)
SRC=$(filter-out %_test.c,$(wildcard *.c))
OBJ=$(SRC:%.c=%.o)

.PHONY: all clean

all: cnake

cnake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o cnake
