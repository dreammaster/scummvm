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

#ifndef SPYCRAFT_LOGIC_25000_H
#define SPYCRAFT_LOGIC_25000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm25000 : public Room
{
public:
	Rm25000();
	~Rm25000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit25000 : public ExitFeature
{
public:
	SouthExit25000();
	void respond(void);
	int doVerb(int);
};

class EastExit25000 : public ExitFeature
{
public:
	EastExit25000();
	void respond(void);
	int doVerb(int);
};

/* feature */
class ControlPanel25000 : public Feature
{
public:
	ControlPanel25000();
	void respond(void);
	int doVerb(int);
};

/* inset */
class ControlPanelInset25000 : public Inset
{
public:
	ControlPanelInset25000();
	//	void respond();
	//	int doVerb( int);
};

/* scripts */
class EnterScript25000 : public Script
{
public:
	EnterScript25000();
	void changeState(int);
};

} // namespace Spycraft

#endif

