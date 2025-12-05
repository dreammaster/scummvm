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

#ifndef SPYCRAFT_LOGIC_36600_H
#define SPYCRAFT_LOGIC_36600_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm36600 : public Room
{
public:
	Rm36600();
	~Rm36600();
	void init(void);
	void cue(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit36600 : public SouthExit
{
public:
	SouthExit36600();
	void respond(void);
	int doVerb(int);
};

/* scripts */
class EnterScript36600 : public Script
{
public:
	EnterScript36600();
	void changeState(int);
};

/* dialogs */
class InfoDeed : public DItem
{
public:
	InfoDeed();
	void doit(void);
};

class TellAboutBS : public DItem
{
public:
	TellAboutBS();
	void doit(void);
};

class DoneHaveHat : public DItem
{
public:
	DoneHaveHat();
	void doit(void);
};

class NumberofYuri : public DItem
{
public:
	NumberofYuri();
	void doit(void);
};

class AddMeetBird : public DItem
{
public:
	AddMeetBird();
	void doit(void);
};

class AddEveryoneNoise : public DItem
{
public:
	AddEveryoneNoise();
	void doit(void);
};

} // namespace Spycraft

#endif			
