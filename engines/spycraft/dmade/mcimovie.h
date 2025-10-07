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

#ifndef SPYCRAFT_DMADE_ADVERROR_H
#define SPYCRAFT_DMADE_ADVERROR_H

#include "spycraft/dmade/advrect.h"

namespace Spycraft {

enum {
	ADV_MCI_WINDOW,
	ADV_MCI_MODEX
};

struct MCIMovie {
	UINT movieID;
	char *aliasName;
	int	mode;
	SRect	rect;
};

// callbackFunc - pointer to a function for directed callbacks
typedef void (*callbackFunc) ();

extern void sfxCheckMCI(WPARAM wParam, LPARAM lParam);
extern void sfxCloseMCI(void);
extern int sfxOpenMCI(char *filename);
extern int sfxPlayMCI(int mode, int wait, int close_when_done, int erase_when_done,
	SRect *rect, callbackFunc doneCallback);
extern void sfxStepMCI(int steps);
extern int sfxGetPositionMCI(void);
extern void sfxPauseMCI(bool repaint, bool call_callback);
extern void sfxResumeMCI(void);
extern void sfxSeekMCI(int offset, int mode);
extern bool sfxFromToMCI(int from, int to);
extern bool sfxIsMCIOpen(void);
extern int RegisterMCI();
extern void sfxAddMCI2Pic(void);
extern int CleanMCI(void);
extern int CanReplayMovie(void);
extern int HavePlayedMovie(void);
extern void ReplayMovie(void);
extern void sfxSetVideoOffMCI(int Off);

} // namespace Spycraft

#endif
