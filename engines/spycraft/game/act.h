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

#ifndef SPYCRAFT_ACT_H
#define SPYCRAFT_ACT_H

#include "spycraft/game/actor.h"
#include "spycraft/game/motion.h"

namespace Spycraft {

class MoveTo;

class Actor : public Prop {
public:
	unsigned __move_cnt;

	int xInc;
	int yInc;
	int moveSpeed;
	MoveTo *mover;
	void setSpeed(int theSpeed);
	void setInc(int theXInc, int theYInc);
	Actor();
	~Actor();
	void doit();
	virtual void setMotion(MoveTo *theMover);
};

class MoveTo : public Object {
public:
	Actor *client;
	Object *caller;
	int destX;
	int destY;
	int nextX;
	int nextY;
	int xInc;
	int yInc;
	int error;
	int xDiff;
	int yDiff;
	MoveTo(int theX, int theY, Actor *theClient);
	MoveTo(int theX, int theY, Actor *theClient, Object *whoToCue);
	void motionCue();
	void setUpBres();
	void doBresen();
	void doit();
};

} // namespace Spycraft

#endif
