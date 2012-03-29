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

#include "common/debug.h"
#include "common/stream.h"
#include "common/textconsole.h"

#include "ags/ags.h"
#include "ags/audio.h"
#include "ags/constants.h"
#include "ags/gamefile.h"
#include "ags/gamestate.h"
#include "ags/script.h"
#include "ags/util.h"

namespace AGS {

GameFile::GameFile(AGSEngine *vm) : _vm(vm), _gameScript(NULL) {
}

GameFile::~GameFile() {
	delete _gameScript;
	delete _dialogScriptsScript;
	for (uint i = 0; i < _scriptModules.size(); ++i)
		delete _scriptModules[i];

	for (uint i = 0; i < _guiButtons.size(); ++i)
		delete _guiButtons[i];
	for (uint i = 0; i < _guiLabels.size(); ++i)
		delete _guiLabels[i];
	for (uint i = 0; i < _guiInvControls.size(); ++i)
		delete _guiInvControls[i];
	for (uint i = 0; i < _guiSliders.size(); ++i)
		delete _guiSliders[i];
	for (uint i = 0; i < _guiTextBoxes.size(); ++i)
		delete _guiTextBoxes[i];
	for (uint i = 0; i < _guiListBoxes.size(); ++i)
		delete _guiListBoxes[i];

	for (uint i = 0; i < _guiGroups.size(); ++i) {
		assert(_guiGroups[i]->getRefCount() == 1);
		_guiGroups[i]->DecRef();
	}

	for (uint i = 0; i < _interactionsChar.size(); ++i)
		delete _interactionsChar[i];
	for (uint i = 0; i < _interactionsInv.size(); ++i)
		delete _interactionsInv[i];
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

bool GameFile::init() {
	Common::SeekableReadStream *dta = _vm->getFile(kGameDataNameV2);
	if (!dta)
		dta = _vm->getFile(kGameDataNameV3);
	if (!dta)
		error("can't find AGS data file");

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
		_paletteUses[i] = (uint8) dta->readByte();
	for (uint32 i = 0; i < 256; i++) {
		_defaultPalette[3 * i + 0] = dta->readByte(); // R
		_defaultPalette[3 * i + 1] = dta->readByte(); // G
		_defaultPalette[3 * i + 2] = dta->readByte(); // B

		dta->skip(1); // Pad
	}

	uint32 viewCount = dta->readUint32LE();
	uint32 charCount = dta->readUint32LE();
	_playerChar = dta->readUint32LE();

	_totalScore = dta->readSint32LE();

	uint32 invItemCount = dta->readUint16LE();
	dta->skip(2); // padding

	_dialogCount = dta->readUint32LE();
	_dlgMsgCount = dta->readUint32LE();
	uint32 fontCount = dta->readUint32LE();

	debug(2, "%d views, %d characters (player is %d), %d inventory items, %d dialog topics",
		viewCount, charCount, _playerChar, invItemCount, _dialogCount);

	_colorDepth = dta->readUint32LE();

	_targetWin = dta->readUint32LE();
	_dialogBullet = dta->readUint32LE();

	_hotDot = dta->readUint16LE();
	_hotDotOuter = dta->readUint16LE();

	_uniqueID = dta->readUint32LE();
	debug(4, "game ID: %d", _uniqueID);

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
		// 3.x Windows Game Explorer stuff
		dta->skip(40); // guid
		dta->skip(20); // saved game file extension
		dta->skip(50); // saved game folder name
	}

	// fonts
	_fonts.resize(fontCount);
	for (uint32 i = 0; i < fontCount; ++i)
		_fonts[i]._flags = dta->readByte();
	for (uint32 i = 0; i < fontCount; ++i)
		_fonts[i]._outline = dta->readByte();

	// TODO: PSP version fixes up fontOutlines here...

	// sprite flags
	uint32 spriteFlagCount = dta->readUint32LE();
	_spriteFlags.resize(spriteFlagCount);
	for (uint32 i = 0; i < spriteFlagCount; ++i)
		_spriteFlags[i] = dta->readByte();

	// inventory info
	_invItemInfo.resize(invItemCount);
	for (uint32 i = 0; i < invItemCount; ++i) {
		InventoryItem &info = _invItemInfo[i];

		char invName[26];
		dta->read(invName, 25);
		invName[25] = '\0';
		info._name = invName;
		dta->skip(3); // padding

		info._pic = dta->readUint32LE();
		info._cursorPic = dta->readUint32LE();
		info._hotspotX = dta->readUint32LE();
		info._hotspotY = dta->readUint32LE();
		dta->skip(5 * 4); // reserved
		info._flags = dta->readByte();
		dta->skip(3); // padding
	}

	// cursors
	_cursors.resize(_cursorCount);
	for (uint32 i = 0; i < _cursorCount; ++i) {
		MouseCursor &cursor = _cursors[i];

		cursor._pic = dta->readUint32LE();
		cursor._hotspotX = dta->readUint16LE();
		cursor._hotspotY = dta->readUint16LE();
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
		// 3.x interaction scripts
		debug(6, "reading char interaction scripts");
		_charInteractionScripts.resize(charCount);
		for (uint i = 0; i < _charInteractionScripts.size(); ++i)
			_charInteractionScripts[i].readFrom(dta);
		debug(6, "reading inv interaction scripts");
		_invInteractionScripts.resize(invItemCount);
		for (uint i = 1; i < _invInteractionScripts.size(); ++i)
			_invInteractionScripts[i].readFrom(dta);
	} else {
		// 2.5+ new interactions
		debug(6, "reading char interactions");
		_interactionsChar.resize(charCount);
		for (uint32 i = 0; i < charCount; i++)
			_interactionsChar[i] = NewInteraction::createFrom(dta);
		debug(6, "reading inv interactions");
		_interactionsInv.resize(invItemCount);
		for (uint32 i = 0; i < invItemCount; i++)
			_interactionsInv[i] = NewInteraction::createFrom(dta);

		uint32 globalVarsCount = dta->readUint32LE();
		_globalVars.resize(globalVarsCount);
		for (uint32 i = 0; i < globalVarsCount; ++i) {
			_globalVars[i].readFrom(dta);
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
		if (scriptModuleCount > MAX_SCRIPT_MODULES)
			error("too many script modules (%d)", scriptModuleCount);

		_scriptModules.resize(scriptModuleCount);
		for (uint i = 0; i < scriptModuleCount; ++i) {
			_scriptModules[i] = new ccScript();
			_scriptModules[i]->readFrom(dta);
		}
	}

	if (_version > kAGSVer272) {
		// 3.x views
		_views.resize(viewCount);
		for (uint i = 0; i < _views.size(); ++i) {
			uint16 loopCount = dta->readUint16LE();
			_views[i]._loops.resize(loopCount);
			for (uint j = 0; j < _views[i]._loops.size(); ++j) {
				ViewLoopNew &loop = _views[i]._loops[j];

				uint16 frameCount = dta->readUint16LE();
				loop._flags = dta->readUint32LE();
				loop._frames.resize(frameCount);
				for (uint n = 0; n < frameCount; ++n)
					loop._frames[n] = readViewFrame(dta);
			}
		}
	} else {
		// 2.x views
		_views.resize(viewCount);
		readOldViews(dta);
	}

	_vm->_characters.resize(charCount);
	for (uint i = 0; i < charCount; ++i) {
		Character *chr = readCharacter(dta);
		chr->_indexId = i;
		_vm->_characters[i] = chr;
	}

	if (_version <= kAGSVer272) {
		// fixup character script names for 2.x (EGO -> cEgo)

		for (uint i = 0; i < _vm->_characters.size(); ++i) {
			if (_vm->_characters[i]->_scriptName.empty())
				continue;
			_vm->_characters[i]->_scriptName.toLowercase();
			_vm->_characters[i]->_scriptName.setChar(toupper(_vm->_characters[i]->_scriptName[0]), 0);
			_vm->_characters[i]->_scriptName.insertChar('c', 0);
		}
	}

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
		/* bool hasScripts = (bool)*/dta->readUint32LE();
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
		uint32 magic = dta->readUint32LE();
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
		_vm->_audio->initFrom(dta);
		_vm->_state->_scoreSound = dta->readUint32LE();
	} else {
		_vm->_audio->init();
		if (_options[OPT_SCORESOUND] > 0) {
			AudioClip *clip = _vm->_audio->getClipByIndex(false, _options[OPT_SCORESOUND]);
			if (clip)
				_vm->_state->_scoreSound = clip->_id;
		}
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

	debug(5, "game file read %d of %d", dta->pos(), dta->size());

	delete dta;

	return true;
}

void InteractionVariable::readFrom(Common::SeekableReadStream *dta) {
	char varName[24];
	dta->read(varName, 23);
	varName[23] = '\0';
	_name = varName;

	_type = dta->readByte();
	_value = dta->readSint32LE();
}

#define MAX_NEWINTERACTION_EVENTS 30

NewInteraction *NewInteraction::createFrom(Common::SeekableReadStream *dta) {
	uint32 unknown = dta->readUint32LE();
	if (unknown != 1) {
		if (unknown != 0)
			error("invalid interaction? (%d)", unknown);
		return NULL;
	}

	NewInteraction *interaction = new NewInteraction();

	uint32 numEvents = dta->readUint32LE();
	if (numEvents > MAX_NEWINTERACTION_EVENTS)
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

		debug(8, "reading NewInteraction response (event %d)", i);
		interaction->_events[i]._response = interaction->readCommandList(dta);
	}

	return interaction;
}

NewInteraction::~NewInteraction() {
	for (uint i = 0; i < _events.size(); ++i)
		delete _events[i]._response;
}

#define MAX_ACTION_ARGS 5

NewInteractionCommandList *NewInteraction::readCommandList(Common::SeekableReadStream *dta) {
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

void InteractionScript::readFrom(Common::SeekableReadStream *dta) {
	uint32 eventCount = dta->readUint32LE();

	_eventScriptNames.resize(eventCount);
	for (uint i = 0; i < eventCount; ++i) {
		_eventScriptNames[i] = readString(dta);
		debug(8, "interaction script: event %d: script '%s'", i, _eventScriptNames[i].c_str());
	}
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
	for (uint n = 0; n < _views.size(); ++n) {
		// read the old 2.x view data
		uint16 numLoops = dta->readUint16LE();
		uint16 numFrames[16];
		for (uint i = 0; i < 16; ++i) {
			numFrames[i] = dta->readUint16LE();
			if (numFrames[i] > 20)
				error("too many frames (%d) in 2.x view", numFrames[i]);
		}
		dta->skip(2); // padding
		// FIXME: loopFlags are discarded?
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

Character *GameFile::readCharacter(Common::SeekableReadStream *dta) {
	Character *chr = new Character(_vm);

	chr->_defView = dta->readSint32LE();
	chr->_talkView = dta->readSint32LE();
	chr->_view = dta->readSint32LE();
	chr->_room = dta->readUint32LE();
	/*chr->_prevRoom = */ dta->readUint32LE();
	chr->_x = dta->readUint32LE();
	chr->_y = dta->readUint32LE();
	chr->_wait = dta->readUint32LE();
	chr->_flags = dta->readUint32LE();
	chr->_following = dta->readSint16LE();
	chr->_followInfo = dta->readUint16LE();
	chr->_idleView = dta->readSint32LE();
	chr->_idleTime = dta->readUint16LE();
	chr->_idleLeft = dta->readUint16LE();
	chr->_transparency = dta->readUint16LE();
	chr->_baseLine = dta->readUint16LE();
	chr->_activeInv = dta->readUint32LE();
	chr->_talkColor = dta->readUint32LE();
	chr->_thinkView = dta->readUint32LE();
	chr->_blinkView = dta->readUint16LE();
	/*chr->_blinkInterval = */ dta->readUint16LE();
	/*chr->_blinkTimer = */ dta->readUint16LE();
	chr->_blinkFrame = dta->readUint16LE();
	chr->_walkSpeedY = dta->readUint16LE();
	/*chr->_picYOffs = */ dta->readUint16LE();
	chr->_z = dta->readUint32LE();
	/*chr->_walkWait = */ dta->readUint32LE();
	chr->_speechAnimSpeed = dta->readUint16LE();
	dta->skip(2); // reserved
	/*chr->_blockingWidth = */ dta->readUint16LE();
	/*chr->_blockingHeight = */ dta->readUint16LE();
	chr->_indexId = dta->readUint32LE();
	/*chr->_picXOffs = */ dta->readUint16LE();
	chr->_walkWaitCounter = dta->readUint16LE();
	/*chr->_loop = */ dta->readUint16LE();
	/*chr->_frame = */ dta->readUint16LE();
	/*chr->_walking = */ dta->readUint16LE();
	/*chr->_animating = */ dta->readUint16LE();
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
		_guiGroups[i] = new GUIGroup(_vm);
		GUIGroup &group = *_guiGroups[i];

		dta->read(group._vText, 4);

		char name[16 + 1];
		dta->read(name, 16);
		name[16] = '\0';
		group._name = name;
		if (_version <= kAGSVer272 && !group._name.empty()) {
			// Fix names for 2.x: "GUI" -> "gGui"
			group._name.toLowercase();
			group._name.setChar(toupper(group._name[0]), 0);
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
		if (objectsCount > MAX_OBJS_ON_GUI)
			error("too many GUI controls (%d)", objectsCount);
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

		// This is overwritten later in the init code.
		// (And we want it to default to not visible, for internal consistency.)
		//group._on = dta->readUint32LE();
		dta->skip(4);
		group._enabled = true;
		group._visible = false;

		dta->skip(MAX_OBJS_ON_GUI * 4); // obj pointers
		group._controlRefPtrs.resize(MAX_OBJS_ON_GUI);
		for (uint j = 0; j < MAX_OBJS_ON_GUI; ++j)
			group._controlRefPtrs[j] = dta->readUint32LE();

		// fixes/upgrades
		if (group._height < 2)
			group._height = 2;
		if (_guiVersion < 103)
			group._name = Common::String::format("GUI%d", i);
		if (_guiVersion < 105)
			group._zorder = i;
		group._id = i;

		group._controls.resize(objectsCount);
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

	for (uint n = 0; n < _guiGroups.size(); ++n) {
		GUIGroup &group = *_guiGroups[n];

		// set up the reverse-lookup array
		for (uint i = 0; i < group._controls.size(); ++i) {
			uint16 type = (group._controlRefPtrs[i] >> 16) & 0xffff;
			uint16 id = group._controlRefPtrs[i] & 0xffff;

			// TODO: bounds-checking?
			switch (type) {
			case GOBJ_BUTTON:
				group._controls[i] = _guiButtons[id];
				break;
			case GOBJ_LABEL:
				group._controls[i] = _guiLabels[id];
				break;
			case GOBJ_INVENTORY:
				group._controls[i] = _guiInvControls[id];
				break;
			case GOBJ_SLIDER:
				group._controls[i] = _guiSliders[id];
				break;
			case GOBJ_TEXTBOX:
				group._controls[i] = _guiTextBoxes[id];
				break;
			case GOBJ_LISTBOX:
				group._controls[i] = _guiListBoxes[id];
				break;
			default:
				error("invalid GUI control type %d", type);
			}

			group._controls[i]->_parent = &group;
			group._controls[i]->_id = i;
		}
	}

	for (uint n = 0; n < _guiGroups.size(); ++n)
		_guiGroups[n]->sortControls();
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
	for (uint i = 0; i < _vm->_characters.size(); ++i) {
		readProperties(dta, _vm->_characters[i]->_properties);
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
