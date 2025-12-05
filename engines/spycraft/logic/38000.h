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

#ifndef SPYCRAFT_LOGIC_38000_H
#define SPYCRAFT_LOGIC_38000_H

//#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm38000 : public Room
{
public:
	Rm38000();
	~Rm38000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit38000 : public SouthExit
{
public:
	SouthExit38000();
	void respond(void);
	int doVerb(int);
};

/* feature */
class InsideVan38000 : public Feature
{
public:
	InsideVan38000();
	void respond(void);
	int doVerb(int);
};

class Rm38100 : public Room
{
public:
	Rm38100();
	~Rm38100();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit38100 : public SouthExit
{
public:
	SouthExit38100();
	void respond(void);
	int doVerb(int);
};

class FolderHole38100 : public Feature
{
public:
	FolderHole38100();
	void respond(void);
	int doVerb(int);
};

class HandOverFileScript38100 : public Script
{
public:
	HandOverFileScript38100();
	void changeState(int);
};

class Rm38200 : public Room
{
public:
	Rm38200();
	~Rm38200();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

/* feature */
class VanHole38100 : public Feature
{
public:
	VanHole38100();
	void respond(void);
	int doVerb(int);
};

/* scripts */
class ProcatTeamScript38100 : public Script
{
public:
	ProcatTeamScript38100();
	void changeState(int);
};

class StupidThornScript38100 : public Script
{
public:
	StupidThornScript38100();
	void changeState(int);
};

class Rm38300 : public Room
{
public:
	Rm38300();
	~Rm38300();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit38300 : public SouthExit
{
public:
	SouthExit38300();
	void respond(void);
	int doVerb(int);
};

class PegBullet38300 : public View
{
public:
	PegBullet38300();
	void respond(void);
	int doVerb(int);
};

class PegBulletInset38300 : public Inset
{
public:
	PegBulletInset38300();
	//	void respond();
};

class Rm38400 : public Room
{
public:
	Rm38400();
	~Rm38400();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit38400 : public SouthExit
{
public:
	SouthExit38400();
	void respond(void);
	int doVerb(int);
};

class NorthExit38400 : public ExitFeature
{
public:
	NorthExit38400();
	void respond(void);
	int doVerb(int);
};

class Rm38500 : public Room
{
public:
	Rm38500();
	~Rm38500();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

} // namespace Spycraft

#endif

