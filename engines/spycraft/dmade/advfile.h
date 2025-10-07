/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SPYCRAFT_DMADE_ADVFILE_H
#define SPYCRAFT_DMADE_ADVFILE_H

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advcursor.h"

namespace Spycraft {

enum {
	MADE_FILE_WRITE,
	MADE_FILE_READ,
	MADE_FILE_APPEND
};

enum {
	MADE_SEEK_BEG,
	MADE_SEEK_CUR,
	MADE_SEEK_END
};

extern int sfxOpenFile(const char *filename, int mode);
extern int sfxReadFile(int hf, void *buffer, int size);
extern char *sfxReadFileString(int hf);
extern int sfxGetString(int hf, char *dest);
extern int sfxWriteFile(int hf, void *buffer, int size);
extern int sfxWriteASCFile(int hf, const char *, ...);
extern int sfxSeekFile(int hf, int offset, int mode);
extern int sfxCloseFile(int hf);
extern int sfxFileSize(char *filename);
extern uint8 sfxFileGetBYTE(int hf);
extern uint16 sfxFileGetWORD(int hf);
extern void OpenDlg(const char *fileMask);
extern void SaveDlg(const char *fileMask);
extern char *sfxGetRestoreFile(void);
extern char *sfxGetSaveFile(void);
extern int sfxCopyFile(const char *src, const char *dest);

} // namespace Spycraft

#endif
