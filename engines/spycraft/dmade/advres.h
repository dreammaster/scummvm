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

#ifndef SPYCRAFT_DMADE_ATS_H
#define SPYCRAFT_DMADE_ATS_H

#include "spycraft/dmade/advlist.h"
#include "spycraft/dmade/ats.h"

namespace Spycraft {

typedef unsigned long rsrcID;

enum {
	RES_ATS,
	RES_WAVE,
	RES_FIF,
	RES_MIDI,
	RES_PIC,
	RES_FONT,
	RES_TEXT,
	RES_MOVIE,
	RES_HTM,
	RES_FACE,
	RES_MMIDI
};

enum {
	RES_IN_MEM,
	RES_IN_DISK
};

struct GenericData {
	int id = 0;
	uint time = 0;
	int mem = 0;
	int location = 0;
	int locked = 0;
	void *data = nullptr;
};

struct ResCache {
	ArrayList *list = nullptr;
	int size = 0;
};

struct VolInfo {
	char *filename = nullptr;
	int location = 0;
};

struct ResInfo {
	int res_pic_size = 0;
	int res_ats_size = 0;
	int res_wav_size = 0;
	int res_fif_size = 0;
	int res_mid_size = 0;
	int res_fon_size = 0;
	int res_tex_size = 0;
	int res_face_size = 0;
};

/* INTERNAL CALLS */
extern void ID2File(char *dest, int id, int type);
extern GenericData *SearchRESData(int type, int id);
extern void SetRESCache(void);
extern void DestroyRESCache(void);

extern char sysDir[];
extern ResInfo resInfo;

extern void *sfxLoadRes(int id, int type);
extern int sfxPurgeRes(int id, int type);
extern int sfxLockRes(int id, int type);
extern int sfxUnlockRes(int id, int type);
extern void sfxSetIniFile(char *name);
extern int sfxCheckRes(int id, int type);
extern int sfxGetDir(char *dest, int id, int type);
extern void sfxSetCacheSize(int type, int value);

} // namespace Spycraft

#endif
