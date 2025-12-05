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

#ifndef SPYCRAFT_GAME_GUN_H
#define SPYCRAFT_GAME_GUN_H

#include "spycraft/game/actor.h"
#include "spycraft/game/list.h"
#include "spycraft/game/script.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/cursor.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

extern GunCursor *gunCursor;

class Clip : public View
{
public:
	char *name;
	Clip();
	void dispose(void);
};

class Mags : public View
{
public:
	char *name;
	Mags();
	void dispose(void);
};

class Gun : public View
{
public:
	char *name;
	Clip *clip;
	Mags *mags;
	int shots;
	int clips;
	int mt;
	Gun();
	~Gun();
	void doit();
	void dispose(void);
	int handleEvent(MADEEventStamp *event);
};
class GunScript : public Script
{
public:
	void changeState(int);
};
class ReloadScript : public Script
{
public:
	void changeState(int);
};

extern Gun *theGun;

} // namespace Spycraft

#endif
