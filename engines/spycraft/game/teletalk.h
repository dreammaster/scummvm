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

#ifndef SPYCRAFT_GAME_TELETALK_H
#define SPYCRAFT_GAME_TELETALK_H

#include "spycraft/game/dialog.h"
#include "spycraft/game/script.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/plistobj.h"

namespace Spycraft {

/* scripts */
class BlakeTeletalkScript : public Script
{
public:
	BlakeTeletalkScript() {
		name = "BlakeTeletalkScript";
	};
	void changeState(int);
};

class BlakeTeletalkHangUpScript : public Script
{
public:
	BlakeTeletalkHangUpScript() {
		name = "BlakeTeletalkHangUpScript";
	};
	void changeState(int);
};

// dialogs				
class BlakeTTDialog : public Dialog
{
public:
	void cue(void);
};

// DItems
class BlakeTTDItem : public DItem
{
public:
	void doit(void);
};

// dialog doits
class BlakeTeletalkWoman : public BlakeTTDItem
{
public:
	void doit();
};

class BlakeTeletalkHolt : public BlakeTTDItem
{
public:
	void doit();
};

// misc
class BlakeTTHangUpObj : public PermListObject
{
public:
	int		playingVoice;
	BlakeTTHangUpObj();
	void dispose(void);
};

} // namespace Spycraft

#endif
