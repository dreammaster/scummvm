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

#ifndef SPYCRAFT_LOGIC_32000_H
#define SPYCRAFT_LOGIC_32000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/timer.h"
#include "spycraft/game/plistobj.h"

namespace Spycraft {

class Rm32000 : public Room
{
public:
	Rm32000();
	~Rm32000();
	void init(void);
	void cue(void);
	int handleEvent(MADEEventStamp *);
};

class NorthExit32000 : public ExitFeature
{
public:
	NorthExit32000();
	void cue(void);
	void respond(void);
	int doVerb(int);
};

class SouthExit32000 : public ExitFeature
{
public:
	SouthExit32000();
	void respond(void);
	int doVerb(int);
};

/* script */
class EnterScript32001 : public Script
{
public:
	EnterScript32001();
	void changeState(int);
};

class EnterScript32002 : public Script
{
public:
	EnterScript32002();
	void changeState(int);
};

class EnterScript32003 : public Script
{
public:
	EnterScript32003();
	void changeState(int);
};

class BackgroundScript32000 : public Script
{
public:
	BackgroundScript32000();
	void changeState(int);
};

class CloseBackgroundScript320000 : public Script
{
public:
	CloseBackgroundScript320000();
	void changeState(int);
};

/* dialog */
class HarmDialog : public Dialog
{
public:
	//	Timer* harmTimer32000;

	HarmDialog();
	void show(void);
	void hide(void);
	void cue(void);
};

class TurnProcat : public DItem
{
public:
	TurnProcat();
	void doit(void);
};

class NoProcat : public DItem
{
public:
	NoProcat();
	void doit(void);
};

class SaveYou : public DItem
{
public:
	SaveYou();
	void doit(void);
};

class HarmKillPlayer : public PermListObject
{
public:
	// 	int	playingVoice;
	HarmKillPlayer();
	void dispose(void);
};

class HarmDItem : public DItem
{
public:
	void doit(void);
};

} // namespace Spycraft

#endif

