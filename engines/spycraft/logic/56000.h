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

#ifndef SPYCRAFT_LOGIC_56000_H
#define SPYCRAFT_LOGIC_56000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "globals.h"

namespace Spycraft {

class Rm56000 : public Room
{
public:
	Rm56000();
	~Rm56000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class NorthExit56000 : public ExitFeature
{
public:
	NorthExit56000();
	void respond(void);
	int doVerb(int);
};

class SouthExit56000 : public SouthExit
{
public:
	SouthExit56000();
	void respond(void);
	int doVerb(int);
};

class Rm56001 : public Room
{
public:
	Rm56001();
	~Rm56001();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class EnterBlackScreenScript56001 : public Script
{
public:
	EnterBlackScreenScript56001();
	void changeState(int);
};

} // namespace Spycraft

#endif

