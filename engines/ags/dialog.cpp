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

#include "engines/ags/ags.h"
#include "engines/ags/audio.h"
#include "engines/ags/character.h"
#include "engines/ags/constants.h"
#include "engines/ags/drawable.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/gui.h"
#include "engines/ags/sprites.h"

#include "common/events.h"
#include "common/stack.h"
#include "graphics/font.h"

namespace AGS {

class DialogOptionsDrawable : public Drawable {
public:
	DialogOptionsDrawable(AGSEngine *vm, DialogTopic *topic, Common::Array<uint> &displayedOptions);
	~DialogOptionsDrawable();

	virtual Common::Point getDrawPos() { return _pos; }
	virtual int getDrawOrder() const { return 0; }
	virtual uint getDrawWidth() { return _surface.w; }
	virtual uint getDrawHeight() { return _surface.h; }

	virtual const Graphics::Surface *getDrawSurface() { return &_surface; }

	virtual uint getDrawTransparency() { return 0; }
	virtual bool isDrawMirrored() { return 0; }
	virtual bool isDrawAlpha() { return true; }
	virtual int getDrawLightLevel() { return 0; }
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) { }

	uint _selected;
	Common::Array<uint> _yPositions;
	uint _optionsHeight;

	void invalidate();

protected:
	AGSEngine *_vm;

	Common::Point _pos;
	Graphics::Surface _surface;

	bool _usingCustomRendering;
	bool _isTextWindow;

	uint32 _fgColor;

	Graphics::Font *_font;
	uint _textAreaWidth;
	uint _bulletWidth;

	DialogTopic *_topic;
	Common::Array<uint> &_displayedOptions;
	uint getOptionsHeight();

	void drawDialogOptions();
};

DialogOptionsDrawable::DialogOptionsDrawable(AGSEngine *vm, DialogTopic *topic, Common::Array<uint> &displayedOptions) : _vm(vm),
	_topic(topic), _displayedOptions(displayedOptions) {

	_selected = (uint)-1;
	_usingCustomRendering = false;
	_isTextWindow = false;
	_fgColor = 14;

	_pos.x = 1;
	_pos.y = _vm->getFixedPixelSize(160);

	_font = _vm->_graphics->getFont(_vm->_state->_normalFont);

	_bulletWidth = 0;
	// is there a bullet sprite? then add that width, plus some spacing
	if (_vm->_gameFile->_dialogBullet)
		_bulletWidth += _vm->getSprites()->getSpriteWidth(_vm->_gameFile->_dialogBullet) + 3;
	// are there numbered options? then leave space for those
	// FIXME: wrong text width
	if (_vm->getGameOption(OPT_DIALOGNUMBERED))
		_bulletWidth += _font->getStringWidth("9. ");

	uint width = _vm->_graphics->_width, height = vm->_graphics->_height;

	// FIXME: custom rendering check
	if (_vm->getGameOption(OPT_DIALOGIFACE)) {
		GUIGroup *group = _vm->_gameFile->_guiGroups[_vm->getGameOption(OPT_DIALOGIFACE)];

		if (group->isTextWindow()) {
			_isTextWindow = true;
			_fgColor = group->_fgColor;

			_textAreaWidth = _vm->multiplyUpCoordinate(_vm->_state->_maxDialogOptionWidth);
			_optionsHeight = getOptionsHeight();

			uint longestLine = 0;
			for (uint i = 0; i < _displayedOptions.size(); ++i) {
				DialogOption &option = _topic->_options[_displayedOptions[i]];

				Common::Array<Common::String> lines;
				_font->wordWrapText(option._name, _textAreaWidth - _bulletWidth + 8, lines);

				for (uint j = 0; j < lines.size(); ++j) {
					uint lineWidth = _font->getStringWidth(lines[j]) + 12 + _bulletWidth;
					if (lineWidth > longestLine)
						longestLine = lineWidth;
				}
			}

			if (longestLine < _textAreaWidth) {
				// Shrink the size to the smallest size needed.
				_textAreaWidth = longestLine;

				// But make sure we don't go below the minimum.
				if (_textAreaWidth < (uint)_vm->multiplyUpCoordinate(_vm->_state->_minDialogOptionWidth))
					_textAreaWidth = _vm->multiplyUpCoordinate(_vm->_state->_minDialogOptionWidth);
				// TODO: Original sanity-checks min <= max here, we should do that elsewhere.
			}

			// FIXME
			warning("unimplemented: text window dialog");
		} else {
			_pos.x = group->_x;
			_pos.y = group->_y;

			// FIXME: sabotage original GUI

			width = group->_width;
			height = group->_height;
			_textAreaWidth = width - 5;
			_optionsHeight = getOptionsHeight();

			if (_vm->getGameOption(OPT_DIALOGUPWARDS))
				_pos.y = group->_y + group->_height - getOptionsHeight();
		}
	} else {
		_textAreaWidth = width - 5;

		// FIXME
		warning("unimplemented: standard dialog");
	}

	if (!_isTextWindow)
		_textAreaWidth -= _vm->multiplyUpCoordinate(_vm->_state->_dialogOptionsX) * 2;

	_surface.create(width, height, vm->_graphics->getPixelFormat());

	invalidate();
}

DialogOptionsDrawable::~DialogOptionsDrawable() {
	_surface.free();
}

uint DialogOptionsDrawable::getOptionsHeight() {
	uint height = 0;

	for (uint i = 0; i < _displayedOptions.size(); ++i) {
		DialogOption &option = _topic->_options[_displayedOptions[i]];

		Common::Array<Common::String> lines;
		_font->wordWrapText(option._name, _textAreaWidth - _bulletWidth + 8, lines);

		// FIXME: right height?
		height += lines.size() * (_font->getFontHeight() + 1);
		height += _vm->multiplyUpCoordinate(_vm->getGameOption(OPT_DIALOGGAP));
	}

	// FIXME: parser input

	return height;
}

void DialogOptionsDrawable::invalidate() {
	uint32 bgColor = _vm->_graphics->getTransparentColor();
	_surface.fillRect(Common::Rect(0, 0, _surface.w, _surface.h), bgColor);

	// FIXME

	drawDialogOptions();
}

void DialogOptionsDrawable::drawDialogOptions() {
	_yPositions.clear();

	Sprite *bullet = NULL;
	if (_vm->_gameFile->_dialogBullet)
		bullet = _vm->getSprites()->getSprite(_vm->_gameFile->_dialogBullet);

	// FIXME: various things (offsets, etc)
	uint yPos = 0;
	for (uint i = 0; i < _displayedOptions.size(); ++i) {
		DialogOption &option = _topic->_options[_displayedOptions[i]];

		uint32 textColor = _vm->getPlayerChar()->_talkColor;
		if ((_vm->_state->_readDialogOptionColor != (uint)-1) && (option._flags & DFLG_HASBEENCHOSEN))
			textColor = _vm->_state->_readDialogOptionColor;

		if (_selected == i) {
			// If the normal color is the same as the highlight color, use 13 instead.
			if (textColor == _fgColor)
				textColor = 13;
			else
				textColor = _fgColor;
		}
		textColor = _vm->_graphics->resolveHardcodedColor(textColor);

		if (bullet)
			_vm->_graphics->blit(bullet->_surface, &_surface, Common::Point(0, yPos), 0);

		if (_vm->getGameOption(OPT_DIALOGNUMBERED)) {
			uint xPos = 0;
			if (bullet)
				xPos = _vm->getSprites()->getSpriteWidth(_vm->_gameFile->_dialogBullet) + 3;
			_vm->_graphics->drawOutlinedString(_vm->_state->_normalFont, &_surface, Common::String::format("%d.", i + 1),
				xPos, yPos, _textAreaWidth, textColor);
		}

		_yPositions.push_back(yPos);

		Common::Array<Common::String> lines;
		_font->wordWrapText(option._name, _textAreaWidth - _bulletWidth + 8, lines);
		uint xPos = _bulletWidth;
		for (uint j = 0; j < lines.size(); ++j) {
			// Draw the lines for this option, indenting the first one.
			_vm->_graphics->drawOutlinedString(_vm->_state->_normalFont, &_surface, lines[j],
				xPos + (j == 0 ? 0 : 9), yPos, _textAreaWidth - _bulletWidth + 8, textColor);
			// FIXME: right height?
			yPos += _font->getFontHeight() + 1;
		}

		if (i + 1 != _displayedOptions.size())
			yPos += _vm->multiplyUpCoordinate(_vm->getGameOption(OPT_DIALOGGAP));
	}

	//_optionsHeight = 0; // FIXME
}

#define CHOSE_TEXTPARSER -3053
#define SAYCHOSEN_USEFLAG 1
#define SAYCHOSEN_YES 2
#define SAYCHOSEN_NO  3

#define RUN_DIALOG_STAY          -1
#define RUN_DIALOG_STOP_DIALOG   -2
#define RUN_DIALOG_GOTO_PREVIOUS -4

void AGSEngine::runDialogId(uint dialogId) {
	if (dialogId >= _gameFile->_dialogs.size())
		error("runDialogId: dialog %d invalid (only have %d dialogs)", dialogId, _gameFile->_dialogs.size());

	// FIXME: can_run_delayed_command

	if (_state->_stopDialogAtEnd != DIALOG_NONE) {
		if (_state->_stopDialogAtEnd != DIALOG_RUNNING)
			error("runDialogId: already-running dialog was in state %d", _state->_stopDialogAtEnd);
		_state->_stopDialogAtEnd = DIALOG_NEWTOPIC + dialogId;
		return;
	}

	if (_runningScripts.size())
		_runningScripts.back().queueAction(kPSARunDialog, dialogId, "RunDialog");
	else
		doConversation(dialogId);
}

int AGSEngine::showDialogOptions(uint dialogId, uint sayChosenOption) {
	if (dialogId >= _gameFile->_dialogs.size())
		error("showDialogOptions: dialog %d invalid (only have %d dialogs)", dialogId, _gameFile->_dialogs.size());

	// FIXME: can_run_delayed_command

	_state->_inConversation++;

	// FIXME: _saidText = 0;

	DialogTopic *topic = &_gameFile->_dialogs[dialogId];

	bool parserEnabled = false;
	if ((topic->_flags & DTFLG_SHOWPARSER) && !_state->_disableDialogParser) {
		// FIXME
		error("runConversation: parser unimplemented");
	}

	// Work out which dialog options are to be displayed.
	Common::Array<uint> displayedOptions;
	for (uint i = 0; i < topic->_options.size(); ++i) {
		if (!(topic->_options[i]._flags & DFLG_ON))
			continue;

		// FIXME: ensure_text_valid_for_font?
		displayedOptions.push_back(i);
	}
	if (displayedOptions.empty())
		error("runConversation: no enabled options in dialog topic %d", dialogId);

	int chosenOption;
	if (displayedOptions.size() == 1 && !parserEnabled && !_state->_showSingleDialogOption) {
		// Only one choice, so just use that one.
		chosenOption = displayedOptions[0];
	} else {
		// Multiple choices, so we need to display the GUI.

		uint cursorWas = _graphics->getCurrentCursor();
		_graphics->setMouseCursor(CURS_ARROW);

		DialogOptionsDrawable drawable(this, topic, displayedOptions);
		_graphics->setExtraDrawable(&drawable);

		while (!shouldQuit()) {
			if ((bool)getGameOption(OPT_RUNGAMEDLGOPTS)) {
				_state->_disabledUserInterface++;
				// FIXME: pass alternative display info
				tickGame(false);
				_state->_disabledUserInterface--;
			} else {
				_state->_gameStep++;
				// FIXME: rendering/polling stuff
				updateEvents(false);
				draw();
			}

			// FIXME: something less stupid :P
			Common::Point mousePos = _system->getEventManager()->getMousePos() - Common::Point(0, drawable.getDrawPos().y);
			uint selectedWas = drawable._selected;
			drawable._selected = (uint)-1;
			if (mousePos.y >= 0 && mousePos.y <= (int)drawable._optionsHeight) {
				drawable._selected = displayedOptions.size() - 1;
				for (uint i = 0; i < displayedOptions.size() - 1; ++i) {
					if ((uint)mousePos.y >= drawable._yPositions[i + 1])
						continue;
					drawable._selected = i;
					break;
				}
			}
			if (drawable._selected != selectedWas)
				drawable.invalidate();

			if (drawable._selected != (uint)-1 && _system->getEventManager()->getButtonState()) {
				chosenOption = displayedOptions[drawable._selected];
				break;
			}
		}

		_graphics->setExtraDrawable(NULL);
		_graphics->setMouseCursor(cursorWas);

		// In case it's the QFG4 style dialog, remove the black screen
		removeScreenOverlay(OVER_COMPLETE);
	}

	_state->_inConversation--;

	if (chosenOption != CHOSE_TEXTPARSER) {
		DialogOption &option = topic->_options[chosenOption];

		option._flags |= DFLG_HASBEENCHOSEN;

		bool sayTheOption = (sayChosenOption == SAYCHOSEN_YES);
		if (sayChosenOption == SAYCHOSEN_USEFLAG)
			sayTheOption = !(option._flags & DFLG_NOREPEAT);

		if (sayTheOption)
			displaySpeech(getTranslation(option._name), _gameFile->_playerChar);
	}

	return chosenOption;
}

void AGSEngine::doConversation(uint dialogId) {
	endSkippingUntilCharStops();

	// JJS: AGS 2.x always makes the mouse cursor visible when displaying a dialog.
	// "Fixed invisible mouse cursor in dialogs with some version 2.x games, e.g. Murder in a Wheel."
	if (getGameFileVersion() <= kAGSVer272)
		_state->_mouseCursorHidden = 0;

	Common::Stack<uint> previousTopics;
	uint currDialogId = dialogId;
	DialogTopic &currDialogTopic = _gameFile->_dialogs[dialogId];

	// run the dialog startup script
	int result = runDialogScript(currDialogTopic, currDialogId, currDialogTopic._startupEntryPoint, 0);
	if ((result == RUN_DIALOG_STOP_DIALOG) || (result == RUN_DIALOG_GOTO_PREVIOUS)) {
		// 'stop' or 'goto-previous' from startup script
		removeScreenOverlay(OVER_COMPLETE);
		_state->_inConversation--;
		return;
	} else if (result >= 0) {
		currDialogId = (uint)result;
	}

	while (result != RUN_DIALOG_STOP_DIALOG && !shouldQuit()) {
		if (currDialogId >= _gameFile->_dialogs.size())
			error("doConversation: new dialog was too high (%d), only have %d dialogs",
				currDialogId, _gameFile->_dialogs.size());

		debugC(kDebugLevelGame, "running conversation topic %d", currDialogId);

		currDialogTopic = _gameFile->_dialogs[currDialogId];

		if (currDialogId != dialogId) {
			// dialog topic changed, so play the startup script for the new topic
			debugC(2, kDebugLevelGame, "changed from topic %d to topic %d", dialogId, currDialogId);
			dialogId = currDialogId;
			result = runDialogScript(currDialogTopic, currDialogId, currDialogTopic._startupEntryPoint, 0);
		} else {
			int chose = showDialogOptions(currDialogId, SAYCHOSEN_USEFLAG);

			if (chose == CHOSE_TEXTPARSER) {
				_saidSpeechLine = false;
				result = runDialogRequest(currDialogId);
				if (_saidSpeechLine) {
					// FIXME: original futzes with the screen for close-up face here
				}
			} else {
				result = runDialogScript(currDialogTopic, currDialogId, currDialogTopic._options[chose]._entryPoint, chose + 1);
			}
		}

		if (result == RUN_DIALOG_GOTO_PREVIOUS) {
			if (previousTopics.empty()) {
				// goto-previous on first topic -- end dialog
				debugC(2, kDebugLevelGame, "back to previous topic (none left, done)");
				result = RUN_DIALOG_STOP_DIALOG;
			} else {
				result = (int)previousTopics.pop();
				debugC(2, kDebugLevelGame, "back to previous topic %d", result);
			}
		}
		if (result >= 0) {
			// another topic change
			previousTopics.push(currDialogId);
			currDialogId = (uint)result;
		}
	}
}

// TODO: move this into DialogTopic itself?
static void getDialogScriptParameters(DialogTopic &topic, uint &pos, uint16 *param1, uint16 *param2 = NULL) {
	const Common::Array<byte> &code = topic._code;
	if (pos + 3 > code.size())
		error("getDialogScriptParameters: %d is off end of script (size %d)", pos, code.size());
	pos++;
	*param1 = READ_LE_UINT16(&code[pos]);
	pos += 2;
	if (param2) {
		if (pos + 2 > code.size())
			error("getDialogScriptParameters: %d is off end of script (size %d)", pos, code.size());
		*param2 = READ_LE_UINT16(&code[pos]);
		pos += 2;
	}
}

int AGSEngine::runDialogScript(DialogTopic &topic, uint dialogId, uint16 offset, uint optionId) {
	_saidSpeechLine = false;

	debugC(2, kDebugLevelGame, "running dialog script for option %d of dialog %d", optionId, dialogId);

	int result = RUN_DIALOG_STAY;
	if (_dialogScriptsScript) {
		Common::Array<RuntimeValue> params;
		params.push_back(optionId);
		runTextScript(_dialogScriptsScript, Common::String::format("_run_dialog%d", dialogId), params);
		result = (int)_dialogScriptsScript->getReturnValue();
	} else {
		// old-style dialog script
		if (offset == (uint16)-1)
			return result;

		uint pos = offset;
		bool scriptRunning = true;
		uint16 param1, param2;

		while (scriptRunning && !shouldQuit()) {
			if (pos + 2 > topic._code.size())
				error("runDialogScript: %d is off end of script (size %d)", pos, topic._code.size());
			byte opcode = topic._code[pos];
			switch (opcode) {
			case DCMD_SAY:
				getDialogScriptParameters(topic, pos, &param1, &param2);

				{
				if (param2 > _gameFile->_speechLines.size())
					error("DCMD_SAY: speech line %d is too high (only have %d)", param2, _gameFile->_speechLines.size());
				const Common::String &speechLine = _gameFile->_speechLines[param2];

				if (param1 == DCHAR_PLAYER)
					param1 = _gameFile->_playerChar;

				if (param1 == DCHAR_NARRATOR)
					display(getTranslation(speechLine));
				else
					displaySpeech(getTranslation(speechLine), param1);
				}

				_saidSpeechLine = true;
				break;
			case DCMD_OPTOFF:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				if (param1 >= topic._options.size())
					error("runDialogScript: DCMD_OPTOFF: option %d is too high (only have %d)",
						param1, topic._options.size());
				topic._options[param1]._flags &= ~DFLG_ON;
				break;
			case DCMD_OPTON:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				if (param1 >= topic._options.size())
					error("runDialogScript: DCMD_OPTON: option %d is too high (only have %d)",
						param1, topic._options.size());
				if (!(topic._options[param1]._flags & DFLG_OFFPERM))
					topic._options[param1]._flags |= DFLG_ON;
				break;
			case DCMD_RETURN:
				scriptRunning = false;
				break;
			case DCMD_STOPDIALOG:
				result = RUN_DIALOG_STOP_DIALOG;
				scriptRunning = false;
				break;
			case DCMD_OPTOFFFOREVER:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				if (param1 >= topic._options.size())
					error("runDialogScript: DCMD_OPTOFFFOREVER: option %d is too high (only have %d)",
						param1, topic._options.size());
				topic._options[param1]._flags &= ~DFLG_ON;
				topic._options[param1]._flags |= DFLG_OFFPERM;
				break;
			case DCMD_RUNTEXTSCRIPT:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				result = runDialogRequest(param1);
				scriptRunning = (result == RUN_DIALOG_STAY);
				break;
			case DCMD_GOTODIALOG:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				result = param1;
				scriptRunning = false;
				break;
			case DCMD_PLAYSOUND:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				_audio->playSound(param1);
				break;
			case DCMD_ADDINV:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				addInventory(param1);
				break;
			case DCMD_SETSPCHVIEW:
				getDialogScriptParameters(topic, pos, &param1, &param2);
				// FIXME
				error("DCMD_SETSPCHVIEW unimplemented");
				break;
			case DCMD_NEWROOM:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				scheduleNewRoom(param1);
				_inNewRoomState = kNewRoomStateNew;
				// JJS: "Fixed 2.xx dialogs not ending when changing the room."
				result = RUN_DIALOG_STOP_DIALOG;
				scriptRunning = false;
				break;
			case DCMD_SETGLOBALINT:
				getDialogScriptParameters(topic, pos, &param1, &param2);
				// FIXME
				error("DCMD_SETGLOBALINT unimplemented");
				break;
			case DCMD_GIVESCORE:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_GIVESCORE unimplemented");
				break;
			case DCMD_GOTOPREVIOUS:
				result = RUN_DIALOG_GOTO_PREVIOUS;
				scriptRunning = false;
				break;
			case DCMD_LOSEINV:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				loseInventory(param1);
				break;
			case DCMD_ENDSCRIPT:
				result = RUN_DIALOG_STOP_DIALOG;
				scriptRunning = false;
				break;
			default:
				error("runDialogScript: unknown opcode %d", opcode);
			}
		}
	}

	// if there was a room change, stop the dialog
	if (_inNewRoomState != kNewRoomStateNone)
		return RUN_DIALOG_STOP_DIALOG;

	if (_saidSpeechLine) {
		// FIXME: original futzes with the screen for close-up face here
		// (see doConversation also)
	}

	debugC(2, kDebugLevelGame, "finished dialog script for option %d of dialog %d, returning %d", optionId, dialogId, result);

	return result;
}

int AGSEngine::runDialogRequest(uint request) {
	_state->_stopDialogAtEnd = DIALOG_RUNNING;

	Common::Array<RuntimeValue> params;
	params.push_back(request);
	runScriptFunction(_gameScript, "dialog_request", params);

	if (_state->_stopDialogAtEnd == DIALOG_STOP) {
		_state->_stopDialogAtEnd = DIALOG_NONE;
		return RUN_DIALOG_STOP_DIALOG;
	} else if (_state->_stopDialogAtEnd >= DIALOG_NEWTOPIC) {
		uint topicId = (uint)_state->_stopDialogAtEnd - DIALOG_NEWTOPIC;
		_state->_stopDialogAtEnd = DIALOG_NONE;
		return topicId;
	} else if (_state->_stopDialogAtEnd >= DIALOG_NEWROOM) {
		uint roomId = (uint)_state->_stopDialogAtEnd - DIALOG_NEWROOM;
		_state->_stopDialogAtEnd = DIALOG_NONE;
		scheduleNewRoom(roomId);
		return RUN_DIALOG_STOP_DIALOG;
	} else {
		_state->_stopDialogAtEnd = DIALOG_NONE;
		return RUN_DIALOG_STAY;
	}
}

} // End of namespace AGS
