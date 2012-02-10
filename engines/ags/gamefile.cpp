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

#include "common/debug.h"
#include "common/stream.h"
#include "common/textconsole.h"

#include "ags/constants.h"
#include "ags/gamefile.h"
#include "ags/resourceman.h"
#include "ags/script.h"

namespace AGS {

GameFile::GameFile() {
}

GameFile::~GameFile() {
}

void GameFile::readVersion(Common::SeekableReadStream &dta) {
	_version = dta.readUint32LE();

	uint32 versionStringLength = dta.readUint32LE();

	byte *versionString = new byte[versionStringLength + 1];
	dta.read(versionString, versionStringLength);
	versionString[versionStringLength] = '\0';

	_versionString = (const char *) versionString;

	delete[] versionString;
}

void GameFile::decryptText(uint8 *str, uint32 max) {
	static const char *kSecretPassword = "Avis Durgan";

	uint passPos = 0;
	while (max-- > 0) {
		*str -= (uint8) kSecretPassword[passPos];
		if (!*str)
			break;

		str++;

		passPos = (passPos + 1) % 11;
	}
}

Common::String GameFile::decryptString(Common::SeekableReadStream *dta) {
	uint32 stringLen = dta->readUint32LE();
	if (stringLen > 5000000)
		error("invalid value in decryptString");
	byte *string = new byte[stringLen + 1];
	dta->read(string, stringLen);
	string[stringLen] = 0;
	decryptText(string, stringLen);
	Common::String newString((char *)string);
	delete[] string;
	return newString;
}

#define MAX_SCRIPT_MODULES 50

bool GameFile::init(const ResourceManager &resMan) {
	Common::SeekableReadStream *dta = resMan.getFile("ac2game.dta");
	if (!dta)
		return false;

	dta->skip(30); // "Adventure Creator Game File v2"

	readVersion(*dta);
	if (_version > kAGSVer321)
		error("AGS version %d ('%s') is not supported", _version, _versionString.c_str());

	// now we read GameSetupStruct

	// game name
	char gameName[51];
	dta->read(gameName, 50);
	gameName[50] = '\0';
	_gameName = gameName;
	dta->skip(2); // padding

	debug(1, "AGS game file for '%s' has version %d ('%s')", _gameName.c_str(), _version, _versionString.c_str());

	// options
	for (uint32 i = 0; i < 100; i++)
		_options[i] = dta->readUint32LE();
	if (_version <= kAGSVer300) {
		// from PSP branch: fix animation speed for old formats
		_options[OPT_OLDTALKANIMSPD] = 1;
	}

	// palette
	for (uint32 i = 0; i < 256; i++)
		_palUses[i] = (uint8) dta->readByte();
	for (uint32 i = 0; i < 256; i++) {
		_defPal[3 * i + 0] = dta->readByte(); // R
		_defPal[3 * i + 1] = dta->readByte(); // G
		_defPal[3 * i + 2] = dta->readByte(); // B

		dta->skip(1); // Pad
	}

	_viewCount = dta->readUint32LE();
	_charCount = dta->readUint32LE();
	_playerChars = dta->readUint32LE();

	_totalScore = dta->readSint32LE();

	_invItemCount = dta->readUint16LE();
	dta->skip(2); // padding

	debug(2, "%d views, %d characters, %d player characters, %d inventory items",
		_viewCount, _charCount, _playerChars, _invItemCount);

	_dialogCount = dta->readUint32LE();
	_dlgMsgCount = dta->readUint32LE();
	_fontCount = dta->readUint32LE();

	_colorDepth = dta->readUint32LE();

	_targetWin = dta->readUint32LE();
	_dialogBullet = dta->readUint32LE();

	_hotDot = dta->readUint16LE();
	_hotDotOuter = dta->readUint16LE();

	_uniqueID = dta->readUint32LE();

	_guiCount = dta->readUint32LE();
	_cursorCount = dta->readUint32LE();

	_defaultResolution = dta->readUint32LE();
	_defaultLipSyncFrame = dta->readUint32LE();

	_invHotDotSprite = dta->readUint32LE();

	// reserved
	dta->skip(17 * 4);

	// messages
	dta->skip(500 * 4);

	// dict
	bool hasDict = (bool)dta->readUint32LE();
	// globalscript
	_globalScript = dta->readUint32LE();
	// chars
	dta->skip(4);
	// compiled_script
	_compiledScript = dta->readUint32LE();
	if (!_compiledScript)
		error("missing compiledScript");

	if (_version > kAGSVer272) {
		// FIXME: guid, save game extension/folder
		error("3.x not supported yet");
	}

	// fonts
	_fontFlags.resize(_fontCount);
	for (uint32 i = 0; i < _fontCount; ++i)
		_fontFlags[i] = dta->readByte();
	_fontOutline.resize(_fontCount);
	for (uint32 i = 0; i < _fontCount; ++i)
		_fontOutline[i] = dta->readByte();

	// TODO: PSP version fixes up fontOutlines here...

	// sprite flags
	uint32 spriteFlagCount = dta->readUint32LE();
	_spriteFlags.resize(spriteFlagCount);
	for (uint32 i = 0; i < spriteFlagCount; ++i)
		_spriteFlags[i] = dta->readByte();

	// inventory info
	_invItemInfo.resize(_invItemCount);
	for (uint32 i = 0; i < _invItemCount; ++i) {
		InventoryItemInfo &info = _invItemInfo[i];

		char invName[26];
		dta->read(invName, 25);
		invName[25] = '\0';
		info._name = invName;
		dta->skip(3); // padding

		info._pic = dta->readUint32LE();
		info._cursorPic = dta->readUint32LE();
		info._hotX = dta->readUint32LE();
		info._hotY = dta->readUint32LE();
		dta->skip(5 * 4); // reserved
		info._flags = dta->readByte();
		dta->skip(3); // padding
	}

	// cursors
	_cursors.resize(_cursorCount);
	for (uint32 i = 0; i < _cursorCount; ++i) {
		MouseCursor &cursor = _cursors[i];

		cursor._pic = dta->readUint32LE();
		cursor._hotX = dta->readUint16LE();
		cursor._hotY = dta->readUint16LE();
		cursor._view = dta->readSint16LE();

		char cursorName[11];
		dta->read(cursorName, 10);
		cursorName[10] = '\0';
		cursor._name = cursorName;

		cursor._flags = dta->readByte();
		dta->skip(3); // padding

		if (_version <= kAGSVer272) {
			// convert old value for 'not set'
			if (cursor._view == 0)
				cursor._view = -1;
		}
	}

	if (_version > kAGSVer272) {
		// FIXME: character and inventory scripts
		error("3.x not supported yet");
	} else {
		debug(1, "char interactions");
		_interactionsChar.resize(_charCount);
		for (uint32 i = 0; i < _charCount; i++)
			_interactionsChar[i] = readNewInteraction(dta);
		debug(1, "inv interactions");
		_interactionsInv.resize(_invItemCount);
		for (uint32 i = 0; i < _invItemCount; i++)
			_interactionsInv[i] = readNewInteraction(dta);

		uint32 globalVarsCount = dta->readUint32LE();
		_globalVars.resize(globalVarsCount);
		for (uint32 i = 0; i < globalVarsCount; ++i) {
			_globalVars[i] = readInteractionVariable(dta);
		}
	}

	if (hasDict) {
		uint32 wordsCount = dta->readUint32LE();
		_dict._words.resize(wordsCount);
		for (uint i = 0; i < wordsCount; ++i) {
			_dict._words[i]._word = decryptString(dta);
			_dict._words[i]._id = dta->readUint16LE();
		}
	}

	_gameScript = new ccScript();
	_gameScript->readFrom(dta);

	_dialogScriptsScript = NULL;
	if (_version > kAGSVer300) {
		// 3.1.1+ dialog script
		_dialogScriptsScript = new ccScript();
		_dialogScriptsScript->readFrom(dta);
	}

	if (_version >= kAGSVer270) {
		// 2.7.0+ script modules
		uint32 scriptModuleCount = dta->readUint32LE();
		if (scriptModuleCount >= MAX_SCRIPT_MODULES)
			error("too many script modules (%d)", scriptModuleCount);

		_scriptModules.resize(scriptModuleCount);
		for (uint i = 0; i < scriptModuleCount; ++i) {
			_scriptModules[i] = new ccScript();
			_scriptModules[i]->readFrom(dta);
		}
	}

	delete dta;

	return true;
}

InteractionVariable GameFile::readInteractionVariable(Common::SeekableReadStream *dta) {
	InteractionVariable var;

	char varName[24];
	dta->read(varName, 23);
	varName[23] = '\0';
	var._name = varName;

	var._type = dta->readByte();
	var._value = dta->readSint32LE();

	return var;
}

#define MAX_NEWINTERACTION_EVENTS 30

NewInteraction *GameFile::readNewInteraction(Common::SeekableReadStream *dta) {
	uint32 unknown = dta->readUint32LE();
	if (unknown != 1) {
		if (unknown != 0)
			error("invalid interaction? (%d)", unknown);
		return NULL;
	}
	debug(1, "new interaction");

	NewInteraction *interaction = new NewInteraction();

	uint32 numEvents = dta->readUint32LE();
	if (numEvents >= MAX_NEWINTERACTION_EVENTS)
		error("too many new interaction events (%d)", numEvents);
	interaction->_events.resize(numEvents);
	for (uint32 i = 0; i < numEvents; i++) {
		interaction->_events[i]._type = dta->readUint32LE();
		interaction->_events[i]._timesRun = 0;
		interaction->_events[i]._response = NULL;
	}

	Common::Array<bool> hasResponse;
	hasResponse.resize(numEvents);
	for (uint32 i = 0; i < numEvents; ++i)
		hasResponse[i] = (bool)dta->readUint32LE();
	for (uint32 i = 0; i < numEvents; ++i) {
		if (!hasResponse[i])
			continue;

		debug(1, "reading response");
		interaction->_events[i]._response = readCommandList(dta);
	}

	return interaction;
}

#define MAX_ACTION_ARGS 5

NewInteractionCommandList *GameFile::readCommandList(Common::SeekableReadStream *dta) {
	NewInteractionCommandList *list = new NewInteractionCommandList();
	uint32 commandsCount = dta->readUint32LE();
	list->_timesRun = dta->readUint32LE();

	Common::Array<bool> hasChildren;
	Common::Array<bool> hasParent;
	list->_commands.resize(commandsCount);
	for (uint32 i = 0; i < commandsCount; ++i) {
		dta->skip(4); // vtable ptr
		list->_commands[i]._type = dta->readUint32LE();

		list->_commands[i]._args.resize(MAX_ACTION_ARGS);
		for (uint j = 0; j < MAX_ACTION_ARGS; ++j) {
			list->_commands[i]._args[j]._type = dta->readByte();
			dta->skip(3);
			list->_commands[i]._args[j]._val = dta->readUint32LE();
			list->_commands[i]._args[j]._extra = dta->readUint32LE();
		}

		hasChildren.push_back((bool)dta->readUint32LE());
		/*bool hasParent = (bool)*/ dta->readUint32LE();
		list->_commands[i]._parent = list;
	}

	list->_commands.resize(commandsCount);
	for (uint32 i = 0; i < commandsCount; ++i) {
		if (!hasChildren[i])
			continue;
		list->_commands[i]._children = readCommandList(dta);
	}

	return list;
}

} // End of namespace AGS
