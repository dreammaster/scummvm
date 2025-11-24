
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

#ifndef AGS2_DATA_GAME_SETUP_STRUCT_H
#define AGS2_DATA_GAME_SETUP_STRUCT_H

#include "ags2/data/game_setup_struct_base.h"
#include "ags2/data/inventory_item_info.h"
#include "ags2/data/mouse_cursor.h"

namespace AGS2 {

struct GameSetupStruct : public GameSetupStructBase {
	unsigned char     fontflags[MAX_FONTS];
	char              fontoutline[MAX_FONTS];
	unsigned char     spriteflags[MAX_SPRITES];
	InventoryItemInfo invinfo[MAX_INV];
	MouseCursor       mcurs[MAX_CURSOR];
	NewInteraction **intrChar;
	NewInteraction *intrInv[MAX_INV];
	InteractionScripts **charScripts;
	InteractionScripts **invScripts;
	int               filever;  // just used by editor
	char              lipSyncFrameLetters[MAXLIPSYNCFRAMES][50];
	CustomPropertySchema propSchema;
	CustomProperties *charProps, invProps[MAX_INV];
	char **viewNames;
	char              invScriptNames[MAX_INV][MAX_SCRIPT_NAME_LEN];
	char              dialogScriptNames[MAX_DIALOG][MAX_SCRIPT_NAME_LEN];
	char              guid[MAX_GUID_LENGTH];
	char              saveGameFileExtension[MAX_SG_EXT_LENGTH];
	char              saveGameFolderName[MAX_SG_FOLDER_LEN];
	int               roomCount;
	int *roomNumbers;
	char **roomNames;
	int               audioClipCount;
	ScriptAudioClip *audioClips;
	int               audioClipTypeCount;
	AudioClipType *audioClipTypes;
};

struct OriGameSetupStruct {
	char              gamename[30];
	char              options[20];
	unsigned char     paluses[256];
	color             defpal[256];
	InterfaceElement  iface[10];
	int               numiface;
	int               numviews;
	MouseCursor       mcurs[10];
	char *globalscript;
	int               numcharacters;
	OldCharacterInfo *chars;
	EventBlock        __charcond[50];
	EventBlock        __invcond[100];
	ccScript *compiled_script;
	int               playercharacter;
	unsigned char     __old_spriteflags[2100];
	int               totalscore;
	short             numinvitems;
	InventoryItemInfo invinfo[100];
	int               numdialog, numdlgmessage;
	int               numfonts;
	int               color_depth;              // in bytes per pixel (ie. 1 or 2)
	int               target_win;
	int               dialog_bullet;            // 0 for none, otherwise slot num of bullet point
	short             hotdot, hotdotouter;   // inv cursor hotspot dot
	int               uniqueid;    // random key identifying the game
	int               reserved[2];
	short             numlang;
	char              langcodes[MAXLANGUAGE][3];
	char *messages[MAXGLOBALMES];
};

struct OriGameSetupStruct2 : public OriGameSetupStruct {
	unsigned char   fontflags[10];
	char            fontoutline[10];
	int             numgui;
	WordsDictionary *dict;
	int             reserved2[8];
};

struct OldGameSetupStruct : public OriGameSetupStruct2 {
	unsigned char spriteflags[6000];
};

} // namespace AGS2

#endif
