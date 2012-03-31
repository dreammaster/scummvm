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

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#ifndef AGS_CONSTANTS_H
#define AGS_CONSTANTS_H

namespace AGS {

enum kDebugLevels {
	kDebugLevelGame		= 1 << 0
};

extern const char *kGameDataNameV2;
extern const char *kGameDataNameV3;

enum AGSVersion {
	kAGSVer251 = 19,
	kAGSVer255 = 22,
	kAGSVer256 = 24,
	kAGSVer261 = 26,
	kAGSVer262 = 27,
	kAGSVer270 = 31,
	kAGSVer272 = 32,
	kAGSVer300b = 36, // ???
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

#define MAX_SPRITES         30000
#define MAX_CURSOR          20
#define PAL_GAMEWIDE        0
#define PAL_LOCKED          1
#define PAL_BACKGROUND      2
#define MAXGLOBALMES        500
#define MAXLANGUAGE         5
#define MAX_FONTS           15
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

// CharacterInfo
#define MAX_INV             301
#define CHF_MANUALSCALING   1
#define CHF_FIXVIEW         2     // between SetCharView and ReleaseCharView
#define CHF_NOINTERACT      4
#define CHF_NODIAGONAL      8
#define CHF_ALWAYSIDLE      0x10
#define CHF_NOLIGHTING      0x20
#define CHF_NOTURNING       0x40
#define CHF_NOWALKBEHINDS   0x80
#define CHF_FLIPSPRITE      0x100  // ?? Is this used??
#define CHF_NOBLOCKING      0x200
#define CHF_SCALEMOVESPEED  0x400
#define CHF_NOBLINKANDTHINK 0x800
#define CHF_SCALEVOLUME     0x1000
#define CHF_HASTINT         0x2000   // engine only
#define CHF_BEHINDSHEPHERD  0x4000   // engine only
#define CHF_AWAITINGMOVE    0x8000   // engine only
#define CHF_MOVENOTWALK     0x10000   // engine only - do not do walk anim
#define CHF_ANTIGLIDE       0x20000
// Speechcol is no longer part of the flags as of v2.5
#define OCHF_SPEECHCOL      0xff000000
#define OCHF_SPEECHCOLSHIFT 24
#define UNIFORM_WALK_SPEED  0
#define FOLLOW_ALWAYSONTOP  0x7ffe

#define MAX_SCRIPT_NAME_LEN 20

// DialogTopic
#define MAXTOPICOPTIONS     30
#define DFLG_ON             1  // currently enabled
#define DFLG_OFFPERM        2  // off forever (can't be trurned on)
#define DFLG_NOREPEAT       4  // character doesn't repeat it when clicked
#define DFLG_HASBEENCHOSEN  8  // dialog option is 'read'
#define DTFLG_SHOWPARSER    1  // show parser in this topic
#define DCMD_SAY            1
#define DCMD_OPTOFF         2
#define DCMD_OPTON          3
#define DCMD_RETURN         4
#define DCMD_STOPDIALOG     5
#define DCMD_OPTOFFFOREVER  6
#define DCMD_RUNTEXTSCRIPT  7
#define DCMD_GOTODIALOG     8
#define DCMD_PLAYSOUND      9
#define DCMD_ADDINV         10
#define DCMD_SETSPCHVIEW    11
#define DCMD_NEWROOM        12
#define DCMD_SETGLOBALINT   13
#define DCMD_GIVESCORE      14
#define DCMD_GOTOPREVIOUS   15
#define DCMD_LOSEINV        16
#define DCMD_ENDSCRIPT      0xff
#define DCHAR_NARRATOR  999
#define DCHAR_PLAYER    998
#define MAX_DIALOG          500

// MouseCursor
#define MCF_ANIMMOVE 1
#define MCF_DISABLED 2
#define MCF_STANDARD 4
#define MCF_HOTSPOT  8  // only animate when over hotspot

#define CHANIM_REPEAT    2
#define CHANIM_BACKWARDS 4
#define ANIM_BACKWARDS 10
#define ANIM_ONCE      1
#define ANIM_REPEAT    2
#define ANIM_ONCERESET 3
#define FONT_STATUSBAR  0
//#define FONT_SPEECHBACK 1
#define MODE_WALK 0
#define MODE_LOOK 1
#define MODE_HAND 2
#define MODE_TALK 3
#define MODE_USE  4
#define MODE_PICKUP 5
#define CURS_ARROW  6
#define CURS_WAIT   7
#define MODE_CUSTOM1 8
#define MODE_CUSTOM2 9

#define OVER_TEXTMSG  1
#define OVER_COMPLETE 2
#define OVER_PICTURE  3
#define OVER_CUSTOM   100
#define OVR_AUTOPLACE 30000

#define EVENT_NONE       0
#define EVENT_INPROGRESS 1
#define EVENT_CLAIMED    2

#define SKIP_AUTOTIMER  1
#define SKIP_KEYPRESS   2
#define SKIP_MOUSECLICK 4

#define TURNING_AROUND     1000
#define TURNING_BACKWARDS 10000

#define AMBIENCE_FULL_DIST 25

// parameters to run_on_event
#define GE_LEAVE_ROOM 1
#define GE_ENTER_ROOM 2
#define GE_MAN_DIES   3
#define GE_GOT_SCORE  4
#define GE_GUI_MOUSEDOWN 5
#define GE_GUI_MOUSEUP   6
#define GE_ADD_INV       7
#define GE_LOSE_INV      8
#define GE_RESTORE_GAME  9

// These numbers were chosen arbitrarily -- the idea is
// to make sure that the user gets the parameters the right way round
#define ANYWHERE       304
#define WALKABLE_AREAS 305
#define BLOCKING       919
#define IN_BACKGROUND  920
#define FORWARDS       1062
#define BACKWARDS      1063
#define VOL_CHANGEEXISTING   1678
#define VOL_SETFUTUREDEFAULT 1679
#define VOL_BOTH             1680
#define SCR_NO_VALUE   31998
#define SCR_COLOR_TRANSPARENT -1

// GameState
#define INVALID_X  30000
#define MAXGSVALUES 500
#define MAXGLOBALSTRINGS 51
#define MAX_INVORDER 500
#define SCALIGN_LEFT     1
#define SCALIGN_CENTRE   2
#define SCALIGN_RIGHT    3
#define DIALOG_NONE      0
#define DIALOG_RUNNING   1
#define DIALOG_STOP      2
#define DIALOG_NEWROOM   100
#define DIALOG_NEWTOPIC  12000
#define MAX_TIMERS       21
#define MAX_PARSED_WORDS 15
#define MAXSAVEGAMES     50
#define MAX_QUEUED_MUSIC 10
#define GLED_INTERACTION 1
#define GLED_EFFECTS     2
#define QUEUED_MUSIC_REPEAT 10000
#define PLAYMP3FILE_MAX_FILENAME_LEN 50
#define MAX_AUDIO_TYPES  30

// ViewFrame
#define VFLG_FLIPSPRITE 1

// hardcoded global message indexes for the hardcoded UI
// (for some reason the original engine has them 500 higher
//  and then subtracts 500 every time it uses them)
#define MSG_MAYNOTINTERRUPT 485 // "Sorry, not now."
#define MSG_RESTORE      484    // "Restore"
#define MSG_CANCEL       485    // "Cancel"
#define MSG_SELECTLOAD   486    // "Select game to restore"
#define MSG_SAVEBUTTON   487    // "Save"
#define MSG_SAVEDIALOG   488    // "Save game name:"
#define MSG_REPLACE      489    // "Replace"
#define MSG_MUSTREPLACE  490    // "The folder is full. you must replace"
#define MSG_REPLACEWITH1 491    // "Replace:"
#define MSG_REPLACEWITH2 492    // "With:"
#define MSG_QUITBUTTON   493    // "Quit"
#define MSG_PLAYBUTTON   494    // "Play"
#define MSG_QUITDIALOG   495    // "Do you want to quit?"

} // End of namespace AGS

#endif // AGS_CONSTANTS_H
