/*
* Copyright © 2006 Keith Packard
* Copyright © 2010 Behdad Esfahbod
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of the author(s) not be used in
* advertising or publicity pertaining to distribution of the software without
* specific, written prior permission.  The authors make no
* representations about the suitability of this software for any purpose.  It
* is provided "as is" without express or implied warranty.
*
* THE AUTHOR(S) DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
* INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
* EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
* DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
* TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*/
#ifndef _DIRENT_H_
#define _DIRENT_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <windows.h>

/* dirent.h is missing on Windows and fontconfig uses it */
/* http://pubs.opengroup.org/onlinepubs/009695399/basedefs/dirent.h.html */

/*
A type representing a directory stream.
It shall also define the structure dirent which shall include the following members :

[XSI][Option Start]
ino_t  d_ino       File serial number.
[Option End]
char   d_name[]    Name of entry.

[XSI][Option Start] The type ino_t shall be defined as described in <sys / types.h>.[Option End]

The character array d_name is of unspecified size, but the number of bytes preceding the terminating null byte shall not exceed{ NAME_MAX }.

The following shall be declared as functions and may also be defined as macros.Function prototypes shall be provided.
*/

struct dirent {
	char d_name[256];
};

struct __dirstream {
	WIN32_FIND_DATA ffd;
	HANDLE hFind;
	struct dirent current;
	int read;
};

typedef struct __dirstream DIR;

int            closedir(DIR *);
DIR           *opendir(const char *);
struct dirent *readdir(DIR *);

/*[TSF][Option Start]
int            readdir_r(DIR *restrict, struct dirent *restrict,
	struct dirent **restrict);
[Option End]
*/

void           rewinddir(DIR *);

/*
[XSI][Option Start]
void           seekdir(DIR *, long);
long           telldir(DIR *);*/

#endif /* _DIRENT_H_ */
#pragma once
