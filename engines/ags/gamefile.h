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

#ifndef AGS_GAMEFILE_H
#define AGS_GAMEFILE_H

#include "common/array.h"
#include "common/hash-str.h"

#include "engines/ags/inventory.h"

namespace AGS {

class GUIGroup;
class GUIButton;
class GUILabel;
class GUIInvControl;
class GUISlider;
class GUITextBox;
class GUIListBox;
class ResourceManager;

struct MouseCursor {
	uint32 _pic;
	uint16 _hotspotX, _hotspotY;
	int16 _view;
	Common::String _name;
	uint8 _flags;
};

struct InteractionVariable {
	void readFrom(Common::SeekableReadStream *dta);

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
	NewInteractionEvent() : _response(NULL) { }

	uint32 _type;
	uint32 _timesRun;
	NewInteractionCommandList *_response;
};

struct NewInteraction {
	static NewInteraction *createFrom(Common::SeekableReadStream *dta);
	~NewInteraction();

	bool hasResponseFor(uint eventId) {
		return (eventId < _events.size()) && _events[eventId]._response && !_events[eventId]._response->_commands.empty();
	}

	Common::Array<NewInteractionEvent> _events;

protected:
	NewInteractionCommandList *readCommandList(Common::SeekableReadStream *dta);
};

struct InteractionScript {
	void readFrom(Common::SeekableReadStream *dta);

	Common::Array<Common::String> _eventScriptNames;
};

struct DictionaryWord {
	Common::String _word;
	uint16 _id;
};

struct WordsDictionary {
	Common::Array<DictionaryWord> _words;
};

struct ccScript;

class ViewFrame : public ScriptObject {
public:
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotViewFrame); }
	const char *getObjectTypeName() { return "ViewFrame"; }

	uint32 _pic;
	uint16 _xOffs, _yOffs;
	uint16 _speed;
	uint32 _flags;
	int32 _sound;
};

#define LOOPFLAG_RUNNEXTLOOP 1

struct ViewLoopNew {
	uint32 _flags;
	Common::Array<ViewFrame> _frames;

	bool shouldRunNextLoop() const { return _flags & LOOPFLAG_RUNNEXTLOOP; }
};

struct ViewStruct {
	Common::Array<ViewLoopNew> _loops;

	Common::String _name;
};

struct DialogOption {
	Common::String _name;
	uint32 _flags;
	uint16 _entryPoint;
};

struct DialogTopic : public ScriptObject {
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotDialog); }
	const char *getObjectTypeName() { return "DialogTopic"; }

	uint _id;

	Common::Array<DialogOption> _options;
	Common::Array<byte> _code;

	uint16 _startupEntryPoint;
	uint32 _flags;

	Common::String _name;
};

struct CustomPropertySchemaProperty {
	Common::String _name;
	Common::String _description;
	Common::String _defaultValue;
	uint32 _type;
};

struct AGSFont {
	byte _flags;
	signed char _outline;
};

class AGSEngine;
class Character;

class GameFile {
public:
	GameFile(AGSEngine *vm);
	~GameFile();

	bool init();
	void readProperties(Common::SeekableReadStream *dta, Common::StringMap &map);

private:
	ViewFrame readViewFrame(Common::SeekableReadStream *dta);
	void readOldViews(Common::SeekableReadStream *dta);
	Character *readCharacter(Common::SeekableReadStream *dta);
	void readGui(Common::SeekableReadStream *dta);
	void readPlugins(Common::SeekableReadStream *dta);
	void readPropertyData(Common::SeekableReadStream *dta);
	void setDefaultMessages();

public:
	uint32 _version;

	Common::String _versionString;
	Common::String _gameName;

	uint32 _options[100];

	uint8 _paletteUses[256];
	byte _defaultPalette[768];

	uint32 _playerChar;

	uint32 _totalScore;

	Common::Array<InventoryItem> _invItemInfo;

	uint32 _dialogCount;
	uint32 _dlgMsgCount;

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

	Common::Array<AGSFont> _fonts;

	Common::Array<byte> _spriteFlags;

	Common::Array<InteractionScript> _charInteractionScripts;
	Common::Array<InteractionScript> _invInteractionScripts;

	Common::Array<NewInteraction *> _interactionsChar;
	Common::Array<NewInteraction *> _interactionsInv;
	Common::Array<InteractionVariable> _globalVars;

	WordsDictionary _dict;

	ccScript *_gameScript;
	ccScript *_dialogScriptsScript;
	Common::Array<ccScript *> _scriptModules;

	Common::Array<ViewStruct> _views;

	Common::Array<Common::String> _messages;

	Common::Array<DialogTopic> _dialogs;
	Common::Array<Common::String> _speechLines;

	uint32 _guiVersion;
	Common::Array<GUIGroup *> _guiGroups;
	Common::Array<GUIGroup *> _guiGroupDrawOrder;

	Common::Array<GUIButton *> _guiButtons;
	Common::Array<GUILabel *> _guiLabels;
	Common::Array<GUIInvControl *> _guiInvControls;
	Common::Array<GUISlider *> _guiSliders;
	Common::Array<GUITextBox *> _guiTextBoxes;
	Common::Array<GUIListBox *> _guiListBoxes;

	Common::Array<CustomPropertySchemaProperty> _schemaProperties;

private:
	AGSEngine *_vm;

	void readVersion(Common::SeekableReadStream &dta);
};

} // End of namespace AGS

#endif // AGS_GAMEFILE_H
