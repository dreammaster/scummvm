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

#ifndef SPYCRAFT_LOGIC_48000_H
#define SPYCRAFT_LOGIC_48000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
//#include "spycraft/logic/47000.h"
#include "inset.h"
#include "flag.h"

namespace Spycraft {

class Timer;

class Rm48000 : public Room
{
public:

	Timer *myTimer;
	Rm48000();
	~Rm48000();
	void init(void);
	void cue(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit48000 : public SouthExit
{
public:
	SouthExit48000();
	void respond(void);
	int doVerb(int);
};

class Photo48000 : public Feature
{
public:
	Photo48000();
	void respond(void);
	int doVerb(int);
};

class PhotoHead48000 : public Feature
{
public:
	PhotoHead48000();
	void respond(void);
	int doVerb(int);
};

class Booze48000 : public Feature
{
public:
	Booze48000();
	void respond(void);
	int doVerb(int);
};

class Shelf48000 : public Feature
{
public:
	Shelf48000();
	void respond(void);
	int doVerb(int);
};

class Desk48000 : public Feature
{
public:
	Desk48000();
	int doVerb(int);
};

class GiftBox48000 : public View
{
public:
	GiftBox48000();
	void respond(void);
	int doVerb(int);
};

class PhotoInset48000 : public Inset
{
public:
	PhotoInset48000();
	//	void respond();
	//	int doVerb( int);
};

class PhotoHeadInset48000 : public Inset
{
public:
	PhotoHeadInset48000();
	//	void respond();
	//	int doVerb( int);
};

class BoozeInset48000 : public Inset
{
public:
	BoozeInset48000();
	//	void respond();
	//	int doVerb( int);
};

/* scripts */
class EnterScriptIntro48000 : public Script
{
public:
	EnterScriptIntro48000();
	void changeState(int);
};

class EnterScriptHarmonica48000 : public Script
{
public:
	EnterScriptHarmonica48000();
	void changeState(int);
};

class EnterScriptOnyx48000 : public Script
{
public:
	EnterScriptOnyx48000();
	void changeState(int);
};
/*
class GiftScript : public Script
{
public:
	GiftScript() {name = "GiftScript";};
	void changeState ( int );
};
*/

/* dialog */
class Dialog2_48000 : public Dialog
{
	void cue(void);
};

class PhotoMaxSend : public DItem
{
public:
	PhotoMaxSend();
	void doit(void);
	virtual void cue(void);
};

class MafiaInfo : public DItem
{
public:
	MafiaInfo();
	void doit(void);
};

class ProblemsInfo : public DItem
{
public:
	ProblemsInfo();
	void doit(void);
};

/* Room48200 */
class Rm48200 : public Room
{
public:
	Rm48200();
	void init(void);
};

class SouthExit48200 : public ExitFeature
{
public:
	SouthExit48200();
	void respond(void);
	int doVerb(int);
};

class BallInset48200 : public Inset
{
public:
	BallInset48200();
	//	int doVerb ( int );
};

class Softball48200 : public Feature
{
public:
	Softball48200();
	int doVerb(int);
};

/* Room 48300 */
class Rm48300 : public Room
{
public:
	Rm48300();
	void init(void);
};

class SouthExit48300 : public ExitFeature
{
public:
	SouthExit48300();
	void respond(void);
	int doVerb(int);
};

class Box48300 : public Feature
{
public:
	Box48300();
	void respond(void);
	int doVerb(int);
};

class HarmInset48300 : public Inset
{
public:
	HarmInset48300();
	//	void respond();
	int doVerb(int);
};

class EnterScript48300 : public Script
{
public:
	EnterScript48300();
	void changeState(int);
};

} // namespace Spycraft

#endif

