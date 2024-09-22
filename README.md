IWadPatcher 1.3
released on 22 September 2024

IWadPatcher patches any registered IWAD from DOOM, DOOM II, Final DOOM,
Heretic, Hexen and Strife to the latest or any other official version.
Shareware IWADs are not supported and can be downloaded for free.
IWadPatcher is written in cross-platform C and can also be used as a C/C++
library. IWadPatcher is released under the BSD license.
Copyright 2024 Peter Vaskovic <petervaskovic@yahoo.de>

uses independent implementation of MD5 1.6, written by
Copyright 1999-2002 L. Peter Deutsch <ghost@aladdin.com>
available at http://userpages.umbc.edu/~mabzug1/cs/md5/md5.html

I would like to thank Hippocrates Sendoukas for helping me out with some
C related questions and giving me helpful coding suggestions in general.
You can visit his site at http://hs.zdaemon.org/

Compile:

    Linux:   Unpack bsdifflib (https://github.com/petervas/bsdifflib)
             source code to the bsdifflib folder.
             Run make.
    
    Windows: Unpack bzip2 source code to the bzip2 folder.
             Unpack bsdifflib (https://github.com/petervas/bsdifflib)
             source code to the bsdifflib folder.
             Run nmake -f Makefile.mak

Changelog:

    IWadPatcher 1.3 (22 September 2024)
        
        Add support for Unity and KEX edition doom.wad, doom2.wad,
        plutonia.wad and tnt.wad.
        Remove bzip2 and bsdifflib source code.
        IWad will now be detected by file hash instead of file name.
        
    IWadPatcher 1.2 (20 October 2012)
        
        Add support for BFG edition doom.wad and doom2.wad.

    IWadPatcher 1.1 (9 November 2010)
        
        Hippocrates Sendoukas optimized the patching process to 
        take place in memory and minimize disk access.
        Updated libbzip2 to 1.0.6.
        Updated bsdifflib/bspatchlib to 1.1.

    IWadPatcher 1.0 (26 May 2010)

        Initial release.
