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

#ifndef SPYCRAFT_LOGIC_1090_H
#define SPYCRAFT_LOGIC_1090_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

namespace Spycraft {

class Rm1090 : public Room
{
public:
	Rm1090();
	void init(void);
};

class Rm1091 : public Room
{
public:
	Rm1091();
	void init(void);
};

class NorthExit1090 : public ExitFeature
{
public:
	NorthExit1090();
	void respond(void);
	int doVerb(int);
};

class SouthExit1090 : public ExitFeature
{
public:
	SouthExit1090();
	void respond(void);
	int doVerb(int);
};

class EastExit1090 : public ExitFeature
{
public:
	EastExit1090();
	void respond(void);
	int doVerb(int);
};

class SouthExit1091 : public ExitFeature
{
public:
	SouthExit1091();
	void respond(void);
	int doVerb(int);
};

class EastExit1091 : public ExitFeature
{
public:
	EastExit1091();
	void respond(void);
	int doVerb(int);
};

/* features */
class DoorBell_1091 : public Feature
{
public:
	DoorBell_1091();
	void respond(void);
	int doVerb(int);
};

/* scripts */
//class ExitScript1090 : public Script
//{
// public:
// 	ExitScript1090() { name = "ExitScript1090"; };
//	void changeState( int );
//};

class DoorbellScript1091 : public Script
{
public:
	DoorbellScript1091() {
		name = "DoorbellScript1091";
	};
	void changeState(int);
};

class HelloScript1091 : public Script
{
public:
	HelloScript1091() {
		name = "HelloScript1091";
	};
	void changeState(int);
};

class ISaidHelloScript1091 : public Script
{
public:
	ISaidHelloScript1091() {
		name = "ISaidHelloScript1091";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif

