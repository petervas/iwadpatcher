# NMake makefile

# Compiler
CC = cl

# Compiler flags
CFLAGS = /nologo /O2 /W3 /Ibzip2 /Ibsdifflib /DDLL /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS

# Linker
LINK = link

# Linker flags
LFLAGS = /nologo /MANIFEST:EMBED /MANIFESTUAC:"level='asInvoker' uiAccess='false'"
LFLAGS_DLL = /nologo /DLL /entry:DllEntryPoint

# Source files
IWADPATCHER_SRCS = iwadpatcher.c md5.c patches.c
BSDIFF_SRCS = bsdifflib\bspatchlib.c
BZIP2_SRCS = bzip2\blocksort.c bzip2\bzlib.c bzip2\compress.c bzip2\crctable.c bzip2\decompress.c bzip2\huffman.c bzip2\randtable.c

# Object files
OBJS = $(IWADPATCHER_SRCS:.c=.obj) $(BSDIFF_SRCS:.c=.obj) $(BZIP2_SRCS:.c=.obj)

# Default target
all: iwadpatcher.exe iwadpatcher.dll

# Compile C source files
.c.obj:
	$(CC) $(CFLAGS) /c /Fo$(@D)\ $<

patches.obj: patches.c
	$(CC) $(CFLAGS) -Zm500 /c patches.c

iwadpatcher.lib: $(OBJS) iwadpatcher.def
	lib /nologo /def:iwadpatcher.def $(OBJS) /out:$@

iwadpatcher.dll: $(OBJS) iwadpatcher.lib
	$(LINK) $(LFLAGS_DLL) /out:$@ $(OBJS) iwadpatcher.exp
	del /Q /F iwadpatcher.exp

iwadpatcher.exe: patchermain.obj iwadpatcher.lib iwadpatcher.dll
	$(LINK) $(LFLAGS) /out:$@ patchermain.obj iwadpatcher.lib

# Clean target
clean:
	del /Q /F iwadpatcher.exe iwadpatcher.dll iwadpatcher.lib patchermain.obj $(OBJS)
