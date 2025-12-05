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

#ifndef SPYCRAFT_LOGIC_1010_H
#define SPYCRAFT_LOGIC_1010_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

namespace Spycraft {

class Rm1010 : public Room
{
public:
	Rm1010();
	void init(void);
};

/* features */
class Building1010 : public Feature
{
public:
	Building1010();
	void respond(void);
	int doVerb(int);
};

/* scripts */
class EstabScript1010 : public Script
{
public:
	EstabScript1010() {
		name = "EstabScript1010";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif

