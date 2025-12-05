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

#ifndef SPYCRAFT_LOGIC_41100_H
#define SPYCRAFT_LOGIC_41100_H

#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm41100 : public Room
{
public:
	Rm41100();
	~Rm41100();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit41100 : public SouthExit
{
public:
	SouthExit41100();
	void respond(void);
	int doVerb(int);
};

class NorthExit41100 : public ExitFeature
{
public:
	NorthExit41100();
	void respond(void);
	int doVerb(int);
};

class SetFlag41100 : public TextFeature
{
public:
	SetFlag41100();
	void respond(void);
	int doVerb(int);
};

class SetFlag41101 : public TextFeature
{
public:
	SetFlag41101();
	void respond(void);
	int doVerb(int);
};

class SetFlag41102 : public TextFeature
{
public:
	SetFlag41102();
	void respond(void);
	int doVerb(int);
};

class SetFlag41103 : public TextFeature
{
public:
	SetFlag41103();
	void respond(void);
	int doVerb(int);
};

class SetFlag41104 : public TextFeature
{
public:
	SetFlag41104();
	void respond(void);
	int doVerb(int);
};

class SetFlag41105 : public TextFeature
{
public:
	SetFlag41105();
	void respond(void);
	int doVerb(int);
};

class SetFlag41106 : public TextFeature
{
public:
	SetFlag41106();
	void respond(void);
	int doVerb(int);
};

class SetFlag41107 : public TextFeature
{
public:
	SetFlag41107();
	void respond(void);
	int doVerb(int);
};

class SetFlag41108 : public TextFeature
{
public:
	SetFlag41108();
	void respond(void);
	int doVerb(int);
};

class SetFlag41109 : public TextFeature
{
public:
	SetFlag41109();
	void respond(void);
	int doVerb(int);
};

class SetFlag41110 : public TextFeature
{
public:
	SetFlag41110();
	void respond(void);
	int doVerb(int);
};

class SetFlag41111 : public TextFeature
{
public:
	SetFlag41111();
	void respond(void);
	int doVerb(int);
};

class SetFlag41112 : public TextFeature
{
public:
	SetFlag41112();
	void respond(void);
	int doVerb(int);
};

class SetFlag41113 : public TextFeature
{
public:
	SetFlag41113();
	void respond(void);
	int doVerb(int);
};

class SetFlag41114 : public TextFeature
{
public:
	SetFlag41114();
	void respond(void);
	int doVerb(int);
};

class Rm41200 : public Room
{
public:
	Rm41200();
	~Rm41200();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit41200 : public SouthExit
{
public:
	SouthExit41200();
	void respond(void);
	int doVerb(int);
};

class NorthExit41200 : public ExitFeature
{
public:
	NorthExit41200();
	void respond(void);
	int doVerb(int);
};

class SetFlag41115 : public TextFeature
{
public:
	SetFlag41115();
	void respond(void);
	int doVerb(int);
};

class SetFlag41116 : public TextFeature
{
public:
	SetFlag41116();
	void respond(void);
	int doVerb(int);
};

class SetFlag41117 : public TextFeature
{
public:
	SetFlag41117();
	void respond(void);
	int doVerb(int);
};

class SetFlag41118 : public TextFeature
{
public:
	SetFlag41118();
	void respond(void);
	int doVerb(int);
};

class SetFlag41119 : public TextFeature
{
public:
	SetFlag41119();
	void respond(void);
	int doVerb(int);
};

class SetFlag41120 : public TextFeature
{
public:
	SetFlag41120();
	void respond(void);
	int doVerb(int);
};

class Rm41300 : public Room
{
public:
	Rm41300();
	~Rm41300();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit41300 : public SouthExit
{
public:
	SouthExit41300();
	void respond(void);
	int doVerb(int);
};

class NorthExit41300 : public ExitFeature
{
public:
	NorthExit41300();
	void respond(void);
	int doVerb(int);
};

class Rm41400 : public Room
{
public:
	Rm41400();
	~Rm41400();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit41400 : public SouthExit
{
public:
	SouthExit41400();
	void respond(void);
	int doVerb(int);
};

/* features */
class Monitor41400 : public Feature
{
public:
	Monitor41400();
	void respond(void);
	int doVerb(int);
};

class Rm41500 : public Room
{
public:
	Rm41500();
	~Rm41500();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit41500 : public SouthExit
{
public:
	SouthExit41500();
	void respond(void);
	int doVerb(int);
};

/* features */
class Screen41500 : public Feature
{
public:
	Screen41500();
	void respond(void);
	int doVerb(int);
};

/* script */
class EnterScript41500 : public Script
{
public:
	EnterScript41500();
	void changeState(int);
};

class EnterBadmanPuzzle41500 : public Script
{
public:
	EnterBadmanPuzzle41500();
	void changeState(int);
};

} // namespace Spycraft

#endif

