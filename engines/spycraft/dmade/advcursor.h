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

#ifndef SPYCRAFT_DMADE_ADVCURSOR_H
#define SPYCRAFT_DMADE_ADVCURSOR_H

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/adverror.h"
#include "spycraft/dmade/advsprite.h"

namespace Spycraft {

#define SYS_ARROW 10
#define SYS_WAIT 12

extern int16 turnedOffWindowsCursor;
extern int16 haveSetGameCursor;
extern int cursorCount;
extern SpriteSharedPtr cursor;

/* MADE External */

//	Function:	sfxHideCursor
//
//	Purpose:	makes the cursor invisible.
//
//	Parameters:	none
//
//	Returns:	none

extern void sfxHideCursor(void);


//	Function:	sfxShowCursor
//
//	Purpose:	makes the cursor visible.
//
//	Parameters:	none
//
//	Returns:	none

extern void sfxShowCursor(void);


//	Function:	sfxSetCursor
//
//	Purpose:	changes the cursor to the specified resource
//
//	Parameters: id - ID of the resource to use
//				hot_spot - ptr to the hot spot (virtual).
//
//	Returns:	0 if successful, else error

extern MADEErr sfxSetCursor(rsrcID id, int loop, int cell);


//	Function:	sfxCursorPriority
//
//	Purpose:	changes the cursor channel number (priority)
//
//	Parameters: priority - new channel number
//
//	Returns:	void

extern void sfxCursorPriority(int priority);


//	Function:	sfxSetCursorPos
//
//	Purpose:	moves the cursor to the specified position
//
//	Parameters: Game window X and Y coordinates
//
//	Returns:	true if successful, else false

extern int sfxSetCursorPos(int x, int y);


//	Function:	sfxSystemCursor
//
//	Purpose:	Hides game cursor, if any. Shows specified Windows system cursor.
//
//	Parameters: int cursorType	( SYS_ARROW or SYS_WAIT )
//
//	Returns:	none

extern void sfxSystemCursor(int cursorType);


//	Function:	sfxMadeCursor
//
//	Purpose:	Hides Windows system cursor. Shows game cursor.
//
//	Parameters: none
//
//	Returns:	none

extern void sfxMadeCursor(void);

extern void sfxWaitCursorOn(void);
extern void sfxWaitCursorOff(void);

extern void UpdateCursor(int x, int y);
extern void AddCursor(Background *cur_background, Background *new_background);
extern void InitCursorPos();

} // namespace Spycraft

#endif
