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

#ifndef SPYCRAFT_GAME_USER_H
#define SPYCRAFT_GAME_USER_H

#include "spycraft/game/made.h"
#include "spycraft/game/object.h"

namespace Spycraft {

// VIRTUAL KEY DEFINES
#define VKEY_ENTER			13
#define VKEY_ESC				27
#define VKEY_UP				38
#define VKEY_DOWN				40
#define VKEY_LEFT				37
#define VKEY_RIGHT			39

#define VKEY_DEBUG			192

// EVENT DEFINES
#define USER_RIGHT_UP		0x0001
#define USER_RIGHT_DOWN		0x0002
#define USER_LEFT_UP		0x0004
#define USER_LEFT_DOWN		0x0008
#define USER_MID_UP			0x0010
#define USER_MID_DOWN		0x0020
#define USER_KEY_UP			0x0040
#define USER_KEY_DOWN		0x0080
#define USER_LEFT_DBL		0x0100

#define USER_MOUSE_UP		0x0015
#define USER_MOUSE_DOWN		0x002a

class User : public Object
{
public:
	int message;
	int input;

	User();
	~User();
	int handleEvent(MADEEventStamp *event);
	void canInput(int tOrF);
};

extern User *user;
extern int event_type;
extern int mouse_x;
extern int mouse_y;
extern unsigned long gameTime;
extern MADEEventStamp theEvent;

} // namespace Spycraft

#endif
