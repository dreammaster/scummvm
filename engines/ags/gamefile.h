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

#ifndef AGS_GAMEFILE_H
#define AGS_GAMEFILE_H

#include "common/array.h"

namespace AGS {

class ResourceManager;

struct InventoryItemInfo {
	Common::String _name;
	uint32 _pic;
	uint32 _cursorPic;
	uint32 _hotX, _hotY;
	uint8 _flags;
};

struct MouseCursor {
	uint32 _pic;
	uint16 _hotX, _hotY;
	int16 _view;
	Common::String _name;
	uint8 _flags;
};

struct InteractionVariable {
	Common::String _name;
	byte _type;
	int32 _value;
};

struct NewInteractionValue {
	byte _type;
	uint32 _val;
	uint32 _extra;
};

struct NewInteractionAction {
};

struct NewInteractionCommand : public NewInteractionAction {
	uint32 _type;
	Common::Array<NewInteractionValue> _args;
	NewInteractionAction *_children;
	struct NewInteractionCommandList *_parent;
};

struct NewInteractionCommandList : public NewInteractionAction {
	Common::Array<NewInteractionCommand> _commands;
	uint32 _timesRun;
};

struct NewInteractionEvent {
	uint32 _type;
	uint32 _timesRun;
	NewInteractionCommandList *_response;
};

struct NewInteraction {
	Common::Array<NewInteractionEvent> _events;
};

struct DictionaryWord {
	Common::String _word;
	uint16 _id;
};

struct WordsDictionary {
	Common::Array<DictionaryWord> _words;
};

struct ccScript;

struct ViewFrame {
	uint32 _pic;
	uint16 _xOffs, _yOffs;
	uint16 _speed;
	uint32 _flags;
	uint32 _sound;
};

struct ViewLoopNew {
	uint32 _flags;
	Common::Array<ViewFrame> _frames;
};

struct ViewStruct {
	Common::Array<ViewLoopNew> _loops;
};

struct CharacterInfo {
	uint32 _defView, _talkView, _view;
	uint32 _room, _prevRoom;

	int32 _x, _y;

	uint32 _wait;
	uint32 _flags;

	uint16 _following;
	uint16 _followInfo;

	uint32 _idleView; // the loop will be randomly picked
	uint16 _idleTime, _idleLeft; // num seconds idle before playing anim

	uint16 _transparency; // if character is transparent
	uint16 _baseLine;

	uint32 _activeInv;
	uint32 _talkColor;
	uint32 _thinkView;

	uint16 _blinkView, _blinkInterval; // design time
	uint16 _blinkTimer, _blinkFrame; // run time

	uint16 _walkSpeedY, _picYOffs;

	int32 _z;

	uint32 _walkWait;

	uint16 _speechAnimSpeed;
	uint16 _blockingWidth, _blockingHeight;

	uint32 _indexId; // used for object functions to know the id

	uint16 _picXOffs;
	uint16 _walkWaitCounter;

	uint16 _loop;
	uint16 _frame;

	uint16 _walking;
	uint16 _animating;

	uint16 _walkSpeed, _animSpeed;

	Common::Array<uint16> _inventory;

	int16 _actX, _actY;

	Common::String _name;
	Common::String _scriptName;

	byte _on;
};

struct DialogOption {
	Common::String _name;
	uint32 _flags;
	uint16 _entryPoint;
};

struct DialogTopic {
	Common::Array<DialogOption> _options;
	Common::Array<byte> _code;

	uint16 _startupEntryPoint;
	uint32 _flags;
};

class GameFile {
public:
	GameFile();
	~GameFile();

	bool init(const ResourceManager &resMan);

private:
	InteractionVariable readInteractionVariable(Common::SeekableReadStream *dta);
	NewInteraction *readNewInteraction(Common::SeekableReadStream *dta);
	NewInteractionCommandList *readCommandList(Common::SeekableReadStream *dta);
	ViewFrame readViewFrame(Common::SeekableReadStream *dta);
	void readOldViews(Common::SeekableReadStream *dta);
	CharacterInfo *readCharacter(Common::SeekableReadStream *dta);
	void setDefaultMessages();

public:
	uint32 _version;

	Common::String _versionString;
	Common::String _gameName;

	uint32 _options[100];

	uint8 _palUses[256];
	byte  _defPal[768];

	uint32 _viewCount;
	uint32 _charCount;
	uint32 _playerChars;

	uint32 _totalScore;

	uint16 _invItemCount;
	Common::Array<InventoryItemInfo> _invItemInfo;

	uint32 _dialogCount;
	uint32 _dlgMsgCount;

	uint32 _fontCount;

	uint32 _colorDepth;

	uint32 _targetWin;
	uint32 _dialogBullet;

	uint16 _hotDot;
	uint16 _hotDotOuter;

	uint32 _uniqueID;

	uint32 _guiCount;
	uint32 _cursorCount;
	Common::Array<MouseCursor> _cursors;

	uint32 _defaultResolution;
	uint32 _defaultLipSyncFrame;

	uint32 _invHotDotSprite;

	uint32 _globalScript;
	uint32 _compiledScript;

	Common::Array<byte> _fontFlags;
	Common::Array<byte> _fontOutline;

	Common::Array<byte> _spriteFlags;

	Common::Array<NewInteraction *> _interactionsChar;
	Common::Array<NewInteraction *> _interactionsInv;
	Common::Array<InteractionVariable> _globalVars;

	WordsDictionary _dict;

	ccScript *_gameScript;
	ccScript *_dialogScriptsScript;
	Common::Array<ccScript *> _scriptModules;

	Common::Array<ViewStruct> _views;

	Common::Array<Common::String> _messages;

	Common::Array<CharacterInfo *> _chars;

	Common::Array<DialogTopic> _dialogs;
	Common::Array<Common::String> _speechLines;

private:
	void readVersion(Common::SeekableReadStream &dta);
	void decryptText(uint8 *str, uint32 max);
	Common::String decryptString(Common::SeekableReadStream *dta);
};

} // End of namespace AGS

#endif // AGS_GAMEFILE_H
