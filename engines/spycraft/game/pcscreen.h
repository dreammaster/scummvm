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

#ifndef SPYCRAFT_GAME_PCSCREEN_H
#define SPYCRAFT_GAME_PCSCREEN_H

#include "spycraft/game/inset.h"
#include "spycraft/game/list.h"
#include "spycraft/game/script.h"

namespace Spycraft {

// for pcLastUsed
enum {
	PC_INTRLINK, PC_KAT, PC_IA, PC_INFOSCI, PC_SNDANAL, PC_CIPHER,
	PC_PHOTODOC, PC_PEGID, PC_ECLIPSE, PC_EBMC, PC_MAXUSEDTYPE
};

enum {
	PC_INTRLINKBUTTON, PC_IABUTTON, PC_INFOSCIBUTTON, PC_KATBUTTON, PC_SOUNDANALBUTTON,
	PC_CIPHERBUTTON, PC_PHOTODOCBUTTON, PC_PEGIDBUTTON, PC_ECLIPSEBUTTON,
	PC_EBMCBUTTON, PC_MAXBUTTONS
};

class PCScreen : public Inset
{
public:
	PCScreen(int roomNum);
	PCScreen(int roomNum, Object *whoToCue);
	void init(void);
	void dispose(void);
	void respond(void);
	void addTheButtons(void);
	void drawButtons(void);
	void addButton(int button);
	void cue(void);	// for updates
	int	 roomNumber;
	List buttons;
};

/* inset features */
class PCButtonInsetFeature : public InsetFeature
{
public:
	PCButtonInsetFeature();
	void respond(void);
	int doVerb(int);
	int	buttType;	// one of enum
};

/* inset views */
class PCButtonBlinker; // fwd decl

class PCButtonView : public View
{
public:
	PCButtonView(int theType, int atsNum);
	~PCButtonView();
	int	blinkOn(void);
	int	atsNumber;
	int	buttType;	// one of enum
	PCButtonInsetFeature *myFeature;
	PCButtonBlinker *blinker; // for blinking button
};

/* scripts */
class PCDelayedCallBack : public Script
{
public:
	PCDelayedCallBack(Object *whoToCue);
	void changeState(int);
};

class PCButtonPressScript : public Script
{
public:
	PCButtonPressScript();
	void changeState(int);
};

class PCScreenUpdateTimer : public Script
{
public:
	PCScreenUpdateTimer(Object *);
	void changeState(int);
	void doit(void);
	List	blinkers;
};

class PCButtonBlinker : public Script
{
public:
	PCButtonBlinker(PCButtonView *);
	void stop(void);
	void kill(void);
	void changeState(int);
	int		bright;	// flag
	PCButtonView *myButton;
};

class PCDistantButtonScript : public Script
{
public:
	PCDistantButtonScript() {
		name = "PCDistantButtonScript";
	};
	void changeState(int);
};

class PlayKATScriptPC : public Script
{
public:
	PlayKATScriptPC() {
		name = "PlayKATScriptPC";
	};
	void changeState(int);
};

/* PC Distant blinker */
class PCDistantButton : public Prop
{
public:
	PCDistantButton(int button, int atsnum, int x, int y);
	void cue(void);
	int blinkOn(void);
	int	buttonType;		// on of enum
};

extern short  pcLastUsed[];	// array--Puzzles set their last used status here.  
// One of enum in intrpuzz.h (zero is none).
// See PC_ enums above for index of array.

} // namespace Spycraft

#endif
