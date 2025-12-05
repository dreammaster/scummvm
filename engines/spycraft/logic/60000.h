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

#ifndef SPYCRAFT_LOGIC_60000_H
#define SPYCRAFT_LOGIC_60000_H

#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

enum {
	END_NONE, END_JAIL, END_DEAD, END_BLACKDEAD, END_FIRED, END_GAME
};// for endType global

class Rm60000 : public Room
{
public:
	Rm60000();	// shouldn't use this one
	Rm60000(int endType);
	~Rm60000();
	void init(void);
	int	theEnd;	// one of enum
};

class Rm60500 : public Room
{
public:
	Rm60500();
	~Rm60500();
	void init(void);
};

/* views */
class ButtonView60000 : public View
{
public:
	ButtonView60000();
	//int pressed;
	int handleEvent(MADEEventStamp *);
	virtual void activate(void) {
	};
};
// 60000
class RestoreView60000 : public ButtonView60000
{
public:
	RestoreView60000();
	void respond(void);
	//int doVerb( int );
	void activate(void);
};

class QuitView60000 : public ButtonView60000
{
public:
	QuitView60000();
	void respond(void);
	//int doVerb( int );
	void activate(void);
};

// 60500
class PlayBt60500 : public ButtonView60000
{
public:
	PlayBt60500();
	void respond(void);
	//int doVerb( int );
	void activate(void);
};

class RestoreBt60500 : public ButtonView60000
{
public:
	RestoreBt60500();
	void respond(void);
	//int doVerb( int );
	void activate(void);
};

class QuitBt60500 : public ButtonView60000
{
public:
	QuitBt60500();
	void respond(void);
	//int doVerb( int );
	void activate(void);
};

/* scripts */
class JailScript60000 : public Script
{
public:
	JailScript60000() {
		name = "JailScript60000";
	};
	void changeState(int);
};

class FiredScript60000 : public Script
{
public:
	FiredScript60000() {
		name = "FiredScript60000";
	};
	void changeState(int);
};

class DeadScript60000 : public Script
{
public:
	DeadScript60000() {
		name = "DeadScript60000";
	};
	void changeState(int);
};

class EndGameScript60000 : public Script
{
public:
	EndGameScript60000() {
		name = "EndGameScript60000";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif

