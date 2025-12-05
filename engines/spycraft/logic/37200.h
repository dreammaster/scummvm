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

#ifndef SPYCRAFT_LOGIC_37200_H
#define SPYCRAFT_LOGIC_37200_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

namespace Spycraft {

class Rm37200 : public Room
{
public:
	Rm37200();
	~Rm37200();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit37200 : public SouthExit
{
public:
	SouthExit37200();
	void respond(void);
	int doVerb(int);
};

class WestExit37200 : public ExitFeature
{
public:
	WestExit37200();
	void respond(void);
	int doVerb(int);
};

/* scripts */
class EnterStickScript37200 : public Script
{
public:
	EnterStickScript37200();
	void changeState(int);
};

class EnterCarrotScript37200 : public Script
{
public:
	EnterCarrotScript37200();
	void changeState(int);
};

/* Room 37201 */
class Rm37201 : public Room
{
public:
	Rm37201();
	~Rm37201();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class EnterBlackScreenScript37201 : public Script
{
public:
	EnterBlackScreenScript37201();
	void changeState(int);
};

} // namespace Spycraft

#endif

