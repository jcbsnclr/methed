CSRC:=$(wildcard *.c)
COBJ:=$(patsubst %.c, %.c.o, $(CSRC))

CC?=gcc

CFLAGS:=${CFLAGS} -Og -g -Wall -Wextra -Werror -c -MMD
LFLAGS:=

%.c.o: %.c
	$(CC) $(CFLAGS) $< -o $@

methed: $(COBJ)
	$(CC) $(LFLAGS) $(COBJ) -o $@

.PHONY: all run clean

all: methed

run: methed
	./methed

clean:
	rm -f *.o
	rm -f *.d
	rm -f methed

-include *.d
