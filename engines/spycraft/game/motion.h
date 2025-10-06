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

#ifndef SPYCRAFT_GAME_MOTION_H
#define SPYCRAFT_GAME_MOTION_H

#include "spycraft/game/object.h"

namespace Spycraft {

class Prop;

class Cycle : public Object {
public:
	int	lastCel;
	int	curCel;

	Prop *client;
	Object *caller;

	Cycle();

	void doit(void);

	virtual void motionCue(void);
	int nextCel(void);
	int prevCel(void);
};

class EndLoop : public Cycle {
public:
	EndLoop(Prop *theProp);
	EndLoop(Prop *theProp, Object *theObj);

	void doit(void);
};

class Forward : public Cycle {
public:
	Forward(Prop *theProp);
	void doit(void);
};


class BegLoop : public Cycle {
public:
	BegLoop(Prop *theProp);
	BegLoop(Prop *theProp, Object *theObj);
	void doit(void);
};

} // namespace Spycraft

#endif
