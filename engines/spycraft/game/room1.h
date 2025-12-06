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

#ifndef SPYCRAFT_GAME_ROOM1_H
#define SPYCRAFT_GAME_ROOM1_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/dialog.h"

namespace Spycraft {

/*class LogoRoom : public Room
{
public:
	LogoRoom();
	int handleEvent ( MADEEventStamp * );

	void init ( void );
};

class openingScript : public Script
{
public:
	openingScript() {name = "openingScript";};
	void changeState ( int );
};*/

class Room1 : public Room
{
public:
	Room1();
	~Room1();

	void init(void);
	int handleEvent(MADEEventStamp *);
};




class DItem11 : public DItem
{
public:
	//DItem11();      //make this inline
	void doit();
};

class DItem12 : public DItem
{
public:
	// DItem12();
	void doit();
};

class DItem13 : public DItem
{
public:
	//DItem13();
	void doit();
};

class DItem14 : public DItem
{
public:
	//DItem13();
	void doit();
};

class DItem15 : public DItem
{
public:
	//DItem13();
	void doit();
};

class DItem16 : public DItem
{
public:
	//DItem13();
	void doit();
};

class DItem17 : public DItem
{
public:
	//DItem13();
	void doit();
};

class DItem18 : public DItem
{
public:
	//DItem13();
	void doit();
};

class DItem19 : public DItem
{
public:
	//DItem13();
	void doit();
};

class DItem20 : public DItem
{
public:
	//DItem13();
	void doit();
};

} // namespace Spycraft

#endif

