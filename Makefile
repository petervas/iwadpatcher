CC			=	gcc
CFLAGS		+=	-D_XOPEN_SOURCE=500 -O2 -Wall -ansi 
LDFLAGS		+=	-lbz2

all:		iwadpatcher

iwadpatcher:	bspatchlib.o iwadpatcher.o md5.o patchermain.o patches.o
	$(CC) $(LDFLAGS) bspatchlib.o iwadpatcher.o md5.o patchermain.o patches.o -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f iwadpatcher bspatchlib.o iwadpatcher.o md5.o patchermain.o patches.o
