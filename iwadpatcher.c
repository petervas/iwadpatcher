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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#if _WIN32
#include <windows.h>
#include <io.h>
#endif

#include "patches.h"
#include "md5.h"
#include "bspatchlib.h"
#include "iwadpatcher.h"

#ifndef PATH_MAX
#define PATH_MAX 260
#endif

#define NUM_PATCHES	27

static const char* patchMD5[NUM_PATCHES] =  				/* md5 hashes */
{
	"fb35c4a5a9fd49ec29ab6e900572c524",
	"981b03e6d1dc033301aa3095acc437ce", "792fd1fea023d61210857089a7c1e351",
	"54978d12de87f162b9bcc011676cb3c0", "11e1cd216801ea2657723abc86ecb01f",
	"1cd63c5ddff1bf8ce844237f580e9cf3", "c4fe9fd920207691a9f493668e0a2083",
	"c3bea40570c23e511a7ed3ebcd9865f7", "d9153ced9fd5b898b36cc5844e35b520",
	"30e3c2d0350b67bfbf47271970b74b2f", "ea74a47a791fdef2e9f2ea8b8a9da13b",
	"d7a07e5d3f4625074312bc299d7ed33f", "c236745bb01d89bbb866c8fed81b6f8c",
	"25e1459ca71d321525f84628f45ca8cd", "3117e399cdb4298eaa3941625f4b2923",
	"1e4cb4ef075ad344dd63971637307e04", "66d686b1ed6d35ff103f15dbd30e0341",
	"b2543a03521365261d0a0f74d5dd90f0", "abb033caf81e26f12a2103e1fa25453f",
	"1077432e2690d390c256ac908b5f4efa", "78d5898e99e220e4de64edaa0e479593",
	"4e158d9953c79ccf97bd0663244cc6b6", "1d39e405bf6ee3df69a8d2646c8d5c49",
	"8f2d3a6a289f5d2f2f9c1eec02b47299", "2fed2031a5b03892106e0f117f17901f",
	"75c8cf89566741fa9d22447604053bd7", "3493be7e1e2588bc9c8b31eab2587a04"
};

static const char* patchName[NUM_PATCHES] =  				/* patch names */
{
	"DOOM_BFG",
	"DOOM_11", "DOOM_12",
	"DOOM_1666", "DOOM_18",
	"DOOM_19", "DOOM_19UD",
	"DOOM2_BFG", "DOOM2_1666G",
	"DOOM2_1666", "DOOM2_17",
	"DOOM2_17A", "DOOM2_18",
	"DOOM2_19", "HERETIC_10",
	"HERETIC_12", "HERETIC_13",
	"HEXEN_10", "HEXEN_11",
	"HEXDD_10", "HEXDD_11",
	"TNT_19", "TNT_19ANTH",
	"STRIFE1_10", "STRIFE1_131",
	"PLUTONIA_19", "PLUTONIA_19ANTH"
};

static unsigned char* patchData[NUM_PATCHES] =  			/* patch data */
{
	DOOM_BFG,
	DOOM_11, DOOM_12,
	DOOM_1666, DOOM_18,
	DOOM_19, DOOM_19UD,
	DOOM2_BFG, DOOM2_1666G,
	DOOM2_1666, DOOM2_17,
	DOOM2_17A, DOOM2_18,
	DOOM2_19, HERETIC_10,
	HERETIC_12, HERETIC_13,
	HEXEN_10, HEXEN_11,
	HEXDD_10, HEXDD_11,
	TNT_19, TNT_19ANTH,
	STRIFE1_10, STRIFE1_131,
	PLUTONIA_19, PLUTONIA_19ANTH
};

static unsigned int patchLen[NUM_PATCHES] =  				/* patch length */
{
	DOOM_BFG_len,
	DOOM_11_len, DOOM_12_len,
	DOOM_1666_len, DOOM_18_len,
	DOOM_19_len, DOOM_19UD_len,
	DOOM2_BFG_len, DOOM2_1666G_len,
	DOOM2_1666_len, DOOM2_17_len,
	DOOM2_17A_len, DOOM2_18_len,
	DOOM2_19_len, HERETIC_10_len,
	HERETIC_12_len, HERETIC_13_len,
	HEXEN_10_len, HEXEN_11_len,
	HEXDD_10_len, HEXDD_11_len,
	TNT_19_len, TNT_19ANTH_len,
	STRIFE1_10_len, STRIFE1_131_len,
	PLUTONIA_19_len, PLUTONIA_19ANTH_len
};


static const char* patchVersion[NUM_PATCHES] =  			/* version numbers */
{
	"BFG edition",
	"1.1", "1.2",
	"1.666", "1.8",
	"1.9", "1.9 Ultimate Doom",
	"BFG edition", "1.666 German",
	"1.666", "1.7",
	"1.7A", "1.8",
	"1.9", "1.0",
	"1.2", "1.3",
	"1.0", "1.1",
	"1.0", "1.1",
	"1.9", "1.9 Anthology",
	"1.0", "1.31",
	"1.9", "1.9 Anthology"
};

typedef struct
{
	int ctrl;
	int diff;
	int xtra;
} ptch_sz;

static ptch_sz uncompr_patch_sizes[NUM_PATCHES] =
{
	/*  0 */ {  44856, 12424497,   63327 },
	/*  1 */ { 243864, 10154196,  242058 },
	/*  2 */ {  52512, 10311937,   87379 },
	/*  3 */ { 191712, 10984549,  175291 },
	/*  4 */ {     24, 11159840,       0 },
	/*  5 */ {     24, 11159840,       0 },
	/*  6 */ { 334752, 11844269,  564023 },
	/*  7 */ {  73104, 14569157,  122664 },
	/*  8 */ { 316968, 14451304,  373412 },
	/*  9 */ {  31680, 14911693,   31707 },
	/* 10 */ {   6312, 14607013,    5675 },
	/* 11 */ {     24, 14612688,       0 },
	/* 12 */ {     24, 14612688,       0 },
	/* 13 */ {  26856, 14584834,   19750 },
	/* 14 */ {  48600, 11062796,   33692 },
	/* 15 */ {   8232, 11091117,    4399 },
	/* 16 */ { 541464, 12915153, 1274823 },
	/* 17 */ {  45120, 20075266,   53126 },
	/* 18 */ {  55224, 20051982,   31690 },
	/* 19 */ {     72,  4429700,       0 },
	/* 20 */ {   5904,  4432819,    7765 },
	/* 21 */ {     24, 18195736,       0 },
	/* 22 */ {  13680, 18645995,    8801 },
	/* 23 */ { 202776, 27873075,  499093 },
	/* 24 */ { 192816, 27878717,  498647 },
	/* 25 */ {     24, 17420824,       0 },
	/* 26 */ {   1080, 18239811,     361 }
};

enum IWAD
{
	DOOM = 0, DOOM2,
	HERETIC, HEXEN,
	HEXDD, TNT,
	STRIFE1, PLUTONIA
};

static const char* iwadName[] =
{
	"DOOM.WAD",		"DOOM2.WAD",
	"HERETIC.WAD",	"HEXEN.WAD",
	"HEXDD.WAD",	"TNT.WAD",
	"STRIFE1.WAD",	"PLUTONIA.WAD"
};

/***************************************************************************/

static int MD5hash(const char *fname, char *md5string)
{
	FILE		*f;
	int			i, nread;
	md5_state_t	state;
	md5_byte_t	digest[16];
	md5_byte_t	buf[8192];

	if ( (f=fopen(fname,"rb")) == NULL )
		return 0;

	md5_init(&state);

	while ( (nread=(int)fread(buf,1,sizeof(buf),f)) > 0 )
		md5_append(&state, buf, nread);

	fclose(f);
	md5_finish(&state, digest);

	for (i=0; i<16; i++)
		sprintf(md5string + i * 2, "%02x", digest[i]);

	return 1;
}

/***************************************************************************/

/* case insensitive string suffix compare (s1_len >= s2_len)*/
static int suffixstricmp(const char *s1, const char *s2)
{
	size_t i;
	size_t s1_len = strlen(s1);
	size_t s2_len = strlen(s2);
	int res = -1;

	if (s1_len<s2_len)
		return res;

	for(i=0; i<s2_len; ++i)
	{
		if ((res = tolower(s1[s1_len-s2_len+i])-tolower(s2[i])) != 0)
			break;
	}

	return res;
}

/***************************************************************************/

static int check_iwadname (const char *iwadfile)
{
	if (!suffixstricmp(iwadfile, iwadName[DOOM])) return DOOM;
	if (!suffixstricmp(iwadfile, iwadName[DOOM2])) return DOOM2;
	if (!suffixstricmp(iwadfile, iwadName[HERETIC])) return HERETIC;
	if (!suffixstricmp(iwadfile, iwadName[HEXEN])) return HEXEN;
	if (!suffixstricmp(iwadfile, iwadName[HEXDD])) return HEXDD;
	if (!suffixstricmp(iwadfile, iwadName[TNT])) return TNT;
	if (!suffixstricmp(iwadfile, iwadName[STRIFE1])) return STRIFE1;
	if (!suffixstricmp(iwadfile, iwadName[PLUTONIA])) return PLUTONIA;

	return -1;
}

/***************************************************************************/

static int check_iwadMD5 (int game, const char *md5string)
{
	int i;
	int res = -1;

	for (i=0; i<NUM_PATCHES; ++i)
	{
		if (!strcmp(patchMD5[i],md5string))
		{
			res = i;
			break;
		}
	}

	if ( ((game == DOOM) && (res>=0 && res<=6)) || ((game == DOOM2) && (res>=7 && res<=13))
	        || ((game == HERETIC) && (res>=14 && res<=16)) || ((game == HEXEN) && (res>=17 && res<=18))
	        || ((game == HEXDD) && (res>=19 && res<=20)) || ((game == TNT) && (res>=21 && res<=22))
	        || ((game == STRIFE1) && (res>=23 && res<=24)) || ((game == PLUTONIA) && (res>=25 && res<=26)) )
	{
		return res;
	}

	return -1;
}

/***************************************************************************/

static int check_version (int game, const char *version)
{
	int		i, res;

	if (version==NULL)
	{
		if (game == DOOM) return 6;
		if (game == DOOM2) return 13;
		if (game == HERETIC) return 16;
		if (game == HEXEN) return 18;
		if (game == HEXDD) return 20;
		if (game == TNT) return 22;
		if (game == STRIFE1) return 24;
		if (game == PLUTONIA) return 26;
	}

	res = -1;

	for (i=NUM_PATCHES-1; i>=0; i--)
	{
		if (!suffixstricmp(patchName[i],version))
		{
			res = i;
			break;
		}
	}

	if (i<0)
		return -1;

	if ( ((game == DOOM) && (res>=0 && res<=6)) || ((game == DOOM2) && (res>=7 && res<=13))
	        || ((game == HERETIC) && (res>=14 && res<=16)) || ((game == HEXEN) && (res>=17 && res<=18))
	        || ((game == HEXDD) && (res>=19 && res<=20)) || ((game == TNT) && (res>=21 && res<=22))
	        || ((game == STRIFE1) && (res>=23 && res<=24)) || ((game == PLUTONIA) && (res>=25 && res<=26)) )
	{
		return res;
	}

	return -1;
}

/***************************************************************************/

static int next_patch(int p)
{
	if (p<0 || p>=NUM_PATCHES)
		return -1;

	if (p == 6)	return 0;
	if (p == 13) return 7;
	if (p == 16) return 14;
	if (p == 18) return 17;
	if (p == 20) return 19;
	if (p == 22) return 21;
	if (p == 24) return 23;
	if (p == 26) return 25;

	return ++p;
}

/***************************************************************************/

static int patch_aux(const char *infile, const char *outfile,
                     int backup, const char *version, char **msg)
{
	unsigned char	*in_buf, *out_buf;
	int				in_sz, out_sz, game, init_ver, cur_ver, fin_ver;
	char			*errs, iwad_date[32], iwad_hash[16*2 + 1], bakname[PATH_MAX+1];
	static char		msgstr[512+PATH_MAX];

	if (!infile || *infile=='\0')
	{
		sprintf(msgstr,"No IWAD file specified.");
		*msg = msgstr;
		return 0;
	}

	if ( (game=check_iwadname(infile)) == -1 )
	{
		sprintf(msgstr,"\"%s\" is not a registered IWAD name.", infile);
		*msg = msgstr;
		return 0;
	}

	if ( !MD5hash(infile, iwad_hash) )
	{
		sprintf(msgstr,"Cannot compute MD5 Hash from %s .", iwadName[game]);
		*msg = msgstr;
		return 0;
	}

	if ( (init_ver=cur_ver=check_iwadMD5(game,iwad_hash)) == -1 )
	{
		sprintf(msgstr,"Cannot patch unknown %s version.", iwadName[game]);
		*msg = msgstr;
		return 0;
	}

	/* piracy check */
	if (game==DOOM2)
	{
		struct stat s;
		stat(infile, &s);
		strftime(iwad_date, 20, "%d-%m-%Y", localtime( &s.st_mtime));

		if (!strcmp(iwad_date,"25-08-1994"))
		{
			sprintf(msgstr,"Cannot patch pirated DOOM2.WAD. Buy the game!");
			*msg = msgstr;
			return 0;
		}
	}

	if ( (fin_ver=check_version(game,version)) == -1 )
	{
		sprintf(msgstr,"Incompatible or unknown patch requested for %s .", iwadName[game]);
		*msg = msgstr;
		return 0;
	}

	if (init_ver == fin_ver)
	{
		sprintf(msgstr, "%s is already at version %s" ,iwadName[game], patchVersion[fin_ver]);
		*msg = msgstr;
		return 0;
	}

	errs = file_to_mem(infile, &in_buf, &in_sz);

	if (errs)
	{
		*msg = errs;
		return 0;
	}

	/* Patching loop */
	do
	{
		cur_ver = next_patch(cur_ver);
		errs = bspatch_mem(in_buf, in_sz, &out_buf, &out_sz, patchData[cur_ver], patchLen[cur_ver],
		                   uncompr_patch_sizes[cur_ver].ctrl, uncompr_patch_sizes[cur_ver].diff,
		                   uncompr_patch_sizes[cur_ver].xtra);
		free(in_buf);

		if (errs)
		{
			*msg = errs;
			return 0;
		}

		/* Replace input by output data so we can repeat this loop */
		in_buf = out_buf;
		in_sz = out_sz;
		out_buf = NULL;
		out_sz = 0;
	}
	while ( cur_ver != fin_ver );

	if (outfile && *outfile)
	{
		mem_to_file(in_buf, in_sz, outfile);
	}
	else
	{
		if (backup)
		{
			sprintf(bakname,"%s.bak",infile);
			rename(infile,bakname);
		}

		mem_to_file(in_buf, in_sz, infile);
	}

	free(in_buf);
	sprintf(msgstr, "%s %s patched to %s.", iwadName[game], patchVersion[init_ver], patchVersion[fin_ver]);
	*msg = msgstr;
	return 1;
}

/***************************************************************************/

int WINAPI patch_iwad(const char *iwadfile, int backup, char **msg)
{
	return patch_aux(iwadfile, NULL, backup, NULL, msg);
}

/***************************************************************************/

int WINAPI patch_iwad_to(const char *iwadfile,int backup,const char *version, char **msg)
{
	return patch_aux(iwadfile, NULL, backup, version, msg);
}

/***************************************************************************/

int WINAPI patch_iwad2(const char *infile, const char *outfile, char **msg)
{
	return patch_aux(infile, outfile, 1, NULL, msg);
}

/***************************************************************************/

int WINAPI patch_iwad_to2(const char *infile, const char *outfile, const char *version, char **msg)
{
	return patch_aux(infile, outfile, 1, version, msg);
}

/***************************************************************************/

#if DLL

BOOL WINAPI _CRT_INIT(HINSTANCE,DWORD,LPVOID);

BOOL WINAPI DllEntryPoint(HINSTANCE hinst,DWORD reason,LPVOID p)
{
	if (reason==DLL_PROCESS_ATTACH || reason==DLL_THREAD_ATTACH)
	{
		if(!_CRT_INIT(hinst,reason,p))
		{
			return FALSE;
		}
	}

	if (reason==DLL_PROCESS_DETACH || reason==DLL_THREAD_DETACH)
	{
		if (!_CRT_INIT(hinst,reason,p))
		{
			return FALSE;
		}
	}

	return TRUE;
}

#endif
