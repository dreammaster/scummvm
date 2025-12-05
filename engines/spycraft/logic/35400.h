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

#ifndef SPYCRAFT_LOGIC_35400_H
#define SPYCRAFT_LOGIC_35400_H

#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm35400 : public Room
{
public:
	Rm35400();
	~Rm35400();
	void init(void);
	void doit(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit35400 : public SouthExit
{
public:
	SouthExit35400();
	void respond(void);
	int doVerb(int);
};

class EastExit35400 : public SouthExit
{
public:
	EastExit35400();
	void respond(void);
	int doVerb(int);
};

/* features */
class ClosedFile35400 : public Feature
{
public:
	ClosedFile35400();
	void respond(void);
	int doVerb(int);
};

class OpenFileScript35400 : public Script
{
public:
	OpenFileScript35400();
	void changeState(int);
};

class Rm35500 : public Room
{
public:
	Rm35500();
	~Rm35500();
	void init(void);
	void doit(void);
	int handleEvent(MADEEventStamp *);
};
/*
class SouthExit35500 : public SouthExit
{
public:
	SouthExit35500();
	void respond ( void );
	int doVerb ( int );
};
*/
class FileHandle35500 : public Feature
{
public:
	FileHandle35500();
	void respond(void);
	int doVerb(int);
};

class Hacker35500 : public Feature
{
public:
	Hacker35500();
	void respond(void);
	int doVerb(int);
};

class HackerInset35520 : public Inset
{
public:
	HackerInset35520();
	//	void respond();
};

class Folder35500 : public Feature
{
public:
	Folder35500();
	void respond(void);
	int doVerb(int);
};

class Rm35600 : public Room
{
public:
	Rm35600();
	~Rm35600();
	void init(void);
	void doit(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit35600 : public SouthExit
{
public:
	SouthExit35600();
	void respond(void);
	int doVerb(int);
};

class LeftPage35600 : public Feature
{
public:
	LeftPage35600();
	void respond(void);
	int doVerb(int);
};

class RightPage35600 : public Feature
{
public:
	RightPage35600();
	void respond(void);
	int doVerb(int);
};

class BlankPage35600 : public View
{
public:
	BlankPage35600();
	void respond(void);
	int doVerb(int);
};

class PageOne35600 : public View
{
public:
	PageOne35600();
	void respond(void);
	int doVerb(int);
};

class PageTwo35600 : public View
{
public:
	PageTwo35600();
	void respond(void);
	int doVerb(int);
};

class PageThree35600 : public View
{
public:
	PageThree35600();
	void respond(void);
	int doVerb(int);
};

class PageFour35600 : public View
{
public:
	PageFour35600();
	void respond(void);
	int doVerb(int);
};

class PageFive35600 : public View
{
public:
	PageFive35600();
	void respond(void);
	int doVerb(int);
};

class PageSix35600 : public View
{
public:
	PageSix35600();
	void respond(void);
	int doVerb(int);
};

class TurnPageScript35600 : public Script
{
public:
	TurnPageScript35600();
	void changeState(int);
};

class Rm35701 : public Room
{
public:
	Rm35701();
	~Rm35701();
	void init(void);
	void doit(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit35701 : public SouthExit
{
public:
	SouthExit35701();
	void respond(void);
	int doVerb(int);
};

class GogDisk35701 : public View
{
public:
	GogDisk35701();
	void respond(void);
	int doVerb(int);
};

class GogDiskInset35701 : public Inset
{
public:
	GogDiskInset35701();
	//	void respond();
};

} // namespace Spycraft

#endif

