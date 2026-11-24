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

#include "ultima/ultima5/views/transfer_character.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int MAX_NAME_LENGTH = 8;
constexpr int FRAMES_PER_STEP = 3;
constexpr int ROW_COUNT = 8;
constexpr int FIRST_ROW = 4;
constexpr int LABEL_COLUMN = 2;
constexpr int OLD_COLUMN = 10;
constexpr int NEW_COLUMN = 28;
constexpr int NOTE_ROW = 16;
constexpr int NAME_ROW = 3;
constexpr int STEP_CLASS = 0;
constexpr int STEP_COUNT = 6;

// What each row of the comparison is called
static const char *const ROW_LABELS[ROW_COUNT] = { "Name:", "Sex:", "Class:", "Exp:", "Level:", "STR:", "DEX:", "INT:" };

bool TransferCharacter::msgFocus(const FocusMessage &msg) {
	if (!loadTitleGraphics() || !_newGame.load("init.gam") || !_newGame.loadWorldObjects("init.ool"))
		error("Could not load the character transfer");

	_step = 0;
	_name.clear();
	_cursorFrame = 0;

	switch (_logic.load()) {
	case TransferLogic::LOAD_OK:
		_phase = PHASE_FOUND;
		break;
	case TransferLogic::LOAD_BAD:
		_phase = PHASE_BAD;
		break;
	default:
		_phase = PHASE_MISSING;
		break;
	}

	delayFrames(FRAMES_PER_STEP);
	return TitleBase::msgFocus(msg);
}

void TransferCharacter::drawNote(const Common::String &text) {
	print(2, NOTE_ROW, text, COLOR_WHITE, COLOR_BLACK);
}

void TransferCharacter::drawTable() {
	const TransferLogic::Character &was = _logic._old;
	const TransferLogic::Character &now = _logic._new;
	const Common::String oldValues[ROW_COUNT] = {
		was._name, was._male ? "Male" : "Female", was._avatar ? "Avatar" : TransferLogic::className(was._class),
		Common::String::format("%d", was._experience), Common::String::format("%d", was._level),
		Common::String::format("%d", was._strength), Common::String::format("%d", was._dexterity),
		Common::String::format("%d", was._intellect)
	};
	const Common::String newValues[ROW_COUNT] = {
		now._name, now._male ? "Male" : "Female", TransferLogic::className(now._class),
		Common::String::format("%d", now._experience), Common::String::format("%d", now._level),
		Common::String::format("%d", now._strength), Common::String::format("%d", now._dexterity),
		Common::String::format("%d", now._intellect)
	};

	// How many of the rows have a new value to show
	int revealed = 0;
	switch (_phase) {
	case PHASE_KEEP_SEX:
		revealed = 1;
		break;
	case PHASE_CONVERT:
		revealed = 3 + _step;
		break;
	case PHASE_SAVED:
		revealed = ROW_COUNT;
		break;
	default:
		break;
	}

	getSurface().clear();
	print(OLD_COLUMN, 1, "Ultima IV", COLOR_WHITE, COLOR_BLACK);
	print(NEW_COLUMN - 2, 1, "Ultima V", COLOR_WHITE, COLOR_BLACK);

	for (int i = 0; i < ROW_COUNT; ++i) {
		print(LABEL_COLUMN, FIRST_ROW + i, ROW_LABELS[i], COLOR_WHITE, COLOR_BLACK);
		print(OLD_COLUMN, FIRST_ROW + i, oldValues[i], COLOR_WHITE, COLOR_BLACK);

		if (i < revealed)
			print(NEW_COLUMN - 2, FIRST_ROW + i, newValues[i], COLOR_WHITE, COLOR_BLACK);
	}
}

void TransferCharacter::draw() {
	const TransferLogic::Character &was = _logic._old;

	switch (_phase) {
	case PHASE_MISSING:
		drawEmptyMenu(0);
		print(2, 17, "Please place the Ultima IV 'party.sav'", COLOR_WHITE, COLOR_BLACK);
		print(2, 18, "file in the Ultima V game folder.", COLOR_WHITE, COLOR_BLACK);
		print(2, 20, "Press any key to return to the menu.", COLOR_WHITE, COLOR_BLACK);
		break;

	case PHASE_BAD:
		drawEmptyMenu(0);
		print(2, 17, "Error: Your Ultima IV game contains bad data.", COLOR_WHITE, COLOR_BLACK);
		print(2, 18, "Unable to continue transfer.", COLOR_WHITE, COLOR_BLACK);
		print(2, 20, "Press any key to return to the menu.", COLOR_WHITE, COLOR_BLACK);
		break;

	case PHASE_FOUND:
		getSurface().clear();
		print(14, 5, "Found:", COLOR_WHITE, COLOR_BLACK);
		print(20 - (int)was._name.size() / 2, 6, was._name, COLOR_WHITE, COLOR_BLACK);
		print(8, 8, Common::String::format("a level %d %s %s", was._level, was._male ? "Male" : "Female",
			TransferLogic::className(was._class)), COLOR_WHITE, COLOR_BLACK);
		print(17, 10, Common::String::format("STR:  %d", was._strength), COLOR_WHITE, COLOR_BLACK);
		print(17, 11, Common::String::format("DEX:  %d", was._dexterity), COLOR_WHITE, COLOR_BLACK);
		print(17, 12, Common::String::format("INT:  %d", was._intellect), COLOR_WHITE, COLOR_BLACK);
		print(10, 15, was._name + (was._avatar ? " is an Avatar." : " is not an Avatar"), COLOR_WHITE, COLOR_BLACK);
		break;

	case PHASE_KEEP_NAME:
		drawTable();
		drawNote("Keep this name?");
		break;

	case PHASE_NEW_NAME:
		drawTable();
		drawNote("Enter new name: " + _name);
		drawCursor(2 + 16 + _name.size(), NOTE_ROW, _cursorFrame);
		break;

	case PHASE_KEEP_SEX:
		drawTable();
		drawNote("Keep same sex?");
		break;

	case PHASE_CONVERT: {
		const TransferLogic::Character &now = _logic._new;
		drawTable();

		switch (_step) {
		case STEP_CLASS:
			drawNote(was._avatar ? "Thou art now an Avatar:" : "Class remains intact");
			break;
		case 1:
			drawNote("Experience has been converted");
			break;
		case 2:
			drawNote("Level has been converted");
			break;
		case 3:
			drawNote(Common::String::format("Strength: was %d(50), now %d(30)", was._strength, now._strength));
			break;
		case 4:
			drawNote(Common::String::format("Dexterity: was %d(50), now %d(30)", was._dexterity, now._dexterity));
			break;
		default:
			drawNote(Common::String::format("Intellect: was %d(50), now %d(30)", was._intellect, now._intellect));
			break;
		}
		break;
	}

	case PHASE_SAVED:
		drawTable();
		drawNote("Conversion complete, saving...");
		break;
	}
}

void TransferCharacter::timeout() {
	_cursorFrame = (_cursorFrame + 1) & 3;
	if (_phase == PHASE_NEW_NAME)
		redraw();

	delayFrames(FRAMES_PER_STEP);
}

// Works out the part of the character that a step of the conversion is about
void TransferCharacter::showStep() {
	switch (_step) {
	case STEP_CLASS:
		_logic.convertClass();
		break;
	case 1:
		_logic.convertExperience();
		break;
	case 2:
		_logic.convertLevel();
		break;
	case 3:
		_logic.convertStrength();
		break;
	case 4:
		_logic.convertDexterity();
		break;
	default:
		_logic.convertIntellect();
		break;
	}
}

void TransferCharacter::advance() {
	if (++_step >= STEP_COUNT) {
		_phase = PHASE_SAVED;
		return;
	}

	showStep();
}

void TransferCharacter::finish() {
	_logic.apply(_newGame._party[0]);

	_G(savegame) = _newGame;
	close();
}

bool TransferCharacter::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	const char letter = toupper(msg.ascii);

	switch (_phase) {
	case PHASE_MISSING:
	case PHASE_BAD:
		close();
		return true;

	case PHASE_FOUND:
		_phase = PHASE_KEEP_NAME;
		break;

	case PHASE_KEEP_NAME:
		if (letter == 'Y') {
			_phase = PHASE_KEEP_SEX;
		} else if (letter == 'N') {
			_name.clear();
			_phase = PHASE_NEW_NAME;
		} else {
			return false;
		}
		break;

	case PHASE_NEW_NAME:
		if (msg.keycode == Common::KEYCODE_RETURN || msg.keycode == Common::KEYCODE_KP_ENTER) {
			if (_name.empty())
				return true;

			_logic._new._name = _name;
			_phase = PHASE_KEEP_SEX;
		} else if (msg.keycode == Common::KEYCODE_BACKSPACE || msg.keycode == Common::KEYCODE_LEFT) {
			if (!_name.empty())
				_name.deleteLastChar();
		} else if (msg.ascii >= 0x20 && msg.ascii < 0x80 && (int)_name.size() < MAX_NAME_LENGTH) {
			_name += (char)msg.ascii;
		} else {
			return false;
		}
		break;

	case PHASE_KEEP_SEX:
		if (letter != 'Y' && letter != 'N')
			return false;

		_logic._new._male = (letter == 'Y') == _logic._old._male;
		_phase = PHASE_CONVERT;
		_step = 0;
		showStep();
		break;

	case PHASE_CONVERT:
		advance();
		break;

	case PHASE_SAVED:
		finish();
		return true;
	}

	redraw();
	return true;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
