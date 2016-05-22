#
# Makefile for Microsoft C++ 8.0/9.0/10.0
#
CC	= cl
CFLAGS	= -Ox -W3 -WX -D_WIN32 -DDLL -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -I. -Ibzip2 -nologo

OBJS	= bspatchlib.obj iwadpatcher.obj md5.obj patches.obj \
	  bzlib.obj blocksort.obj compress.obj crctable.obj \
	  decompress.obj huffman.obj randtable.obj

LIBS	= kernel32.lib
RM	= rm
LFLAGS	= -nologo -DLL -subsystem:windows,4.0 -entry:DllEntryPoint@12

#-------------------------------------------------------------------------

all: iwadpatcher.dll iwadpatcher.exe

#-------------------------------------------------------------------------

clean:
	-@$(RM) *.obj
	-@$(RM) iwadpatcher.lib
	-@$(RM) iwadpatcher.dll
	-@$(RM) iwadpatcher.exe

#-------------------------------------------------------------------------

iwadpatcher.dll: $(OBJS) iwadpatcher.lib
	@echo $(LFLAGS) -out:iwadpatcher.dll $(OBJS) > j.rsp
	@echo iwadpatcher.exp $(LIBS) >> j.rsp
	link @j.rsp
	@$(RM) j.rsp
	@$(RM) iwadpatcher.exp

iwadpatcher.obj: iwadpatcher.c iwadpatcher.h patches.h md5.h bspatchlib.h
	$(CC) $(CFLAGS) -c iwadpatcher.c

md5.obj: md5.c md5.h
	$(CC) $(CFLAGS) -c md5.c

patches.obj: patches.c patches.h
	$(CC) $(CFLAGS) -Zm500 -c patches.c

bspatchlib.obj: bspatchlib.c bspatchlib.h
	$(CC) $(CFLAGS) -c bspatchlib.c

bzlib.obj: bzip2/bzlib.c bzip2/bzlib_private.h bzip2/bzlib.h
	$(CC) $(CFLAGS) -c bzip2/bzlib.c

blocksort.obj: bzip2/blocksort.c bzip2/bzlib_private.h bzip2/bzlib.h
	$(CC) $(CFLAGS) -c bzip2/blocksort.c

compress.obj: bzip2/compress.c bzip2/bzlib_private.h bzip2/bzlib.h
	$(CC) $(CFLAGS) -c bzip2/compress.c

crctable.obj: bzip2/crctable.c bzip2/bzlib_private.h bzip2/bzlib.h
	$(CC) $(CFLAGS) -c bzip2/crctable.c

decompress.obj: bzip2/decompress.c bzip2/bzlib_private.h bzip2/bzlib.h
	$(CC) $(CFLAGS) -c bzip2/decompress.c

huffman.obj: bzip2/huffman.c bzip2/bzlib_private.h bzip2/bzlib.h
	$(CC) $(CFLAGS) -c bzip2/huffman.c

randtable.obj: bzip2/randtable.c bzip2/bzlib_private.h bzip2/bzlib.h
	$(CC) $(CFLAGS) -c bzip2/randtable.c

iwadpatcher.lib: $(OBJS) iwadpatcher.def
	lib -nologo -machine:i386 -def:iwadpatcher.def $(OBJS) -out:iwadpatcher.lib

#-------------------------------------------------------------------------

iwadpatcher.exe: patchermain.c iwadpatcher.h iwadpatcher.lib iwadpatcher.dll
	$(CC) $(CFLAGS) -Feiwadpatcher.exe patchermain.c iwadpatcher.lib
	@$(RM) patchermain.obj

#-------------------------------------------------------------------------

