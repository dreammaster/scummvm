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

#ifndef SPYCRAFT_LOGIC_1070_H
#define SPYCRAFT_LOGIC_1070_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"

namespace Spycraft {

class Rm1070 : public Room
{
public:
	Rm1070();
	~Rm1070();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class EnterScript1070 : public Script
{
public:
	EnterScript1070();
	void changeState(int);
};


/*class SouthExit1070 : public ExitFeature
{
public:
	SouthExit1070();
	void respond ( void );
	int doVerb ( int );
};

class Tundra3D1070 : public TextFeature
{
public:
	Tundra3D1070();
	int doVerb ( int );
};

class JumpJet1070 : public TextFeature
{
public:
	JumpJet1070();
	int doVerb ( int );
};*/

} // namespace Spycraft

#endif

