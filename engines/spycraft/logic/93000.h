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

#ifndef SPYCRAFT_LOGIC_93000_H
#define SPYCRAFT_LOGIC_93000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"
#include "sound.h"

namespace Spycraft {

class Rm93000 : public Room
{
public:
	Rm93000();
	~Rm93000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm93500 : public Room
{
public:
	Rm93500();
	~Rm93500();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm93010 : public Room
{
public:
	Rm93010();
	~Rm93010();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class Rm93020 : public Room
{
public:
	Rm93020();
	~Rm93020();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

/**************
 *  features  *
 **************/
class Backpack : public Feature
{
public:
	Backpack();
	void respond(void);
	int doVerb(int);
};

class BackpackOn : public Feature
{
public:
	BackpackOn();
	void respond(void);
	int doVerb(int);
};

class BackpackOff : public Feature
{
public:
	BackpackOff();
	void respond(void);
	int doVerb(int);
};

/*************
 *  scripts  *
 *************/

class Script93000 : public Script
{
public:
	Sound *snd1;
	Script93000() {
		name = "Script93000"; snd1 = new (Sound);
	};
	void changeState(int);
};

class Script93001 : public Script
{
public:
	Script93001() {
		name = "Script93001";
	};
	void changeState(int);
};

class BlakeCaughtScript : public Script
{
public:
	BlakeCaughtScript() {
		name = "BlakeCaughtScript";
	};
	void changeState(int);
};

class DropGunScript : public Script
{
public:
	DropGunScript() {
		name = "DropGunScript";
	};
	void changeState(int);
};

class BlakeDeathScript : public Script
{
public:
	BlakeDeathScript() {
		name = "BlakeDeathScript";
	};
	void changeState(int);
};

class ToHeliTransfer : public Script
{
public:
	ToHeliTransfer() {
		name = "ToHeliTransfer";
	};
	void changeState(int);
};

// Grendal appears
class Script93500 : public Script
{
public:
	Script93500() {
		name = "Script93500";
	};
	void changeState(int);
};

class ShootGrendal : public Script
{
public:
	ShootGrendal() {
		name = "ShootGrendal";
	};
	void changeState(int);
};

/*************
 *   view    *
 *************/

class DropGunIcon : public View
{
public:
	DropGunIcon();
	void respond(void);
	int doVerb(int);
};

class DropGogglesIcon : public View
{
public:
	DropGogglesIcon();
	void respond(void);
	int doVerb(int);
};

class GetGunIcon : public View
{
public:
	GetGunIcon();
	void respond(void);
	int doVerb(int);
};

class ShootBlakeIcon : public View
{
public:
	int person;		   // grendal = 0 : blake = 1
	ShootBlakeIcon(int who);
	void respond(void);
	int doVerb(int);
};

} // namespace Spycraft

#endif
