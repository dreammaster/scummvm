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

#include "common/stream.h"
#include "spycraft/afxwin.h"
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

typedef Common::Stream *FHANDLE;

extern FHANDLE sfxOpenFile(const char *filename, int mode);
extern int sfxReadFile(FHANDLE hf, void *buffer, int size);
extern char *sfxReadFileString(FHANDLE hf);
extern int sfxGetString(FHANDLE hf, char *dest);
extern int sfxWriteFile(FHANDLE hf, void *buffer, int size);
extern int sfxWriteASCFile(FHANDLE hf, const char *, ...);
extern int sfxSeekFile(FHANDLE hf, int offset, int mode);
extern int sfxCloseFile(FHANDLE hf);
extern int sfxFileSize(const char *filename);
extern int sfxGetFileSize(FHANDLE hf);
extern uint8 sfxFileGetBYTE(FHANDLE hf);
extern uint16 sfxFileGetWORD(FHANDLE hf);
extern void OpenDlg(const char *fileMask);
extern void SaveDlg(const char *fileMask);
extern char *sfxGetRestoreFile();
extern char *sfxGetSaveFile();
extern bool sfxCopyFile(const char *src, const char *dest);

} // namespace Spycraft

#endif
