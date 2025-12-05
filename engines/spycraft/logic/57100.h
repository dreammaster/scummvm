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

#ifndef SPYCRAFT_LOGIC_57100_H
#define SPYCRAFT_LOGIC_57100_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/inset.h"
#include "spycraft/game/flag.h"

namespace Spycraft {

class Rm57100 : public Room
{
public:
	Rm57100();
	~Rm57100();
	void init();
	void cue();
	int handleEvent(MADEEventStamp *);
};

class EastExit57100 : public ExitFeature
{
public:
	EastExit57100();
	void respond(void);
	int doVerb(int);
};

/* features */
class Calendar57100 : public Feature
{
public:
	Calendar57100();
	void respond(void);
	int doVerb(int);
};

/* insets */
class CalendarInset57100 : public Inset
{
public:
	CalendarInset57100();
	int doVerb(int);
};

class GunLange57100 : public View
{
public:
	GunLange57100();
	void respond(void);
	int doVerb(int);
};

/* dialogs */
class LangeShootThorn : public DItem
{
public:
	LangeShootThorn();
	void doit(void);
};

class ThornShootLange : public DItem
{
public:
	ThornShootLange();
	void doit(void);
};

/* scripts */
class LangeEnter57100 : public Script
{
public:
	LangeEnter57100();
	void changeState(int newState);
};

} // namespace Spycraft

#endif
