/*-
 * Copyright 2012, Peter Vaskovic, (petervaskovic@yahoo.de)
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
	        "\n"
	        "Modifies (patches) IWADs enabling you to change their version in\n"
	        "ANY DIRECTION that you like (either forward or backward).\n"
	        "\n"
	        "Supported IWADs:\n"
	        "\tdoom.wad, doom2.wad, heretic.wad, hexen.wad,\n"
	        "\thexdd.wad, tnt.wad, plutonia.wad, strife1.wad.\n"
	        "\n"
	        "Valid version strings:\n"
	        "\tFor Doom:     \"DOOM_BFG\",    \"DOOM_11\",     \"DOOM_12\",\n"
	        "\t              \"DOOM_1666\",   \"DOOM_18\",     \"DOOM_19\",\n"
            "\t              \"DOOM_19UD\"\n"
	        "\tFor Doom2:    \"DOOM2_BFG\",   \"DOOM2_1666G\", \"DOOM2_1666\",\n"
	        "\t              \"DOOM2_17\",    \"DOOM2_17A\",   \"DOOM2_18\",\n"
            "\t              \"DOOM2_19\"\n"			
	        "\tFor Heretic:  \"HERETIC_10\",  \"HERETIC_12\",  \"HERETIC_13\"\n"
	        "\tFor Hexen:    \"HEXEN_10\",    \"HEXEN_11\"\n"
	        "\tFor HexDD:    \"HEXDD_10\",    \"HEXDD_11\"\n"
	        "\tFor TNT:      \"TNT_19\",      \"TNT_19ANTH\"\n"
	        "\tFor Plutonia: \"PLUTONIA_19\", \"PLUTONIA_19ANTH\"\n"
	        "\tFor Strife:   \"STRIFE1_10\",  \"STRIFE1_131\"\n"
	        "\n"
	        "If no version string is specified, it defaults to the latest one\n"
	        "for the given IWAD. Notice that the BFG editions are NOT considered\n"
			"to be the latest IWAD versions!\n"
	        "\n"
	        "Version: 1.2 (2012-10-20)\n"
	      );
	return 1;
}
