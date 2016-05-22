IWadPatcher 1.2
released on October 20th, 2012

IWadPatcher patches any registered IWAD from DOOM, DOOM II, Final DOOM,
Heretic, Hexen and Strife to the latest or any other official version.
Shareware IWADs cannot be patched as they can be downloaded for free.
IWadPatcher is written in ANSI C compliant cross-platform C and can also
be used as a C/C++ library.
IWadPatcher is released under the BSD license.
Copyright 2012 Peter Vaskovic <petervaskovic@yahoo.de>

uses binary diff/patch library (bsdifflib/bspatchlib) 1.1, written by
Copyright 2010 Peter Vaskovic <petervaskovic@yahoo.de>
available at https://github.com/petervas/bsdifflib

uses independent implementation of MD5 1.6, written by
Copyright 1999-2002 L. Peter Deutsch <ghost@aladdin.com>
available at http://userpages.umbc.edu/~mabzug1/cs/md5/md5.html

source release includes files from libbzip2 1.0.6 (20 September 2010)
Copyright 1996-2010 Julian R Seward <jseward@bzip.org>
available at http://www.bzip.org/

source code was formatted with Artistic Style 1.24
available at http://astyle.sourceforge.net/
using the options "-A1txjz1f".

I would like to thank Hippocrates Sendoukas for helping me out with some
C related questions and giving me helpful coding suggestions in general.
You can visit his site at http://hs.keystone.gr/

Changelog:

    IWadPatcher 1.0 (26 May 2010)

        Initial release.

    IWadPatcher 1.1 (9 November 2010)
        
        Hippocrates Sendoukas optimized the patching process to 
        take place in memory and minimize disk access.
        Updated libbzip2 to 1.0.6.
		Updated bsdifflib/bspatchlib to 1.1.
	
	IWadPatcher 1.2 (20 October 2012)
		
		Add support for BFG edition doom.wad and doom2.wad.

-------------------------------------------------------------------------------

Usage: iwadpatcher [-Ooutputfile] [-Vdesired_version] iwadfile

Modifies (patches) IWADs enabling you to change their version in
ANY DIRECTION that you like (either forward or backward).

Supported IWADs:
	doom.wad, doom2.wad, heretic.wad, hexen.wad,
	hexdd.wad, tnt.wad, plutonia.wad, strife1.wad.

Valid version strings:
	For Doom:     "DOOM_BFG",    "DOOM_11",     "DOOM_12",
                  "DOOM_1666",   "DOOM_18",     "DOOM_19",
                  "DOOM_19UD"
	For Doom2:    "DOOM2_BFG",   "DOOM2_1666G", "DOOM2_1666",
	              "DOOM2_17",    "DOOM2_17A",   "DOOM2_18",
                  "DOOM2_19"
	For Heretic:  "HERETIC_10",  "HERETIC_12",  "HERETIC_13"
	For Hexen:    "HEXEN_10",    "HEXEN_11"
	For HexDD:    "HEXDD_10",    "HEXDD_11"
	For TNT:      "TNT_19",      "TNT_19ANTH"
	For Plutonia: "PLUTONIA_19", "PLUTONIA_19ANTH"
	For Strife:   "STRIFE1_10",  "STRIFE1_131"

If no version string is specified, it defaults to the latest one
for the given IWAD. Notice that the BFG editions are NOT considered
to be the latest IWAD versions!
	
For more info about the different IWAD versions go to: 
http://doomwiki.org/wiki/IWAD

-------------------------------------------------------------------------------
