
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

#ifndef AGS2_ACRUNTIME_H
#define AGS2_ACRUNTIME_H

namespace AGS2 {


enum DBG {
	DBG_NOIFACE = 1,
	DBG_NODRAWSPRITES = 2,
	DBG_NOOBJECTS = 4,
	DBG_NOUPDATE = 8,
	DBG_NOSFX = 0x10,
	DBG_NOMUSIC = 0x20,
	DBG_NOSCRIPT = 0x40,
	DBG_DBGSCRIPT = 0x80,
	DBG_DEBUGMODE = 0x100,
	DBG_REGONLY = 0x200,
	DBG_NOVIDEO = 0x400
};

enum EV {
	EV_TEXTSCRIPT = 1,
	EV_RUNEVBLOCK = 2,
	EV_FADEIN = 3,
	EV_IFACECLICK = 4,
	EV_NEWROOM = 5
};

enum TS {
	TS_REPEAT = 1,
	TS_KEYPRESS = 2,
	TS_MCLICK = 3
};

enum EVB {
	EVB_HOTSPOT = 1,
	EVB_ROOM = 2
};

enum LOCTYPE {
	LOCTYPE_HOTSPOT = 1,
	LOCTYPE_CHAR = 2,
	LOCTYPE_OBJ = 3
};

enum WalkBehindMethodEnum {
	DrawOverCharSprite,
	DrawAsSeparateSprite,
	DrawAsSeparateCharSprite
};


struct GameState {
	bool recording = false;
	bool playback = false;
	int  takeover_data = 0;  // value passed to RunAGSGame in previous game
	char  takeover_from[50] = {};
};

struct GameSetup {
	bool disable_exception_handling = false;
	int digicard = 0;
	int midicard = 0;
	bool usevox = false;
	bool no_speech_pack = false;
	int screenres = 0;
	bool windowed = false;
	const char *const data_files_dir = "";
	Common::String translation;
};

} // namespace AGS2

#endif
