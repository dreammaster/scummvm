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

#ifndef SPYCRAFT_LOGIC_36500_H
#define SPYCRAFT_LOGIC_36500_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm36500 : public Room
{
public:
	Rm36500();
	~Rm36500();
	void cue();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit36500 : public SouthExit
{
public:
	SouthExit36500();
	void respond(void);
	int doVerb(int);
};

/* scripts */
class EnterScript36500 : public Script
{
public:
	EnterScript36500();
	void changeState(int);
};

/* dialogs */
class DoneHaveHat3 : public DItem
{
public:
	DoneHaveHat3();
	void doit(void);
};

class NumberofYuri3 : public DItem
{
public:
	NumberofYuri3();
	void doit(void);
};

class AgainThanks3 : public DItem
{
public:
	AgainThanks3();
	void doit(void);
};

} // namespace Spycraft

#endif			
