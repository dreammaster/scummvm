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

#include "spycraft/game/cursor.h"
#include "spycraft/game/user.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/invent.h"

namespace Spycraft {

extern int currentFocus;

Cursor::Cursor() {
	name = "Cursor";
}

void Cursor::posn(int theX, int theY) {
	sfxSetCursorPos(theX, theY);
	x = theX;
	y = theY;
}

MadeCursor::MadeCursor() {
	name = "MadeCursor";
}

void MadeCursor::init() {
	sfxSetCursor(view, loop, cel);
}


BlinkCursor::BlinkCursor() {
	name = "BlinkCursor";
	blinking = false;
	speed = 0;
}

void BlinkCursor::hide(void) {
	if (!hidden) {
		sfxHideCursor();
		hidden = true;
	}
}

void BlinkCursor::show(void) {
	if (hidden) {
		sfxShowCursor();
		hidden = false;
	}
}

void BlinkCursor::blink() {
	if (view == 906)
	{
		sfxSetCursor(906, 0, 0);
		sfxCursorPriority(99);
	} else {
		if (invVerb) {
			for (int i = 0; i < MAX_INV; i++) {
				if ((inventry->items[i]->verb == invVerb) && (hotObj)
					&& ((hotObj->checkRespondVerb(invVerb)) || (hotObj->checkRespondVerb(ALL_V)))) {
					sfxSetCursor(960, inventry->items[i]->handView, 0);
					return;
				}
			}
			sfxSetCursor(970, 0, 1);
		} else
			sfxSetCursor(970, 0, 1);
	}
}

void BlinkCursor::blink(int theSpeed) {
	if (view == 906)
	{
		sfxSetCursor(906, 0, 0);
		sfxCursorPriority(99);
	} else {
		if (invVerb) {
			for (int i = 0; i < MAX_INV; i++) {
				if ((inventry->items[i]->verb == invVerb) && (hotObj)
					&& ((hotObj->checkRespondVerb(invVerb)) || (hotObj->checkRespondVerb(ALL_V)))) {
					sfxSetCursor(960, inventry->items[i]->handView, 0);
					return;
				}
			}
			sfxSetCursor(970, 0, 1);
		} else
			sfxSetCursor(970, 0, 1);
	}
}

void BlinkCursor::rest(void) {
	if (view == 906)
	{
		sfxSetCursor(906, 0, 0);
		sfxCursorPriority(99);
	} else
		if (currentFocus != PUZZLE_FOCUS)
			sfxSetCursor(970, 0, 0);
		else
			sfxSetCursor(970, 0, 1);

}

void BlinkCursor::doit() {
	if (!hidden && blinking && ((gameTime - __cycle_cnt) > (unsigned)speed)) {

		__cycle_cnt = gameTime;
		if (visible) {
			sfxHideCursor();
			visible = false;
		} else {
			sfxShowCursor();
			visible = true;
		}
	}
}

AniCursor::AniCursor() {
	name = "AniCursor";
	cycler = NULL;

	__cycle_cnt = 0;
	cycleSpeed = 0;
	curCel = 0;
	cel = 9;
	lastCel = 9; //DEBUG
}

AniCursor::~AniCursor() {
}

void AniCursor::blink(void) {
	setCycle(true);
}

void AniCursor::setCycle(int stopOrGo) {
	if (stopOrGo)
		cel = 0;
	cycler = stopOrGo;
}

void AniCursor::rest(void) {
	setCycle(false);

}

void AniCursor::doit() {
	if (cycler && ((gameTime - __cycle_cnt) > (unsigned)cycleSpeed)) {
		__cycle_cnt = gameTime;

		int oldCel = cel;
		cel = nextCel();
		if (curCel == lastCel)
			rest();
		if ((oldCel != cel) && (cycler))
			sfxSetCursor(970, loop, cel);

	}
}

int AniCursor::nextCel() {
	if (curCel < lastCel)
		return (++curCel);
	else {
		curCel = 0;
		return 0;
	}
}

} // namespace Spycraft
