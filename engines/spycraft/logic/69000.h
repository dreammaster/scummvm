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

#ifndef SPYCRAFT_LOGIC_69000_H
#define SPYCRAFT_LOGIC_69000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

class Rm69002 : public Room
{
public:
	Rm69002();
	~Rm69002();
	void cue(void);
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm69003 : public Room
{
public:
	Rm69003();
	~Rm69003();
	void cue(void);
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm69000 : public Room
{
public:
	Rm69000();
	~Rm69000();
	void cue(void);
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit69000 : public SouthExit
{
public:
	SouthExit69000();
	void respond(void);
	int doVerb(int);
};

class EastExit69000 : public ExitFeature
{
public:
	EastExit69000();
	void respond(void);
	int doVerb(int);
};

class WestExit69000 : public ExitFeature
{
public:
	WestExit69000();
	void respond(void);
	int doVerb(int);
};

/* feature */

class Onyx69000 : public Feature
{
public:
	Onyx69000();
	int doVerb(int);
	void respond(void);
};

/* scripts */
class EnterScript69000 : public Script
{
public:
	EnterScript69000();
	void changeState(int);
};

class OnyxScript69000 : public Script
{
public:
	Sound *onyxSound;
	OnyxScript69000();
	void changeState(int);
};

// views //

class Cuffs69000 : public View
{
public:
	Cuffs69000();
	void respond(void);
	int doVerb(int);
};

class CuffCursor : public MadeCursor
{
public:
	CuffCursor();
};

// custom ditem //
class DialogFlag : public DItem
{
public:
	DialogFlag() {
		name = "DialogFlag";
	};
	void doit(void);
};

} // namespace Spycraft

#endif

