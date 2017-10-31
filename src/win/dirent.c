/*
* fontconfig/src/fclist.c
*
* Copyright © 2000 Keith Packard
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

#include "dirent.h"
#include <stdlib.h>
#include <errno.h>

int closedir(DIR * dir)
{
	FindClose(dir->hFind);
}

static copy_ffd_to_dirent(WIN32_FIND_DATA * ffd, struct dirent * dir)
{
	int i;
	for (i = 0; i < MAX_PATH && ffd->cFileName[i]; i++)
		dir->d_name[i] = ffd->cFileName[i];
	dir->d_name[i] = '\0';
}

DIR * opendir(const char * path)
{
	WIN32_FIND_DATA ffd;
	wchar_t wpath[MAX_PATH + 5];
	HANDLE hFind;
	DIR * result;
	int i;

	for (i = 0; i < sizeof(wpath) && path[i]; i++)
		wpath[i] = path[i];
	if (i > sizeof(wpath) - 4) {
		errno = EINVAL;
		return NULL;
	}
	wpath[i++] = L'\\';
	wpath[i++] = L'*';
	wpath[i++] = L'.';
	wpath[i++] = L'*';
	wpath[i] = L'\0';

	hFind = FindFirstFileW(wpath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		errno = ENOENT;
		return NULL;
	}

	result = malloc(sizeof(DIR));
	memcpy(&result->ffd, &ffd, sizeof(ffd));
	result->hFind = hFind;
	result->read = 0;
	copy_ffd_to_dirent(&ffd, &result->current);
	return result;
}

struct dirent * readdir(DIR * dir)
{
	struct dirent * result = NULL;

	if (dir->read > 0) {
		if (FindNextFile(dir->hFind, &dir->ffd))
			copy_ffd_to_dirent(&dir->ffd, &dir->current);
		else
			dir->current.d_name[0] = 0;
	}

	if (dir->current.d_name[0]) {
		result = &dir->current;
	}

	dir->read++;
	return result;
}
