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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#if defined(_WIN32)
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

#define MSGSTR_MAX_LEN 512 + PATH_MAX

#if defined(_MSC_VER) && _MSC_VER<1900		//VS before 2015
	#define snprintf	_snprintf
#endif

enum DoomPatch
{
	DOOM_11,
	DOOM_12,
	DOOM_1666,
	DOOM_18,
	DOOM_19,
	DOOM_19UD,
	DOOM_BFG,
	DOOM_ETERNAL,
	DOOM_UNITY,
	DOOM_KEX,
	NUM_DOOM_PATCHES
};

enum Doom2Patch
{
	DOOM2_1666G,
	DOOM2_1666,
	DOOM2_17,
	DOOM2_17A,
	DOOM2_18,
	DOOM2_19,
	DOOM2_BFG,
	DOOM2_ETERNAL,
	DOOM2_UNITY,
	DOOM2_KEX,
	NUM_DOOM2_PATCHES
};

enum PlutoniaPatch
{
	PLUTONIA_19,
	PLUTONIA_19ANTH,
	PLUTONIA_UNITY,
	PLUTONIA_KEX,
	NUM_PLUTONIA_PATCHES
};

enum TntPatch
{
	TNT_19,
	TNT_19ANTH,
	TNT_UNITY,
	TNT_KEX,
	NUM_TNT_PATCHES
};

enum HereticPatch
{
	HERETIC_10,
	HERETIC_12,
	HERETIC_13,
	NUM_HERETIC_PATCHES
};

enum HexenPatch
{
	HEXEN_10,
	HEXEN_11,
	NUM_HEXEN_PATCHES
};

enum HexddPatch
{
	HEXDD_10,
	HEXDD_11,
	NUM_HEXDD_PATCHES
};

enum Strife1Patch
{
	STRIFE1_10,
	STRIFE1_131,
	NUM_STRIFE1_PATCHES
};

/*
 * To obtain ctrl, diff and xtra sizes just pass the patch file only to bspatch
 * from the bsdifflib tools.
 */
typedef struct
{
	int ctrl;
	int diff;
	int xtra;
} ptch_sz;

typedef struct
{
	const char *patchName;
	const char *versionStr;
	const char *patchMD5;
	unsigned char *patchData;
	int patchLen;
	ptch_sz uncompr_patch_sizes;
} iwad_patch_info;

/*
 * A patch e.g. DOOM_1666 will patch from (the previous) version 1.2 to 1.666.
 * The first patch e.g. DOOM_11 will patch from version Unity to 1.1.
 */
static iwad_patch_info patchDoom[NUM_DOOM_PATCHES] =
	{
		{"DOOM_11", "1.1", "981b03e6d1dc033301aa3095acc437ce",
		 DOOM_11_data, DOOM_11_len, {271968, 10154097, 242157}},
		{"DOOM_12", "1.2", "792fd1fea023d61210857089a7c1e351",
		 DOOM_12_data, DOOM_12_len, {52512, 10311937, 87379}},
		{"DOOM_1666", "1.666", "54978d12de87f162b9bcc011676cb3c0",
		 DOOM_1666_data, DOOM_1666_len, {191712, 10984549, 175291}},
		{"DOOM_18", "1.8", "11e1cd216801ea2657723abc86ecb01f",
		 DOOM_18_data, DOOM_18_len, {24, 11159840, 0}},
		{"DOOM_19", "1.9", "1cd63c5ddff1bf8ce844237f580e9cf3",
		 DOOM_19_data, DOOM_19_len, {24, 11159840, 0}},
		{"DOOM_19UD", "1.9 Ultimate Doom", "c4fe9fd920207691a9f493668e0a2083",
		 DOOM_19UD_data, DOOM_19UD_len, {334752, 11844269, 564023}},
		{"DOOM_BFG", "BFG edition", "fb35c4a5a9fd49ec29ab6e900572c524",
		 DOOM_BFG_data, DOOM_BFG_len, {44856, 12424497, 63327}},
		{"DOOM_ETERNAL", "Eternal (PSN)", "e4f120eab6fb410a5b6e11c947832357",
		 DOOM_ETERNAL_data, DOOM_ETERNAL_len, {936, 12474410, 151}},
		{"DOOM_UNITY", "Unity 1.3", "8517c4e8f0eef90b82852667d345eb86",
		 DOOM_UNITY_data, DOOM_UNITY_len, {194616, 12548140, 154781}},
		{"DOOM_KEX", "KEX (2024)", "4461d4511386518e784c647e3128e7bc",
		 DOOM_KEX_data, DOOM_KEX_len, {37320, 12715838, 17654}}
	};

static iwad_patch_info patchDoom2[NUM_DOOM2_PATCHES] =
	{
		{"DOOM2_1666G", "1.666 German", "d9153ced9fd5b898b36cc5844e35b520",
		 DOOM2_1666G_data, DOOM2_1666G_len, {225264, 14612783, 211933}},
		{"DOOM2_1666", "1.666", "30e3c2d0350b67bfbf47271970b74b2f",
		 DOOM2_1666_data, DOOM2_1666_len, {31680, 14911693, 31707}},
		{"DOOM2_17", "1.7", "ea74a47a791fdef2e9f2ea8b8a9da13b",
		 DOOM2_17_data, DOOM2_17_len, {6312, 14607013, 5675}},
		{"DOOM2_17A", "1.7A", "d7a07e5d3f4625074312bc299d7ed33f",
		 DOOM2_17A_data, DOOM2_17A_len, {24, 14612688, 0}},
		{"DOOM2_18", "1.8", "c236745bb01d89bbb866c8fed81b6f8c",
		 DOOM2_18_data, DOOM2_18_len, {24, 14612688, 0}},
		{"DOOM2_19", "1.9", "25e1459ca71d321525f84628f45ca8cd",
		 DOOM2_19_data, DOOM2_19_len, {26856, 14584834, 19750}},
		{"DOOM2_BFG", "BFG edition", "c3bea40570c23e511a7ed3ebcd9865f7",
		 DOOM2_BFG_data, DOOM2_BFG_len, {73104, 14569157, 122664}},
		{"DOOM2_ETERNAL", "Eternal (Bethesda.net)", "97573aaf26957099ed45e61d81a0a1a3",
		 DOOM2_ETERNAL_data, DOOM2_ETERNAL_len, {37656, 14548287, 54925}},
		{"DOOM2_UNITY", "Unity 1.3", "8ab6d0527a29efdc1ef200e5687b5cae",
		 DOOM2_UNITY_data, DOOM2_UNITY_len, {242856, 14558806, 244037}},
		{"DOOM2_KEX", "KEX (2024)", "9aa3cbf65b961d0bdac98ec403b832e1",
		 DOOM2_KEX_data, DOOM2_KEX_len, {13152, 14767338, 35168}}
	};

static iwad_patch_info patchPlutonia[NUM_PLUTONIA_PATCHES] =
	{
		{"PLUTONIA_19", "1.9", "75c8cf89566741fa9d22447604053bd7",
		 PLUTONIA_19_data, PLUTONIA_19_len, {83112, 17409678, 11146}},
		{"PLUTONIA_19ANTH", "1.9 Anthology", "3493be7e1e2588bc9c8b31eab2587a04",
		 PLUTONIA_19ANTH_data, PLUTONIA_19ANTH_len, {1080, 18239811, 361}},
		{"PLUTONIA_UNITY", "Unity 1.3", "ae76c20366ff685d3bb9fab11b148b84",
		 PLUTONIA_UNITY_data, PLUTONIA_UNITY_len, {140280, 17447083, 70103}},
		{"PLUTONIA_KEX", "KEX (2024)", "24037397056e919961005e08611623f4",
		 PLUTONIA_KEX_data, PLUTONIA_KEX_len, {4272, 17517391, 14102}}
	};

static iwad_patch_info patchTnt[NUM_TNT_PATCHES] =
	{
		{"TNT_19", "1.9", "4e158d9953c79ccf97bd0663244cc6b6",
		 TNT_19_data, TNT_19_len, {90000, 18176539, 19197}},
		{"TNT_19ANTH", "1.9 Anthology", "1d39e405bf6ee3df69a8d2646c8d5c49",
		 TNT_19ANTH_data, TNT_19ANTH_len, {13680, 18645995, 8801}},
		{"TNT_UNITY", "Unity 1.3", "f5528f6fd55cf9629141d79eda169630",
		 TNT_UNITY_data, TNT_UNITY_len, {137352, 17731162, 798722}},
		{"TNT_KEX", "KEX (2024)", "8974e3117ed4a1839c752d5e11ab1b7b",
		 TNT_KEX_data, TNT_KEX_len, {4104, 17784117, 520513}}
	};

static iwad_patch_info patchHeretic[NUM_HERETIC_PATCHES] =
	{
		{"HERETIC_10", "1.0", "3117e399cdb4298eaa3941625f4b2923",
		 HERETIC_10_data, HERETIC_10_len, {48600, 11062796, 33692}},
		{"HERETIC_12", "1.2", "1e4cb4ef075ad344dd63971637307e04",
		 HERETIC_12_data, HERETIC_12_len, {8232, 11091117, 4399}},
		{"HERETIC_13", "1.3", "66d686b1ed6d35ff103f15dbd30e0341",
		 HERETIC_13_data, HERETIC_13_len, {541464, 12915153, 1274823}}
	};

static iwad_patch_info patchHexen[NUM_HEXEN_PATCHES] =
	{
		{"HEXEN_10", "1.0", "b2543a03521365261d0a0f74d5dd90f0",
		 HEXEN_10_data, HEXEN_10_len, {45120, 20075266, 53126}},
		{"HEXEN_11", "1.1", "abb033caf81e26f12a2103e1fa25453f",
		 HEXEN_11_data, HEXEN_11_len, {55224, 20051982, 31690}}
	};

static iwad_patch_info patchHexdd[NUM_HEXDD_PATCHES] =
	{
		{"HEXDD_10", "1.0", "1077432e2690d390c256ac908b5f4efa",
		 HEXDD_10_data, HEXDD_10_len, {72, 4429700, 0}},
		{"HEXDD_11", "1.1", "78d5898e99e220e4de64edaa0e479593",
		 HEXDD_11_data, HEXDD_11_len, {5904, 4432819, 7765}}
	};

static iwad_patch_info patchStrife1[NUM_STRIFE1_PATCHES] =
	{
		{"STRIFE1_10", "1.0", "8f2d3a6a289f5d2f2f9c1eec02b47299",
		 STRIFE1_10_data, STRIFE1_10_len, {202776, 27873075, 499093}},
		{"STRIFE1_131", "1.2/1.31", "2fed2031a5b03892106e0f117f17901f",
		 STRIFE1_131_data, STRIFE1_131_len, {192816, 27878717, 498647}}
	};

enum IWAD
{
	DOOM,
	DOOM2,
	PLUTONIA,
	TNT,
	HERETIC,
	HEXEN,
	HEXDD,
	STRIFE1,
	NUM_IWAD
};

typedef struct
{
	const char *iwadName;
	int numPatches;
	iwad_patch_info *patchInfo;
} iwad_info;

static iwad_info iwad[] =
	{
		{"DOOM.WAD", NUM_DOOM_PATCHES, patchDoom},
		{"DOOM2.WAD", NUM_DOOM2_PATCHES, patchDoom2},
		{"PLUTONIA.WAD", NUM_PLUTONIA_PATCHES, patchPlutonia},
		{"TNT.WAD", NUM_TNT_PATCHES, patchTnt},
		{"HERETIC.WAD", NUM_HERETIC_PATCHES, patchHeretic},
		{"HEXEN.WAD", NUM_HEXEN_PATCHES, patchHexen},
		{"HEXDD.WAD", NUM_HEXDD_PATCHES, patchHexdd},
		{"STRIFE1.WAD", NUM_STRIFE1_PATCHES, patchStrife1}
	};

/***************************************************************************/

static int MD5hash(const char *fname, char *md5string)
{
	FILE *f;
	int i, nread;
	md5_state_t state;
	md5_byte_t digest[16];
	md5_byte_t buf[8192];

	if ((f = fopen(fname, "rb")) == NULL)
		return 0;

	md5_init(&state);

	while ((nread = (int)fread(buf, (size_t)1, sizeof(buf), f)) > 0)
		md5_append(&state, buf, nread);

	fclose(f);
	md5_finish(&state, digest);

	for (i = 0; i < 16; i++)
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

	if (s1_len < s2_len)
		return res;

	for (i = 0; i < s2_len; ++i)
	{
		if ((res = tolower(s1[s1_len - s2_len + i]) - tolower(s2[i])) != 0)
			break;
	}

	return res;
}

/***************************************************************************/

static int check_iwadMD5(int *game, const char *md5string)
{
	int i, j;
	int res = -1;

	for (i = 0; i < NUM_IWAD; ++i)
		for (j = 0; j < iwad[i].numPatches; ++j)
		{
			if (!strcmp(iwad[i].patchInfo[j].patchMD5, md5string))
			{
				res = j;
				*game = i;
				break;
			}
		}

	return res;
}

/***************************************************************************/

static int check_version(int game, const char *version)
{
	int i, res;

	if (version == NULL)
	{
		if (game == DOOM)
			return DEFAULT_PATCH_DOOM;
		if (game == DOOM2)
			return DEFAULT_PATCH_DOOM2;
		if (game == PLUTONIA)
			return DEFAULT_PATCH_PLUTONIA;
		if (game == TNT)
			return DEFAULT_PATCH_TNT;
		if (game == HERETIC)
			return DEFAULT_PATCH_HERETIC;
		if (game == HEXEN)
			return DEFAULT_PATCH_HEXEN;
		if (game == HEXDD)
			return DEFAULT_PATCH_HEXDD;
		if (game == STRIFE1)
			return DEFAULT_PATCH_STRIFE1;
	}

	res = -1;

	for (i = 0; i < iwad[game].numPatches; ++i)
	{
		if (!suffixstricmp(iwad[game].patchInfo[i].patchName, version))
		{
			res = i;
			break;
		}
	}

	return res;
}

/***************************************************************************/

static int patch_aux(const char *infile, const char *outfile,
					 int backup, const char *version, char **msg)
{
	unsigned char *in_buf, *out_buf;
	int in_sz, out_sz, game, init_ver, cur_ver, fin_ver, default_name;
	char *errs, iwad_hash[16 * 2 + 1], bakname[PATH_MAX + 1];
	static char msgstr[MSGSTR_MAX_LEN];

	if (!infile || *infile == '\0')
	{
		snprintf(msgstr, sizeof(msgstr), "No IWAD file specified.");
		*msg = msgstr;
		return 0;
	}

	if (!MD5hash(infile, iwad_hash))
	{
		snprintf(msgstr, sizeof(msgstr), "Cannot compute MD5 Hash for %s.", infile);
		*msg = msgstr;
		return 0;
	}

	game = 0;
	if ((init_ver = cur_ver = check_iwadMD5(&game, iwad_hash)) == -1)
	{
		snprintf(msgstr, sizeof(msgstr), "Cannot patch unknown %s version.", infile);
		*msg = msgstr;
		return 0;
	}

	default_name = !suffixstricmp(iwad[game].iwadName, infile);

	if ((fin_ver = check_version(game, version)) == -1)
	{
		if (default_name)
			snprintf(msgstr, sizeof(msgstr), "Incompatible or unknown patch requested for %s.", iwad[game].iwadName);
		else
			snprintf(msgstr, sizeof(msgstr), "Incompatible or unknown patch requested for %s (%s).", iwad[game].iwadName, infile);
		*msg = msgstr;
		return 0;
	}

	if (init_ver == fin_ver)
	{
		if (default_name)
			snprintf(msgstr, sizeof(msgstr), "%s is already at version %s.", iwad[game].iwadName, iwad[game].patchInfo[fin_ver].versionStr);
		else
			snprintf(msgstr, sizeof(msgstr), "%s (%s) is already at version %s.", iwad[game].iwadName, infile, iwad[game].patchInfo[fin_ver].versionStr);
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
		cur_ver = (cur_ver + 1) % iwad[game].numPatches;
		errs = bspatch_mem(in_buf, in_sz, &out_buf, &out_sz, iwad[game].patchInfo[cur_ver].patchData, iwad[game].patchInfo[cur_ver].patchLen,
						   iwad[game].patchInfo[cur_ver].uncompr_patch_sizes.ctrl, iwad[game].patchInfo[cur_ver].uncompr_patch_sizes.diff,
						   iwad[game].patchInfo[cur_ver].uncompr_patch_sizes.xtra);
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
	} while (cur_ver != fin_ver);

	if (outfile && *outfile)
	{
		mem_to_file(in_buf, in_sz, outfile);
	}
	else
	{
		if (backup)
		{
			snprintf(bakname, sizeof(bakname), "%s.bak", infile);
			if (rename(infile, bakname) == -1)
			{
				snprintf(msgstr, sizeof(msgstr), "Could not rename %s to %s.", infile, bakname);
				*msg = msgstr;
				return 0;
			}
		}

		mem_to_file(in_buf, in_sz, infile);
	}

	free(in_buf);
	if (default_name)
		snprintf(msgstr, sizeof(msgstr), "%s %s patched to %s.", iwad[game].iwadName, iwad[game].patchInfo[init_ver].versionStr,
				 iwad[game].patchInfo[fin_ver].versionStr);
	else
		snprintf(msgstr, sizeof(msgstr), "%s (%s) %s patched to %s.", iwad[game].iwadName, infile, iwad[game].patchInfo[init_ver].versionStr,
				 iwad[game].patchInfo[fin_ver].versionStr);
	*msg = msgstr;
	return 1;
}

/***************************************************************************/

int WINAPI patch_iwad(const char *iwadfile, int backup, char **msg)
{
	return patch_aux(iwadfile, NULL, backup, NULL, msg);
}

/***************************************************************************/

int WINAPI patch_iwad_to(const char *iwadfile, int backup, const char *version, char **msg)
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

#if defined(DLL)

BOOL WINAPI _CRT_INIT(HINSTANCE, DWORD, LPVOID);

BOOL WINAPI DllEntryPoint(HINSTANCE hinst, DWORD reason, LPVOID p)
{
	if (reason == DLL_PROCESS_ATTACH || reason == DLL_THREAD_ATTACH)
	{
		if (!_CRT_INIT(hinst, reason, p))
		{
			return FALSE;
		}
	}

	if (reason == DLL_PROCESS_DETACH || reason == DLL_THREAD_DETACH)
	{
		if (!_CRT_INIT(hinst, reason, p))
		{
			return FALSE;
		}
	}

	return TRUE;
}

#endif
