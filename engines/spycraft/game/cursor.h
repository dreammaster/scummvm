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

#ifndef SPYCRAFT_GAME_CURSOR_H
#define SPYCRAFT_GAME_CURSOR_H

#include "spycraft/game/actor.h"
#include "spycraft/game/motion.h"

namespace Spycraft {

class Cursor : public View {
public:
	Cursor();

	void init(void) {
	};
	void posn(int, int);
	virtual void setCycle(int) {
	};
};

class BigCursor : public Cursor {
public:
	BigCursor();

	void doit(void);
	void init(void);
};

class MadeCursor : public Cursor {
public:
	MADEPt orig_pt;

	MadeCursor();
	void init(void);
};

class BlinkCursor : public MadeCursor {
protected:
	unsigned long __cycle_cnt = 0;
	int visible = 0;
	int blinking = 0;
	short hidden = 0;
public:
	int	speed = 0;

	BlinkCursor();
	void rest(void);
	void blink();
	void blink(int theSpeed);
	void doit(void);
	void hide(void);
	void show(void);
};

class AniCursor : public MadeCursor {
protected:
	unsigned long __cycle_cnt = 0;

public:
	int	cycleSpeed = 0;
	int cycler = 0;
	int curCel = 0;
	int lastCel = 0;

	AniCursor();
	~AniCursor();

	void doit(void);
	void blink(void);
	void rest(void);
	int nextCel(void);
	void setCycle(int);

};

} // namespace Spycraft

#endif
