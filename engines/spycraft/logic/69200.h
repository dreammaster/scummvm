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

#ifndef SPYCRAFT_LOGIC_69200_H
#define SPYCRAFT_LOGIC_69200_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

namespace Spycraft {

class Rm69200 : public Room
{
public:
	Rm69200();
	//~Rm69200();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class WestExit69200 : public ExitFeature
{
public:
	WestExit69200();
	void respond(void);
	int doVerb(int);
};

/* features */
class Doll69200 : public Feature
{
public:
	Doll69200();
	void respond(void);
	int doVerb(int);
};

class Card69200 : public Feature
{
public:
	Card69200();
	void respond(void);
	int doVerb(int);
};


/* insets */
class DollInset69200 : public Inset
{
public:
	DollInset69200();
	//	void respond();
};

class CardInset69200 : public Inset
{
public:
	CardInset69200();
	//	void respond();
};

} // namespace Spycraft

#endif

