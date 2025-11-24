
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

#ifndef AGS2_DATA_GAME_SETUP_STRUCT_BASE_H
#define AGS2_DATA_GAME_SETUP_STRUCT_BASE_H

#include "common/serializer.h"
#include "ags2/lib/allegro/color.h"
#include "ags2/data/defines.h"
#include "ags2/data/character_info.h"
#include "ags2/data/words_dictionary.h"
#include "ags2/common/cscomp.h"

namespace AGS2 {

/**
 * This struct is written directly to the disk file
 * The GameSetupStruct subclass parts are written individually
 */
struct GameSetupStructBase {
	char              gamename[50];
	int32             options[100];
	unsigned char     paluses[256];
	color             defpal[256];
	int32             numviews;
	int32             numcharacters;
	int32             playercharacter;
	int32             totalscore;
	short             numinvitems;
	int32             numdialog, numdlgmessage;
	int32             numfonts;
	int32             color_depth;          // in bytes per pixel (ie. 1 or 2)
	int32             target_win;
	int32             dialog_bullet;        // 0 for none, otherwise slot num of bullet point
	unsigned short    hotdot, hotdotouter;  // inv cursor hotspot dot
	int32             uniqueid;    // random key identifying the game
	int32             numgui;
	int32             numcursors;
	int32             default_resolution; // 0=undefined, 1=320x200, 2=320x240, 3=640x400 etc
	int32             default_lipsync_frame; // used for unknown chars
	int32             invhotdotsprite;
	int32             reserved[17];
	char *messages[MAXGLOBALMES];
	WordsDictionary *dict;
	char *globalscript;
	CharacterInfo *chars;
	ccScript *compiled_script;

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

} // namespace AGS2

#endif
