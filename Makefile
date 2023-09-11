CC          = gcc
CFLAGS      += -O2 -Wall -Ibsdifflib
LDFLAGS     += -lbz2

SOURCES = iwadpatcher.c md5.c patchermain.c patches.c bsdifflib/bspatchlib.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: iwadpatcher

iwadpatcher: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f iwadpatcher $(OBJECTS)
