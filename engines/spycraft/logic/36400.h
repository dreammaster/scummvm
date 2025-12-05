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

#ifndef SPYCRAFT_LOGIC_36400_H
#define SPYCRAFT_LOGIC_36400_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

namespace Spycraft {

class Rm36400 : public Room
{
public:
	Rm36400();
	~Rm36400();
	void cue();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit36400 : public SouthExit
{
public:
	SouthExit36400();
	void respond(void);
	int doVerb(int);
};

/* scripts */
class EnterScript36400 : public Script
{
public:
	EnterScript36400();
	void changeState(int);
};

/* dialogs */
class ProveDeed : public DItem
{
public:
	ProveDeed();
	void doit(void);
};

class DoneHaveHat2 : public DItem
{
public:
	DoneHaveHat2();
	void doit(void);
};

class AddMeetBird2 : public DItem
{
public:
	AddMeetBird2();
	void doit(void);
};

class AddWeDo2 : public DItem
{
public:
	AddWeDo2();
	void doit(void);
};

} // namespace Spycraft

#endif			
