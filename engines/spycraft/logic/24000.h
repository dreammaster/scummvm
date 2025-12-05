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

#ifndef SPYCRAFT_LOGIC_24000_H
#define SPYCRAFT_LOGIC_24000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

/* rooms */
class Rm24000 : public Room
{
public:
	Rm24000();
	void init(void);
};

class Rm24100 : public Room
{
public:
	Rm24100();
	void init();
};

/* exit features */
class SouthExit24000 : public ExitFeature
{
public:
	SouthExit24000();
	void respond(void);
	int doVerb(int);
};

class SouthExit24100 : public ExitFeature
{
public:
	SouthExit24100();
	void respond(void);
	int doVerb(int);
};

/* features */
class Desk24000 : public Feature
{
public:
	Desk24000();
	void respond(void);
	int doVerb(int);
};

class Phone24100 : public Feature
{
public:
	Phone24100();
	void respond(void);
	int doVerb(int);
	void cue(void);
};

class Rolodex24100 : public Feature
{
public:
	Rolodex24100();
	void respond(void);
	int doVerb(int);
};

class Computer24100 : public Feature
{
public:
	Computer24100();
	void respond(void);
	int doVerb(int);
};

class WallChart24100 : public Feature
{
public:
	WallChart24100();
	void respond(void);
	int doVerb(int);
};

class WallBox24100 : public Feature
{
public:
	WallBox24100();
	void respond(void);
	int doVerb(int);
};

/* views */
class DeskSprite24000 : public View
{
public:
	DeskSprite24000(int v_num, int x, int y);
};

class YingPhotoView24100 : public View
{
public:
	YingPhotoView24100();
	void respond(void);
	int doVerb(int);
};

class YingStickyNoteView24100 : public View
{
public:
	YingStickyNoteView24100();
	void respond(void);
	int doVerb(int);
};

class LockPickView24100 : public View
{
public:
	LockPickView24100();
	void respond(void);
	int doVerb(int);
};

class BeowulfView24100 : public View
{
public:
	BeowulfView24100();
	void respond(void);
	int doVerb(int);
};

class InternetGuideView24100 : public View
{
public:
	InternetGuideView24100();
	void respond(void);
	int doVerb(int);
};

class YingEBMCatView24100 : public View
{
public:
	YingEBMCatView24100();
	void respond(void);
	int doVerb(int);
};

class ShanghaiView24100 : public View
{
public:
	ShanghaiView24100();
	void respond(void);
	int doVerb(int);
};

/* insets */
class YingPhotoInset24100 : public Inset
{
public:
	YingPhotoInset24100();
};

class YingStickyNoteInset24100 : public Inset
{
public:
	YingStickyNoteInset24100();
};

class LockPickInset24100 : public Inset
{
public:
	LockPickInset24100();
	void respond(void);
	int doVerb(int);
};

class LockPickOpenInset24100 : public Inset
{
public:
	LockPickOpenInset24100();
	void respond(void);
	int doVerb(int);
};

class InternetGuideInset24100 : public Inset
{
public:
	InternetGuideInset24100();
};

class YingEBMCatInset24100 : public Inset
{
public:
	YingEBMCatInset24100();
};

class BeowulfInset24100 : public Inset
{
public:
	BeowulfInset24100();
};

class ShanghaiInset24100 : public Inset
{
public:
	ShanghaiInset24100();
};

class PhoneInset24100 : public Inset
{
public:
	PhoneInset24100();
	int doVerb(int);
};

class RolodexInset24100 : public Inset
{
public:
	RolodexInset24100();
	int doVerb(int);
};

class WallChartInset24100 : public Inset
{
public:
	WallChartInset24100();
};

/* props */
class ScreenSaverProp24000 : public Prop
{
public:
	ScreenSaverProp24000();
	void cue(void);
};

class ScreenSaverProp24100 : public Prop
{
public:
	ScreenSaverProp24100();
	void cue(void);
};

/* scripts */

class ZipScript24100 : public Script
{
public:
	ZipScript24100() {
		name = "ZipScript24100";
	};
	void changeState(int);
};

class FolderSndScript24100 : public Script
{
public:
	FolderSndScript24100() {
		name = "FolderSndScript24100";
	};
	void changeState(int);
};

//class PhoneRingScript24100 : public Script
//{
// public:
// 	PhoneRingScript24100() { name = "PhoneRingScript24100"; };
//	void changeState( int );
//};

class JaimieBlamScript24000 : public Script
{
public:
	JaimieBlamScript24000() {
		name = "JaimieBlamScript24000";
	};
	void changeState(int);
};

class EBMCatScript24100 : public Script
{
public:
	EBMCatScript24100() {
		name = "EBMCatScript24100";
	};
	void changeState(int);
};

class YingBealeScript24100 : public Script
{
public:
	YingBealeScript24100() {
		name = "YingBealeScript24100";
	};
	void changeState(int);
};

class launchShanghai : public Script
{
public:
	launchShanghai() {
		name = "launchShanghai";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif

