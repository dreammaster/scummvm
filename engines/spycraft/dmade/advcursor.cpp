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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advres.h"
#include "spycraft/dmade/ats.h"
#include "spycraft/dmade/winmade.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advlist.h"
#include "spycraft/dmade/advcursor.h"
#include "spycraft/dmade/mcimovie.h"

namespace Spycraft {

int16 haveSetGameCursor = false;
int16 turnedOffWindowsCursor = false;
bool cursorHidden = false;
SpriteSharedPtr cursor;
static int mouse_x, mouse_y;
rsrcID curCursorID;
static int hiddenFlag = false;

extern int curBack;
extern int offsetX;
extern int offsetY;
extern int eraseWhenDone;

int sfxSetCursorPos(int x, int y) {
	POINT pt;

	pt.x = x + offsetX;
	pt.y = y + offsetY;

	SetCursorPos(pt.x, pt.y);

	if (cursor)
		sfxMoveSprite(cursor, pt.x, pt.y);

	return true;
}

void sfxCursorPriority(int priority) {
	if (cursor != nullptr)
		sfxSpriteChannel(cursor, (uint16)priority);
}

MADEErr sfxSetCursor(rsrcID id, int loop, int cell) {
	POINT pCursor;
	ATS *ats;
	Viewport *port;

	if (curBack < 0) {
		return fckFINoError;
	}

	if (id < 1)
		return noRsrcTypeError;

	ats = (ATS *)sfxLoadRes(id, RES_ATS);

	if (ats == nullptr)
		return noRsrcTypeError;

	sfxLockRes(id, RES_ATS);

	GetCursorPos(&pCursor);
	ScreenToClient(hGameWnd, &pCursor);
	mouse_x = (short)pCursor.x - offsetX;
	mouse_y = (short)pCursor.y - offsetY;

	port = ats->reels[loop]->frames[cell];

	if (cursor) {
		int theBack;
		SpriteArray *list;

		sfxKillSprite(cursor);

		if (curBack)
			theBack = 0;
		else
			theBack = 1;
		list = backgrounds[theBack]->spriteList;
		list->remove(Common::SharedPtr<Sprite>(cursor));

		sfxUnlockRes(curCursorID, RES_ATS);
		cursor = sfxCreateSprite(curBack, mouse_x, mouse_y, 512, 512, port);
	} else {
		cursor = sfxCreateSprite(curBack, mouse_x, mouse_y, 512, 512, port);
	}

	sfxAttachATS2Sprite(cursor, ats, loop, cell);
	curCursorID = id;

	sfxSpriteChannel(cursor, 0xffff);

	if (cursor == nullptr)
		return (fckFINoError);

	haveSetGameCursor = true;

	if (!turnedOffWindowsCursor) {
		ShowCursor(false);
		turnedOffWindowsCursor = 1;
	}

	cursorHidden = false;

	return 0;
}

void sfxShowCursor(void) {
	// no Made cursor
	if (cursor == nullptr)
		return;

	// System cursor active
	if (!turnedOffWindowsCursor)
		return;

	sfxShowSprite(cursor);
}

void sfxHideCursor(void) {
	// no Made cursor
	if (cursor == nullptr)
		return;

	// System cursor active
	if (!turnedOffWindowsCursor)
		return;

	sfxHideSprite(cursor);
}

void UpdateCursor(int x, int y) {
	mouse_x = x;
	mouse_y = y;

	// no Made cursor
	if (cursor == nullptr)
		return;

	// System cursor active
	if (!turnedOffWindowsCursor)
		return;

	if (!cursorHidden)
		sfxMoveSprite(cursor, x, y);
}

void InitCursorPos() {
	POINT pCursor;

	GetCursorPos(&pCursor);
	ScreenToClient(hGameWnd, &pCursor);
	UpdateCursor((WORD)pCursor.x, (WORD)pCursor.y);
}

void sfxSystemCursor(int cursorType) {
	if (cursor && !cursorHidden) {
		sfxHideSprite(cursor);
#ifdef DIRECTX
		if (!sfxIsMovieOpen() && turnedOffWindowsCursor) {
#else
		if (!sfxIsMCIOpen() && turnedOffWindowsCursor) {
#endif
			sfxReleaseSprites(GetBackground((short)curBack));
			sfxUpdate();
		}
		cursorHidden = true;
	}


	// Made cursor active
	if (turnedOffWindowsCursor) {
		turnedOffWindowsCursor = false;
		if (cursorType == SYS_WAIT)
			SetCursor(LoadCursor(nullptr, IDC_WAIT));
		else
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
		ShowCursor(true);
		if (cursor)
			haveSetGameCursor = false;
	} else {
		// System cursor active
		if (cursorType == SYS_WAIT)
			SetCursor(LoadCursor(nullptr, IDC_WAIT));
		else
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
	}
}

extern int window_right;
extern int window_bottom;

void sfxMadeCursor() {
	POINT pCursor;

	if (cursor == nullptr)
		return;

	// system cursor is active
	if (!turnedOffWindowsCursor) {
		cursorHidden = false;
		turnedOffWindowsCursor = true;
		haveSetGameCursor = true;

		ShowCursor(false);
		GetCursorPos(&pCursor);
		ScreenToClient(hGameWnd, &pCursor);
		if ((pCursor.x >= offsetX) && (pCursor.x <= window_right) &&
			(pCursor.y >= offsetY) && (pCursor.y <= window_bottom)) {
			sfxShowSprite(cursor);
			if (!sfxIsMCIOpen()) {
				sfxReleaseSprites(GetBackground((short)curBack));
				sfxUpdate();
			}
		}

		UpdateCursor(pCursor.x - offsetX, pCursor.y - offsetY);
	}
}

void sfxWaitCursorOn() {
	if (cursor && !(cursor->state & SPRITE_HIDDEN)) {
		hiddenFlag = true;
		sfxHideCursor();
		sfxReleaseSprites(backgrounds[curBack]);
		sfxUpdate();
		ShowCursor(true);
	}

	SetCursor(LoadCursor(nullptr, IDC_WAIT));
}

void sfxWaitCursorOff() {
	if (hiddenFlag) {
		sfxShowCursor();
		ShowCursor(false);
	}
}

} // namespace Spycraft
