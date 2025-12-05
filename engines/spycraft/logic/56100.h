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

#ifndef SPYCRAFT_LOGIC_56100_H
#define SPYCRAFT_LOGIC_56100_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/inset.h"
#include "spycraft/game/flag.h"

namespace Spycraft {

class Rm56100 : public Room
{
public:
	Rm56100();
	~Rm56100();
	void init();
	int handleEvent(MADEEventStamp *);
};

/* exit features */
/*
class NorthExit56100 : public ExitFeature
{
public:
	NorthExit56100();
	void respond( void );
	int doVerb( int );
};
*/
class SouthExit56100 : public SouthExit
{
public:
	SouthExit56100();
	void respond(void);
	int doVerb(int);
};

/* features */
class Doorknob56100 : public Feature
{
public:
	Doorknob56100();
	void respond(void);
	int doVerb(int);
};

class KnockDoor56100 : public Feature
{
public:
	KnockDoor56100();
	void respond(void);
	int doVerb(int);
};
/*
class Door56100 : public Feature
{
public:
	Door56100();
	void respond( void );
	int doVerb( int );
};
*/
/* insets */
class DoorknobInset56100 : public Inset
{
public:
	DoorknobInset56100();
	void dispose();
};

/* inset features */
class LockInsetFeature56100 : public InsetFeature
{
public:
	LockInsetFeature56100();
	void respond(void);
	int doVerb(int);
};

/* scripts */
/*
class DoorScript56100 : public Script
{
 public:
	DoorScript56100();
	void changeState( int );
};
*/
class PickLockScript56100 : public Script
{
public:
	PickLockScript56100();
	void changeState(int);
};

/*
class Rm56200 : public Room
{
public:
	Rm56200();
	~Rm56200();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class RoomEnter56200 : public Script
{
public:
	RoomEnter56200();
	void changeState ( int );
};
*/
class Rm56101 : public Room
{
public:
	Rm56101();
	~Rm56101();
	void init();
	int handleEvent(MADEEventStamp *);
};

class SouthExit56101 : public SouthExit
{
public:
	SouthExit56101();
	void respond(void);
	int doVerb(int);
};

class WestExit56101 : public ExitFeature
{
public:
	WestExit56101();
	void respond(void);
	int doVerb(int);
};

class EastExit56101 : public ExitFeature
{
public:
	EastExit56101();
	void respond(void);
	int doVerb(int);
};

class OpenBSLock56101 : public Feature
{
public:
	OpenBSLock56101();
	void respond(void);
	int doVerb(int);
};

} // namespace Spycraft

#endif
