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

#ifndef SPYCRAFT_GAME_PHONE_H
#define SPYCRAFT_GAME_PHONE_H

#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define FROM_RUSSIA 0
#define FROM_USA 1

class PhoneKeyPad : public Room
{
public:
	PhoneKeyPad(int location);
	~PhoneKeyPad();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExitPhone : public SouthExit
{
public:
	SouthExitPhone();
	void respond(void);
	int doVerb(int);
};

/******************************
 *
 *	Button Features
 *
 ******************************/
class NumberKeyFeature : public Feature
{
public:
	int keynum;
	NumberKeyFeature(int Mynum);
	void respond(void);
	int doVerb(int);
};

class MessageButton : public Feature
{
public:
	MessageButton();
	void respond(void);
	int doVerb(int);
};

class HangUpButton : public Feature
{
public:
	HangUpButton();
	void respond(void);
	int doVerb(int);
};

class LineButton : public Feature
{
public:
	LineButton();
	void respond(void);
	int doVerb(int);
};

/********************
 *
 *  Views
 *
 ********************/
class LinePress : public View
{
public:
	LinePress();
};

class MessagePress : public View
{
public:
	MessagePress();
};

class MessageBlink : public Prop
{
public:
	MessageBlink();
	void cue(void);
};

class HangPress : public View
{
public:
	HangPress();
};

class KeyDisplay : public View
{
public:
	KeyDisplay(int place);
};

class KeyPress : public View
{
public:
	KeyPress(int number);
};

/********************
 *
 *  Scripts
 *
 ********************/
class LineScript : public Script
{
public:
	LineScript() {
		name = "LineScript";
	};
	void changeState(int);
};

class IncomingCallScript : public Script
{
public:
	Sound *callSound;
	IncomingCallScript() {
		name = "IncomingCallScript"; callSound = new Sound;
	};
	void changeState(int);
};

class MessageScript : public Script
{
public:
	Sound *phoneSound;
	MessageScript(int mystate) {
		name = "MessageScript";
		phoneSound = new Sound;
		state = mystate;
	};
	void changeState(int);
};

class HangButtonScript : public Script
{
public:
	Sound *hangSound;
	HangButtonScript() {
		name = "HangButtonScript"; hangSound = new Sound;
	};
	void changeState(int);
};

class DialingProcedure : public Script
{
public:
	Sound *phoneSound;
	DialingProcedure() {
		name = "DialingProcedure"; phoneSound = new Sound;
	};
	void changeState(int);
};

class KeyPressScript : public Script
{
public:
	Sound *tone;
	int keynum;
	KeyPressScript(int number);
	void changeState(int);
};

class BankService : public Script
{
public:
	Sound *BankVoice;
	BankService(int myState) {
		name = "BankService";
		BankVoice = new Sound;
		state = myState;
	};
	void changeState(int);
};

} // namespace Spycraft

#endif
