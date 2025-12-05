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

#ifndef SPYCRAFT_LOGIC_10000_H
#define SPYCRAFT_LOGIC_10000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"

namespace Spycraft {

class Rm10000 : public Room
{
public:
	Rm10000();
	void init(void);
	//int handleEvent ( MADEEventStamp * );
};

class Rm10100 : public Room
{
public:
	Rm10100();
	void init(void);
};

class Rm10200 : public Room
{
public:
	Rm10200();
	void init(void);
};

/* exit features */
class SouthExit10000 : public ExitFeature
{
public:
	SouthExit10000();
	void respond(void);
	int doVerb(int);
};

class SouthExit10100 : public ExitFeature
{
public:
	SouthExit10100();
	void respond(void);
	int doVerb(int);
};

class SouthExit10200 : public ExitFeature
{
public:
	SouthExit10200();
	void respond(void);
	int doVerb(int);
};

/* features */
class House10000 : public Feature
{
public:
	House10000();
	void respond(void);
	int	doVerb(int);
};

class Door10100 : public Feature
{
public:
	Door10100();
	void respond(void);
	int	doVerb(int);
};

class Knocker10200 : public Feature
{
public:
	Knocker10200();
	void respond(void);
	int	doVerb(int);
};

/* views */
class HouseView10100 : public View
{
public:
	HouseView10100();
};

class DoorView10200 : public View
{
public:
	DoorView10200();
};

/* scripts */
class KnockScript10200 : public Script
{
public:
	Sound *soundptr;
	KnockScript10200() {
		name = "KnockScript10200";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif

