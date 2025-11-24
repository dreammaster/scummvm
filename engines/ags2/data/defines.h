
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

#ifndef AGS2_DATA_DEFINES_H
#define AGS2_DATA_DEFINES_H

namespace AGS2 {

#define GAME_FILE_VERSION 42
#define MAX_PATH 256

// Max script string length
#define MAX_MAXSTRLEN 200
#define MAXGLOBALVARS 50
#define INVALID_X  30000
#define MAXGSVALUES 500
#define MAXGLOBALSTRINGS 51
#define MAX_INVORDER 500
#define MAX_TIMERS       21
#define MAX_PARSED_WORDS 15
#define MAXSAVEGAMES     50
#define MAX_QUEUED_MUSIC 10
#define MAX_AUDIO_TYPES  30
#define MAX_WALK_AREAS 15
#define MAXANIMS      10
#define MAX_FLAGS     15
#define MAXOBJNAMELEN 30
#define MAX_BSCENE    5   // max number of frames in animating bg scene
#define MAX_SPRITES         30000
#define MAX_CURSOR          20
#define MAXGLOBALMES        500
#define MAXLANGUAGE         5
#define MAX_FONTS           15
#define MAX_PARSER_WORD_LENGTH 30
#define MAX_INV             301

#define MAX_INIT_SPR  40
#define MAX_OBJ       16  // max walk-behinds
#define NUM_MISC      20
#define MAXMESS       100
#define NUMOTCON      7                 // number of conditions before standing on
#define NUM_CONDIT    (120 + NUMOTCON)
#define MAX_HOTSPOTS  50   // v2.62 increased from 20 to 30; v2.8 to 50
#define MAX_REGIONS   16

#define MAX_SCRIPT_NAME_LEN 20
#define MAX_ROOMS 300

#define MAXVIEWNAMELENGTH 15
#define MAXLIPSYNCFRAMES  20
#define MAX_GUID_LENGTH   40
#define MAX_SG_EXT_LENGTH 20
#define MAX_SG_FOLDER_LEN 50

#define QUEUED_MUSIC_REPEAT 10000
#define PLAYMP3FILE_MAX_FILENAME_LEN 50

} // namespace AGS2

#endif
