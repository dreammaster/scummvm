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

#ifndef SPYCRAFT_LOGIC_36100_H
#define SPYCRAFT_LOGIC_36100_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"

namespace Spycraft {

class Rm36100 : public Room
{
public:
	Rm36100();
	void init();
};

/* exit features */
class SouthExit36100 : public SouthExit
{
public:
	SouthExit36100();
	void respond(void);
	int doVerb(int);
};

/* features */
class Desk36100 : public Feature
{
public:
	Desk36100();
	void respond(void);
	int doVerb(int);
};

/* text features */
/*
class Photo36100 : public TextFeature
{
public:
	Photo36100();
	void respond( void );
	int doVerb( int );
};
*/

class Computer36100 : public Feature
{
public:
	Computer36100();
	void respond(void);
	int doVerb(int);
};

/* insets */
class ComputerInset36100 : public Inset
{
public:
	ComputerInset36100();
	//	void respond();
};

/* scripts */
class EnterScript36100 : public Script
{
public:
	EnterScript36100();
	void changeState(int);
};

} // namespace Spycraft

#endif
