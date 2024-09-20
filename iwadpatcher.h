#ifndef IWADPATCHERH__
#define IWADPATCHERH__
/*-
 * Copyright 2023, Peter Vaskovic, (petervaskovic@yahoo.de)
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
#if defined(_WIN32)
#include <windows.h>
#else
#define WINAPI
#endif

/* Define what version an IWAD should be patched to if no version has been specified by the user */
#ifndef DEFAULT_PATCH_DOOM
#define DEFAULT_PATCH_DOOM	DOOM_19UD
#endif
#ifndef DEFAULT_PATCH_DOOM2
#define DEFAULT_PATCH_DOOM2	DOOM2_19
#endif
#ifndef DEFAULT_PATCH_PLUTONIA
#define DEFAULT_PATCH_PLUTONIA PLUTONIA_19ANTH
#endif
#ifndef DEFAULT_PATCH_TNT
#define DEFAULT_PATCH_TNT TNT_19ANTH
#endif
#ifndef DEFAULT_PATCH_HERETIC
#define DEFAULT_PATCH_HERETIC HERETIC_13
#endif
#ifndef DEFAULT_PATCH_HEXEN
#define DEFAULT_PATCH_HEXEN HEXEN_11
#endif
#ifndef DEFAULT_PATCH_HEXDD
#define DEFAULT_PATCH_HEXDD HEXDD_11
#endif
#ifndef DEFAULT_PATCH_STRIFE1
#define DEFAULT_PATCH_STRIFE1 STRIFE1_131
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * All functions return 1 on success and 0 otherwise.
	 * An error description is returned through the "msg" parameter.
	 */
	int WINAPI patch_iwad(const char *iwadfile,int backup,char **msg);
	int WINAPI patch_iwad_to(const char *iwadfile,int backup,const char *version, char **msg);
	int WINAPI patch_iwad2(const char *infile, const char *outfile, char **msg);
	int WINAPI patch_iwad_to2(const char *infile, const char *outfile, const char *version, char **msg);

#ifdef __cplusplus
}
#endif

#endif /* IWADPATCHERH__ */
