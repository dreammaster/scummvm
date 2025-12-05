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

#ifndef SPYCRAFT_LOGIC_36700_H
#define SPYCRAFT_LOGIC_36700_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

namespace Spycraft {

class Rm36700 : public Room
{
public:
	Rm36700();
	~Rm36700();
	void cue();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

/* scripts */
class EnterScript36700 : public Script
{
public:
	EnterScript36700();
	void changeState(int);
};

/* dialog */
class DowntoBullpen : public DItem
{
public:
	DowntoBullpen();
	void doit(void);
};

class DowntoInterrogation : public DItem
{
public:
	DowntoInterrogation();
	void doit(void);
};

} // namespace Spycraft

#endif			
