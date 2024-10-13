/*-
 * Copyright 2024, Peter Vaskovic, (petervaskovic@yahoo.de)
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "iwadpatcher.h"

#define xstr(a) str(a)
#define str(a) #a

int main(int argc,char * argv[])
{
	int		ok, i;
	char	*msg, *output_file, *version, c;
	output_file = version = NULL;

	for (i=0;  i<argc-1;  )
	{
		if (*argv[i+1]!='-')
		{
			break;    /* Doesn't start with - */
		}

		if ( (c = argv[i+1][1]) == '\0' )
		{
			break;    /* Single - indicating the end of options */
		}

		i++;

		if (c=='o' || c=='O')
		{
			output_file = argv[i]+2;
		}
		else if (c=='v' || c=='V')
		{
			version = argv[i]+2;
		}
		else
		{
			goto USAGE;
		}
	}

	if (argc != i+2)
	{
		goto USAGE;
	}

	if (output_file && *output_file)
	{
		ok = (version && *version) ?
			 patch_iwad_to2(argv[i+1], output_file, version, &msg) :
			 patch_iwad2(argv[i+1], output_file, &msg);
	}
	else
	{
		ok = (version && *version) ?
			 patch_iwad_to(argv[i+1], 1, version, &msg) :
			 patch_iwad(argv[i+1], 1, &msg);
	}

	printf("%s\n",msg);
	return !ok;
USAGE:
	printf( "\n"
			"Usage: iwadpatcher [-Ooutputfile] [-Vdesired_version] iwadfile\n"
			"Patch IWADs to any official PC version.\n"
			"\n"
			"Example: iwadpatcher -ODOOM2_PATCHED.WAD -VDOOM2_19 DOOM2.WAD\n"
			"     or: iwadpatcher -VHERETIC_12 HERETIC.WAD\n"
			"     or: iwadpatcher DOOM.WAD\n"
			"\n"
			"Supported IWADs:\n"
			"\tDOOM.WAD, DOOM2.WAD, PLUTONIA.WAD, TNT.WAD,\n"
			"\tHERETIC.WAD, HEXEN.WAD, HEXDD.WAD, STRIFE1.WAD\n"
			"\n"
			"Valid version strings:\n"
			"\tDOOM:     DOOM_11,     DOOM_12,     DOOM_1666,\n"
			"\t          DOOM_18,     DOOM_19,     DOOM_19UD,\n"
			"\t          DOOM_BFG,    DOOM_ETERNAL,\n"
			"\t          DOOM_UNITY   DOOM_KEX\n"
			"\tDOOM2:    DOOM2_1666G, DOOM2_1666,  DOOM2_17,\n"
			"\t          DOOM2_17A,   DOOM2_18,    DOOM2_19,\n"
			"\t          DOOM2_BFG,   DOOM2_ETERNAL,\n"
			"\t          DOOM2_UNITY  DOOM2_KEX\n"
			"\tPLUTONIA: PLUTONIA_19, PLUTONIA_19ANTH,\n"
			"\t          PLUTONIA_UNITY\n"
			"\t          PLUTONIA_KEX\n"
			"\tTNT:      TNT_19,      TNT_19ANTH,  TNT_UNITY\n"
			"\t          TNT_KEX\n"
			"\tHERETIC:  HERETIC_10,  HERETIC_12,  HERETIC_13\n"
			"\tHEXEN:    HEXEN_10,    HEXEN_11\n"
			"\tHEXDD:    HEXDD_10,    HEXDD_11\n"
			"\tSTRIFE1:  STRIFE1_10,  STRIFE1_131\n"
			"\n"
			"If no version is specified, the default will be:\n"
			"\tDOOM:     " xstr(DEFAULT_PATCH_DOOM) "\n"
			"\tDOOM2:    " xstr(DEFAULT_PATCH_DOOM2) "\n"
			"\tPLUTONIA: " xstr(DEFAULT_PATCH_PLUTONIA) "\n"
			"\tTNT:      " xstr(DEFAULT_PATCH_TNT) "\n"
			"\tHERETIC:  " xstr(DEFAULT_PATCH_HERETIC) "\n"
			"\tHEXEN:    " xstr(DEFAULT_PATCH_HEXEN) "\n"
			"\tHEXDD:    " xstr(DEFAULT_PATCH_HEXDD) "\n"
			"\tSTRIFE1:  " xstr(DEFAULT_PATCH_STRIFE1) "\n"
			"\n"
			"Version: 1.4 (2024-10-13)\n"
			);
	return 1;
}
