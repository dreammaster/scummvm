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
#include "ags/util.h"

namespace AGS {

GameFile::GameFile(AGSEngine *vm) : _vm(vm) {
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

#define MAX_SCRIPT_MODULES 50
#define MAXLIPSYNCFRAMES  20

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

	_dialogCount = dta->readUint32LE();
	_dlgMsgCount = dta->readUint32LE();
	_fontCount = dta->readUint32LE();

	debug(2, "%d views, %d characters, %d player characters, %d inventory items, %d dialog topics",
		_viewCount, _charCount, _playerChars, _invItemCount, _dialogCount);

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
	Common::Array<bool> hasMessages;
	hasMessages.resize(MAXGLOBALMES);
	for (uint i = 0; i < MAXGLOBALMES; i++)
		hasMessages[i] = (bool)dta->readUint32LE();

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

	if (_version > kAGSVer272) {
		// 3.x views

		// FIXME
		error("3.x not supported yet");
	} else {
		// 2.x views
		readOldViews(dta);
	}

	_chars.resize(_charCount);
	for (uint i = 0; i < _charCount; ++i)
		_chars[i] = readCharacter(dta);

	// FIXME: PSP version fixes up character script names here
	// FIXME: PSP version fixes up ANTIGLIDE here

	// FIXME: lip sync data
	for (uint i = 0; i < MAXLIPSYNCFRAMES; ++i)
		dta->skip(50);

	_messages.resize(MAXGLOBALMES);
	setDefaultMessages();
	for (uint i = 0; i < MAXGLOBALMES; ++i) {
		if (!hasMessages[i])
			continue;

		// global messages are not encrypted on < 2.61
		if (_version < kAGSVer261)
			_messages[i] = readString(dta);
		else
			_messages[i] = decryptString(dta);

		debug(5, "message %d is '%s'", i, _messages[i].c_str());
	}

	// dialog topics
	Common::Array<uint16> dialogCodeSizes;
	_dialogs.resize(_dialogCount);
	dialogCodeSizes.resize(_dialogCount);
	for (uint i = 0; i < _dialogCount; ++i) {
		char optionNames[MAXTOPICOPTIONS][150 + 1];
		for (uint j = 0; j < MAXTOPICOPTIONS; ++j) {
			dta->read(optionNames[j], 150);
			optionNames[j][150] = '\0';
		}
		int optionFlags[MAXTOPICOPTIONS];
		for (uint j = 0; j < MAXTOPICOPTIONS; ++j)
			optionFlags[j] = dta->readUint32LE();
		bool hasScripts = (bool)dta->readUint32LE();
		uint16 entryPoints[MAXTOPICOPTIONS];
		for (uint j = 0; j < MAXTOPICOPTIONS; ++j)
			entryPoints[j] = dta->readUint16LE();
		_dialogs[i]._startupEntryPoint = dta->readUint16LE();
		dialogCodeSizes[i] = dta->readUint16LE();
		uint32 numOptions = dta->readUint32LE();
		_dialogs[i]._flags = dta->readUint32LE();

		if (numOptions > MAXTOPICOPTIONS)
			error("too many options (%d) in dialog topic", numOptions);
		_dialogs[i]._options.resize(numOptions);
		for (uint j = 0; j < numOptions; ++j) {
			_dialogs[i]._options[j]._name = optionNames[j];
			_dialogs[i]._options[j]._flags = optionFlags[j];
			_dialogs[i]._options[j]._entryPoint = entryPoints[j];
			debug(5, "dialog option '%s'", optionNames[j]);
		}
	}

	if (_version <= kAGSVer300) {
		// <= 3.0: dialog scripts
		for (uint i = 0; i < _dialogCount; ++i) {
			if (dialogCodeSizes[i] == 0)
				continue;

			_dialogs[i]._code.resize(dialogCodeSizes[i]);
			dta->read(&_dialogs[i]._code[0], dialogCodeSizes[i]);

			// we can just discard this..
			Common::String dialogTextScript = decryptString(dta);
			debug(9, "dialog script text was %s", dialogTextScript.c_str());
		}

		while (true) {
			uint32 stringLen = dta->readUint32LE();
			if (dta->eos())
				error("corrupt data file while reading speech lines");
			if (stringLen == 0xcafebeef)
				break;

			byte *string = new byte[stringLen + 1];
			dta->read(string, stringLen);
			string[stringLen] = 0;
			decryptText(string, stringLen);
			_speechLines.push_back(Common::String((char *)string));
			delete[] string;

			debug(5, "speech line '%s'", _speechLines.back().c_str());
		}
	} else {
		uint32 magic = dta->readUint16LE();
		if (magic != 0xcafebeef)
			error("bad magic %x for GUI", magic);
	}

	readGui(dta);

	readPlugins(dta);

	readPropertyData(dta);

	for (uint i = 0; i < _views.size(); ++i) {
		_views[i]._name = readString(dta);
		debug(5, "view %d is '%s'", i, _views[i]._name.c_str());
	}
	for (uint i = 0; i < _invItemInfo.size(); ++i) {
		_invItemInfo[i]._scriptName = readString(dta);
		debug(5, "inventory item %d has script '%s'", i, _invItemInfo[i]._scriptName.c_str());
	}
	for (uint i = 0; i < _dialogs.size(); ++i) {
		_dialogs[i]._name = readString(dta);
		debug(5, "dialog %d is '%s'", i, _dialogs[i]._name.c_str());
	}

	if (_version >= kAGSVer320) {
		// FIXME
		error("3.x not supported yet");
	} else {
		// TODO: audio clips
	}

	if ((_version >= kAGSVer300b) && _options[OPT_DEBUGMODE]) {
		uint32 roomCount = dta->readUint32LE();
		for (uint i = 0; i < roomCount; ++i) {
			// TODO: store these
			uint32 roomNumber = dta->readUint32LE();
			Common::String roomName = readString(dta);
			debug(5, "room %d (number %d) is '%s'", i, roomNumber, roomName.c_str());
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

ViewFrame GameFile::readViewFrame(Common::SeekableReadStream *dta) {
	ViewFrame frame;

	frame._pic = dta->readUint32LE();
	frame._xOffs = dta->readUint16LE();
	frame._yOffs = dta->readUint16LE();
	frame._speed = dta->readUint16LE();
	dta->skip(2); // padding
	frame._flags = dta->readUint32LE();
	frame._sound = dta->readUint32LE();
	dta->skip(2 * 4); // reserved_for_future

	return frame;
}

void GameFile::readOldViews(Common::SeekableReadStream *dta) {
	_views.resize(_viewCount);
	for (uint n = 0; n < _viewCount; ++n) {
		// read the old 2.x view data
		uint16 numLoops = dta->readUint16LE();
		uint16 numFrames[16];
		for (uint i = 0; i < 16; ++i) {
			numFrames[i] = dta->readUint16LE();
			if (numFrames[i] >= 20)
				error("too many frames (%d) in 2.x view", numFrames[i]);
		}
		dta->skip(2); // padding
		uint32 loopFlags[16];
		for (uint i = 0; i < 16; ++i)
			loopFlags[i] = dta->readUint32LE();
		ViewFrame frames[16][20];
		for (uint j = 0; j < 16; ++j)
			for (uint i = 0; i < 20; ++i)
				frames[j][i] = readViewFrame(dta);

		// create a new 3.x view from it
		_views[n]._loops.resize(numLoops);
		for (uint i = 0; i < numLoops; ++i) {
			ViewLoopNew &loop = _views[n]._loops[i];
			if (numFrames[i] > 0 && frames[i][numFrames[i] - 1]._pic == 0xffffffff) {
				// rewrite to use a flag rather than an invalid last frame
				loop._flags = LOOPFLAG_RUNNEXTLOOP;
				numFrames[i]--;
			} else
				loop._flags = 0;

			for (uint j = 0; j < numFrames[i]; ++j)
				loop._frames.push_back(frames[i][j]);
		}
	}
}

CharacterInfo *GameFile::readCharacter(Common::SeekableReadStream *dta) {
	CharacterInfo *chr = new CharacterInfo();

	chr->_defView = dta->readUint32LE();
	chr->_talkView = dta->readUint32LE();
	chr->_view = dta->readUint32LE();
	chr->_room = dta->readUint32LE();
	chr->_prevRoom = dta->readUint32LE();
	chr->_x = dta->readUint32LE();
	chr->_y = dta->readUint32LE();
	chr->_wait = dta->readUint32LE();
	chr->_flags = dta->readUint32LE();
	chr->_following = dta->readUint16LE();
	chr->_followInfo = dta->readUint16LE();
	chr->_idleView = dta->readUint32LE();
	chr->_idleTime = dta->readUint16LE();
	chr->_idleLeft = dta->readUint16LE();
	chr->_transparency = dta->readUint16LE();
	chr->_baseLine = dta->readUint16LE();
	chr->_activeInv = dta->readUint32LE();
	chr->_talkColor = dta->readUint32LE();
	chr->_thinkView = dta->readUint32LE();
	chr->_blinkView = dta->readUint16LE();
	chr->_blinkInterval = dta->readUint16LE();
	chr->_blinkTimer = dta->readUint16LE();
	chr->_blinkFrame = dta->readUint16LE();
	chr->_walkSpeedY = dta->readUint16LE();
	chr->_picYOffs = dta->readUint16LE();
	chr->_z = dta->readUint32LE();
	dta->skip(2 * 4); // reserved
	chr->_blockingWidth = dta->readUint16LE();
	chr->_blockingHeight = dta->readUint16LE();
	chr->_indexId = dta->readUint32LE();
	chr->_picXOffs = dta->readUint16LE();
	chr->_walkWaitCounter = dta->readUint16LE();
	chr->_loop = dta->readUint16LE();
	chr->_frame = dta->readUint16LE();
	chr->_walking = dta->readUint16LE();
	chr->_animating = dta->readUint16LE();
	chr->_walkSpeed = dta->readUint16LE();
	chr->_animSpeed = dta->readUint16LE();
	chr->_inventory.resize(MAX_INV);
	for (uint i = 0; i < MAX_INV; ++i)
		chr->_inventory[i] = dta->readUint16LE();
	chr->_actX = dta->readUint16LE();
	chr->_actY = dta->readUint16LE();

	char ourName[41];
	dta->read(ourName, 40);
	ourName[40] = '\0';
	chr->_name = ourName;

	char scriptName[MAX_SCRIPT_NAME_LEN + 1];
	dta->read(scriptName, MAX_SCRIPT_NAME_LEN);
	scriptName[MAX_SCRIPT_NAME_LEN] = '\0';
	chr->_scriptName = scriptName;

	chr->_on = dta->readByte();
	dta->skip(1); // padding

	debug(2, "read char '%s', script name '%s'", ourName, scriptName);

	return chr;
}

void GameFile::setDefaultMessages() {
	_messages[483] = "Sorry, not now.";
	_messages[484] = "Restore";
	_messages[485] = "Cancel";
	_messages[486] = "Select a game to restore:";
	_messages[487] = "Save";
	_messages[488] = "Type a name to save as:";
	_messages[489] = "Replace";
	_messages[490] = "The save directory is full. You must replace an existing game:";
	_messages[491] = "Replace:";
	_messages[492] = "With:";
	_messages[493] = "Quit";
	_messages[494] = "Play";
	_messages[495] = "Are you sure you want to quit?";
}

#define GUI_VERSION 115

void GameFile::readGui(Common::SeekableReadStream *dta) {
	_guiVersion = dta->readUint32LE();
	uint32 guiCount;
	if (_guiVersion < 100) {
		guiCount = _guiVersion;
	} else if (_guiVersion <= GUI_VERSION) {
		guiCount = dta->readUint32LE();
	} else {
		error("GUI version %d is too new?", _guiVersion);
	}

	if (guiCount > 1000)
		error("GUI is corrupt? (%d entries)", guiCount);

	debug(2, "GUI version %d, with %d groups", _guiVersion, guiCount);

	_guiGroups.resize(guiCount);
	for (uint i = 0; i < guiCount; ++i) {
		GUIGroup &group = _guiGroups[i];

		dta->read(group._vText, 4);

		char name[16 + 1];
		dta->read(name, 16);
		name[16] = '\0';
		group._name = name;
		if (_version <= kAGSVer272 && !group._name.empty()) {
			// Fix names for 2.x: "GUI" -> "gGui"
			group._name.toLowercase();
			group._name.setChar(toupper(_guiGroups[i]._name[0]), 0);
			group._name.insertChar('g', 0);
		}
		debug(4, "gui group '%s'", group._name.c_str());

		char clickEventHandler[20 + 1];
		dta->read(clickEventHandler, 20);
		clickEventHandler[20] = '\0';
		group._clickEventHandler = clickEventHandler;

		group._x = dta->readUint32LE();
		group._y = dta->readUint32LE();
		group._width = dta->readUint32LE();
		group._height = dta->readUint32LE();
		group._focus = dta->readUint32LE();
		uint32 objectsCount = dta->readUint32LE();
		group._popup = dta->readUint32LE();
		group._popupYP = dta->readUint32LE();
		group._bgColor = dta->readUint32LE();
		group._bgPic = dta->readUint32LE();
		group._fgColor = dta->readUint32LE();
		group._mouseOver = dta->readUint32LE();
		group._mouseWasX = dta->readUint32LE();
		group._mouseWasY = dta->readUint32LE();
		group._mouseDownOn = dta->readUint32LE();
		group._highlightObj = dta->readUint32LE();
		group._flags = dta->readUint32LE();
		group._transparency = dta->readUint32LE();
		group._zorder = dta->readUint32LE();
		group._id = dta->readUint32LE();
		dta->skip(6 * 4); // reserved
		group._on = dta->readUint32LE();

		dta->skip(MAX_OBJS_ON_GUI * 4); // obj pointers
		group._objectRefPtrs.resize(MAX_OBJS_ON_GUI);
		for (uint j = 0; j < MAX_OBJS_ON_GUI; ++j)
			group._objectRefPtrs[j] = dta->readUint32LE();

		// fixes/upgrades
		if (group._height < 2)
			group._height = 2;
		if (_guiVersion < 103)
			group._name = Common::String::format("GUI%d", i);
		if (_guiVersion < 105)
			group._zorder = i;
		group._id = i;
	}

	uint32 buttonCount = dta->readUint32LE();
	_guiButtons.resize(buttonCount);
	for (uint i = 0; i < buttonCount; ++i) {
		_guiButtons[i] = new GUIButton(_vm);
		_guiButtons[i]->readFrom(dta);
	}

	uint32 labelCount = dta->readUint32LE();
	_guiLabels.resize(labelCount);
	for (uint i = 0; i < labelCount; ++i) {
		_guiLabels[i] = new GUILabel(_vm);
		_guiLabels[i]->readFrom(dta);
	}

	uint32 invControlCount = dta->readUint32LE();
	_guiInvControls.resize(invControlCount);
	for (uint i = 0; i < invControlCount; ++i) {
		_guiInvControls[i] = new GUIInvControl(_vm);
		_guiInvControls[i]->readFrom(dta);
	}

	if (_guiVersion >= 100) {
		uint32 sliderCount = dta->readUint32LE();
		_guiSliders.resize(sliderCount);
		for (uint i = 0; i < sliderCount; ++i) {
			_guiSliders[i] = new GUISlider(_vm);
			_guiSliders[i]->readFrom(dta);
		}
	}

	if (_guiVersion >= 101) {
		uint32 textboxCount = dta->readUint32LE();
		_guiTextBoxes.resize(textboxCount);
		for (uint i = 0; i < textboxCount; ++i) {
			_guiTextBoxes[i] = new GUITextBox(_vm);
			_guiTextBoxes[i]->readFrom(dta);
		}
	}

	if (_guiVersion >= 102) {
		uint32 listboxCount = dta->readUint32LE();
		_guiListBoxes.resize(listboxCount);
		for (uint i = 0; i < listboxCount; ++i) {
			_guiListBoxes[i] = new GUIListBox(_vm);
			_guiListBoxes[i]->readFrom(dta);
		}
	}

	// TODO: setup the gui lookup array etc

	// TODO: draw order?
}

void GameFile::readPlugins(Common::SeekableReadStream *dta) {
	uint pluginsVersion = dta->readUint32LE();
	if (pluginsVersion != 1)
		error("invalid plugins version %d", pluginsVersion);
	uint32 pluginsCount = dta->readUint32LE();

	for (uint i = 0; i < pluginsCount; ++i) {
		Common::String name = readString(dta);
		warning("ignoring plugin '%s'", name.c_str());
		uint32 pluginSize = dta->readUint32LE();
		dta->skip(pluginSize);
	}
}

void GameFile::readPropertyData(Common::SeekableReadStream *dta) {
	// custom property schema
	uint32 schemaVersion = dta->readUint32LE();
	if (schemaVersion != 1)
		error("invalid schema version %d", schemaVersion);
	uint32 numProperties = dta->readUint32LE();
	_schemaProperties.resize(numProperties);
	for (uint i = 0; i < numProperties; ++i) {
		CustomPropertySchemaProperty &property = _schemaProperties[i];
		property._name = readString(dta);
		property._description = readString(dta);
		property._defaultValue = readString(dta);
		property._type = dta->readUint32LE();
		debug(7, "schema property '%s' ('%s'), default '%s'", property._name.c_str(),
			property._description.c_str(), property._defaultValue.c_str());
	}

	// character properties
	for (uint i = 0; i < _chars.size(); ++i) {
		readProperties(dta, _chars[i]->_properties);
	}

	// inventory item properties
	for (uint i = 0; i < _invItemInfo.size(); ++i) {
		readProperties(dta, _invItemInfo[i]._properties);
	}
}

void GameFile::readProperties(Common::SeekableReadStream *dta, Common::StringMap &map) {
	uint32 propVersion = dta->readUint32LE();
	if (propVersion != 1)
		error("invalid properties version %d", propVersion);
	uint32 numProperties = dta->readUint32LE();
	for (uint i = 0; i < numProperties; ++i) {
		Common::String key = readString(dta);
		Common::String value = readString(dta);
		map[key] = value;
		debug(7, "property '%s'='%s'", key.c_str(), value.c_str());
	}
}

} // End of namespace AGS
