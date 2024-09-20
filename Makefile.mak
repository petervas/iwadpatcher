# NMake makefile

# Compiler
CC = cl

# Compiler flags
CFLAGS = /nologo /O2 /W3 /Ibzip2 /Ibsdifflib /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS

# Linker
LINK = link

# Linker flags
LFLAGS = /nologo /MANIFEST:EMBED /MANIFESTUAC:"level='asInvoker' uiAccess='false'"

# Source files
IWADPATCHER_SRCS = patchermain.c iwadpatcher.c md5.c patches.c
BSDIFF_SRCS = bsdifflib\bspatchlib.c
BZIP2_SRCS = bzip2\blocksort.c bzip2\bzlib.c bzip2\compress.c bzip2\crctable.c bzip2\decompress.c bzip2\huffman.c bzip2\randtable.c

# Object files
IWADPATCHER_OBJS = $(IWADPATCHER_SRCS:.c=.obj)
BSDIFF_OBJS = $(BSDIFF_SRCS:.c=.obj)
BZIP2_OBJS = $(BZIP2_SRCS:.c=.obj)

# Default target
all: iwadpatcher.exe

# Compile C source files
.c.obj:
	$(CC) $(CFLAGS) /c /Fo$(@D)\ $<

# Link object files
iwadpatcher.exe: $(IWADPATCHER_OBJS) $(BSDIFF_OBJS) $(BZIP2_OBJS)
	$(LINK) $(LFLAGS) -out:$@ $(IWADPATCHER_OBJS) $(BSDIFF_OBJS) $(BZIP2_OBJS)

# Clean target
clean:
	del /Q /F iwadpatcher.exe $(BSDIFF_OBJS) $(BZIP2_OBJS) $(IWADPATCHER_OBJS)
