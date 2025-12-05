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

#ifndef SPYCRAFT_LOGIC_50000_H
#define SPYCRAFT_LOGIC_50000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm50000 : public Room
{
public:
	Rm50000();
	~Rm50000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit50000 : public SouthExit
{
public:
	SouthExit50000();
	void respond(void);
	int doVerb(int);
};

/* features */
class BloodyCamera50000 : public Feature
{
public:
	BloodyCamera50000();
	void respond(void);
	int doVerb(int);
};

class Blanket50000 : public Feature
{
public:
	Blanket50000();
	void respond(void);
	int doVerb(int);
};

class Garbage50000 : public Feature
{
public:
	Garbage50000();
	void respond(void);
	int doVerb(int);
};

class JunkyWall50000 : public Feature
{
public:
	JunkyWall50000();
	void respond(void);
	int doVerb(int);
};

/* insets */
class BloodyCameraInset50000 : public Inset
{
public:
	BloodyCameraInset50000();
	//	void respond();
};

class BlanketInset50000 : public Inset
{
public:
	BlanketInset50000();
	//	void respond();
};

class GarbageInset50000 : public Inset
{
public:
	GarbageInset50000();
	//	void respond();
};

class JunkyWallInset50000 : public Inset
{
public:
	JunkyWallInset50000();
	//	void respond();
};

/* script */
class EnterScript50000 : public Script
{
public:
	EnterScript50000();
	void changeState(int);
};

class EnterBlackScreenScript50300 : public Script
{
public:
	EnterBlackScreenScript50300();
	void changeState(int);
};

class Rm50001 : public Room
{
public:
	Rm50001();
	~Rm50001();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit50001 : public SouthExit
{
public:
	SouthExit50001();
	void respond(void);
	int doVerb(int);
};

class CameraChip50001 : public View
{
public:
	CameraChip50001();
	void respond(void);
	int doVerb(int);
};

class Rm50100 : public Room
{
public:
	Rm50100();
	~Rm50100();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit50100 : public SouthExit
{
public:
	SouthExit50100();
	void respond(void);
	int doVerb(int);
};

class Rm50200 : public Room
{
public:
	Rm50200();
	~Rm50200();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit50200 : public SouthExit
{
public:
	SouthExit50200();
	void respond(void);
	int doVerb(int);
};

class Rm50300 : public Room
{
public:
	Rm50300();
	~Rm50300();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit50300 : public SouthExit
{
public:
	SouthExit50300();
	void respond(void);
	int doVerb(int);
};

class NorthExit50300 : public ExitFeature
{
public:
	NorthExit50300();
	void respond(void);
	int doVerb(int);
};

class GetCamera50000 : public Script
{
public:
	GetCamera50000();
	void changeState(int);
};

} // namespace Spycraft

#endif

