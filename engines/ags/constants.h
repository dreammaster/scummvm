/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef AGS_CONSTANTS_H
#define AGS_CONSTANTS_H

namespace AGS {

enum AGSVersion {
	kAGSVer251 = 19,
	kAGSVer255 = 22,
	kAGSVer256 = 24,
	kAGSVer261 = 26,
	kAGSVer262 = 27,
	kAGSVer270 = 31,
	kAGSVer272 = 32,
	kAGSVer300 = 37,
	kAGSVer311 = 39,
	kAGSVer312 = 40,
	kAGSVer320 = 41,
	kAGSVer321 = 42
};

enum AGSRoomVersion {
	kAGSRoomVer114 = 8,
	kAGSRoomVer2a = 9,
	kAGSRoomVer2a7 = 10,
	kAGSRoomVer200 = 11,
	kAGSRoomVer208 = 12,
	kAGSRoomVer214 = 13,
	kAGSRoomVer240 = 14,
	kAGSRoomVer241 = 15,
	kAGSRoomVer250 = 16,
	kAGSRoomVer250x = 17,
	kAGSRoomVer251 = 18,
	kAGSRoomVer253 = 19,
	kAGSRoomVer255 = 20,
	kAGSRoomVer255r = 21,
	kAGSRoomVer261 = 22,
	kAGSRoomVer262 = 23,
	kAGSRoomVer270 = 24,
	kAGSRoomVer272 = 25,
	kAGSRoomVer300 = 26,
	kAGSRoomVer300x = 27,
	kAGSRoomVer303 = 28,
	kAGSRoomVer303x = 29
};

// from acroom.h of AGS source

#define OPT_DEBUGMODE       0
#define OPT_SCORESOUND      1
#define OPT_WALKONLOOK      2
#define OPT_DIALOGIFACE     3
#define OPT_ANTIGLIDE       4
#define OPT_TWCUSTOM        5
#define OPT_DIALOGGAP       6
#define OPT_NOSKIPTEXT      7
#define OPT_DISABLEOFF      8
#define OPT_ALWAYSSPCH      9
#define OPT_SPEECHTYPE      10
#define OPT_PIXPERFECT      11
#define OPT_NOWALKMODE      12
#define OPT_LETTERBOX       13
#define OPT_FIXEDINVCURSOR  14
#define OPT_NOLOSEINV       15
#define OPT_NOSCALEFNT      16
#define OPT_SPLITRESOURCES  17
#define OPT_ROTATECHARS     18
#define OPT_FADETYPE        19
#define OPT_HANDLEINVCLICKS 20
#define OPT_MOUSEWHEEL      21
#define OPT_DIALOGNUMBERED  22
#define OPT_DIALOGUPWARDS   23
#define OPT_CROSSFADEMUSIC  24
#define OPT_ANTIALIASFONTS  25
#define OPT_THOUGHTGUI      26
#define OPT_TURNTOFACELOC   27
#define OPT_RIGHTLEFTWRITE  28  // right-to-left text writing
#define OPT_DUPLICATEINV    29  // if they have 2 of the item, draw it twice
#define OPT_SAVESCREENSHOT  30
#define OPT_PORTRAITSIDE    31
#define OPT_STRICTSCRIPTING 32  // don't allow MoveCharacter-style commands
#define OPT_LEFTTORIGHTEVAL 33  // left-to-right operator evaluation
#define OPT_COMPRESSSPRITES 34
#define OPT_STRICTSTRINGS   35  // don't allow old-style strings
#define OPT_NEWGUIALPHA     36
#define OPT_RUNGAMEDLGOPTS  37
#define OPT_NATIVECOORDINATES 38
#define OPT_OLDTALKANIMSPD  39
#define OPT_HIGHESTOPTION   39
#define OPT_NOMODMUSIC      98
#define OPT_LIPSYNCTEXT     99
#define PORTRAIT_LEFT       0
#define PORTRAIT_RIGHT      1
#define PORTRAIT_ALTERNATE  2
#define PORTRAIT_XPOSITION  3
#define FADE_NORMAL         0
#define FADE_INSTANT        1
#define FADE_DISSOLVE       2
#define FADE_BOXOUT         3
#define FADE_CROSSFADE      4
#define FADE_LAST           4   // this should equal the last one
#define SPF_640x400         1
#define SPF_HICOLOR         2
#define SPF_DYNAMICALLOC    4
#define SPF_TRUECOLOR       8
#define SPF_ALPHACHANNEL 0x10
#define SPF_HADALPHACHANNEL 0x80  // the saved sprite on disk has one
//#define FFLG_NOSCALE        1
#define FFLG_SIZEMASK 0x003f
#define FONT_OUTLINE_AUTO -10

} // End of namespace AGS

#endif // AGS_CONSTANTS_H
