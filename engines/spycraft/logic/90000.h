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

#ifndef SPYCRAFT_LOGIC_90000_H
#define SPYCRAFT_LOGIC_90000_H

#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"
#include "room1.h"

namespace Spycraft {

class Rm90000 : public Room
{
public:
	Rm90000();
	~Rm90000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm90001 : public Room
{
public:
	Rm90001();
	~Rm90001();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm90002 : public Room	  	// download room
{
public:
	Rm90002();
	~Rm90002();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm90003 : public Room		// Static room
{
public:
	Rm90003();
	~Rm90003();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm90004 : public Room		// Blow up room
{
public:
	Rm90004();
	~Rm90004();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit90000 : public SouthExit
{
public:
	SouthExit90000();
	void respond(void);
	int doVerb(int);
};

class SouthExit90001 : public SouthExit
{
public:
	SouthExit90001();
	void respond(void);
	int doVerb(int);
};

class WestExit90001 : public ExitFeature  //Being used as heli door
{
public:
	WestExit90001();
	void respond(void);
	int doVerb(int);
};

/************/
/* features */
/************/

class OkFeature : public Feature
{
public:
	OkFeature();
	void respond(void);
	int doVerb(int);
};

class CancelFeature : public Feature
{
public:
	CancelFeature();
	void respond(void);
	int doVerb(int);
};

class StopFeature : public Feature
{
public:
	StopFeature();
	void respond(void);
	int doVerb(int);
};

class Window90001 : public Feature
{
public:
	Window90001();
	void respond(void);
	int doVerb(int);
};

class Backpack90001 : public Feature
{
public:
	Backpack90001();
	void respond(void);
	int doVerb(int);
};
/***********/
/* scripts */
/***********/
class HeliBlowupScript : public Script
{
public:
	HeliBlowupScript() {
		name = "HeliBlowupScript";
	};
	void changeState(int);
};

class goTo91000 : public Script
{
public:
	goTo91000() {
		name = "goToScript";
	};
	void changeState(int);
};

class HeliTransferScript : public Script
{
public:
	Sound *pilotVo2;
	HeliTransferScript() {
		name = "HeliTransferScript"; pilotVo2 = new Sound;
	};
	void changeState(int);
};

class HeliTimerStart : public Script
{
public:
	Sound *pilotVo1;
	HeliTimerStart() {
		name = "HeliTimerStart"; pilotVo1 = new Sound;
	};
	void changeState(int);
};

class ExplodeSound : public Script
{
public:
	Sound *explode;
	Sound *pilotVo3;
	ExplodeSound() {
		name = "ExplodeSound"; explode = new Sound; pilotVo3 = new Sound;
	};
	void changeState(int);
};

class CountDownScript90000 : public Script
{
public:
	CountDownScript90000();
	void changeState(int);
};

class PowerBookSnd : public Script
{
public:
	Sound *snd1;
	PowerBookSnd();
	void changeState(int);
};

class StatusBarStart : public Script
{
public:
	StatusBarStart() {
		name = "StatusBarStart";
	};
	void changeState(int);
};

class OkButtonClick : public Script
{
public:
	Sound *oksound;
	OkButtonClick() {
		name = "OkButtonClick"; oksound = new Sound;
	};
	void changeState(int);
};

class CancelButtonClick : public Script
{
public:
	CancelButtonClick() {
		name = "CancelButtonScript";
	};
	void changeState(int);
};

class StopButtonClick : public Script
{
public:
	Sound *snd5;
	StopButtonClick() {
		name = "StopButtonClick"; snd5 = new Sound;
	};
	void changeState(int);
};

class BackpackSound : public Script
{
public:
	Sound *snd6;
	BackpackSound() {
		name = "BackpackSound"; snd6 = new Sound;
	};
	void changeState(int);
};

/*********/
/* views */
/*********/

class ClosedPowerBook : public View
{
public:
	ClosedPowerBook();
	void respond(void);
	int doVerb(int);
};

class EmptyBackpack : public View
{
public:
	EmptyBackpack();
};

class PowerBook90001 : public View
{
public:
	PowerBook90001();
	void respond(void);
	int doVerb(int);
};

class HeliDebug : public View
{
public:
	HeliDebug();
	void respond(void);
	int doVerb(int);
};

class PowerBookInv : public View
{
public:
	PowerBookInv();
};

class StatusBar : public View
{
public:
	StatusBar();
};

class OkButton : public View
{
public:
	OkButton();
};

class CancelButton : public View
{
public:
	CancelButton();
};

class DownloadMenu : public View
{
public:
	DownloadMenu();
};

class StartScreen : public View
{
public:
	StartScreen();
};

class PercentBar : public View
{
public:
	PercentBar();
};

class StopButton : public View
{
public:
	StopButton();
};

/* cursor */

class PDACursor : public MadeCursor
{
public:
	PDACursor();
};

class PBookCursor : public MadeCursor
{
public:
	PBookCursor();
};

/* props */

class DownloadGlobe : public Prop
{
public:
	DownloadGlobe();
	void cue(void);
};

} // namespace Spycraft

#endif

