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

#include "common/file.h"
#include "ultima/ultima5/views/create_character.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr byte WHITE = 15;
constexpr int MAX_NAME_LENGTH = 8;
constexpr int MAX_TEXT_LENGTH = 2000;
constexpr int FRAMES_PER_STEP = 3;
constexpr int NAME_COLUMN = 15;
constexpr int NAME_ROW = 19;

// Where the text of the gypsy's introduction and her last words are in question.dat
constexpr uint INTRO_TEXT_OFFSET = 0;
constexpr uint RESULT_TEXT_OFFSET = 0x322;

// The pictures of create.16
constexpr int PICTURE_TENT = 0;
constexpr int PICTURE_CARD_BACK = 1;
constexpr int PICTURE_FIRST_CARD = 2;
constexpr int PICTURE_RESULT = 10;

// Where each virtue's card is drawn, on the left and shifted for the right
static const byte CARD_X[VIRTUE_COUNT] = { 0x28, 0x30, 0x30, 0x28, 0x28, 0x30, 0x28, 0x30 };
static const byte CARD_Y[VIRTUE_COUNT] = { 0x05, 0x07, 0x04, 0x0A, 0x08, 0x00, 0x05, 0x06 };
constexpr int RIGHT_CARD_OFFSET = 0xB8;

bool CreateCharacter::msgFocus(const FocusMessage &msg) {
	Common::File f;
	if (_questions.empty() && f.open("question.dat")) {
		_questions.resize(f.size());
		f.read(&_questions[0], _questions.size());
	}

	if (!_intro.load() || !_images.load("create", _G(renderMode)) || !_text.load() || _questions.empty() ||
			!_newGame.load("init.gam"))
		error("Could not load the character creation");

	_page.create(320, 200);
	_name.clear();
	_cursorFrame = 0;
	showNamePrompt();

	delayFrames(FRAMES_PER_STEP);
	return Shared::View::msgFocus(msg);
}

Common::String CreateCharacter::questionText(uint offset) const {
	Common::String result;

	for (uint i = offset; i < _questions.size() && i < offset + MAX_TEXT_LENGTH && _questions[i] != 0; ++i)
		result += (char)_questions[i];

	return result;
}

void CreateCharacter::showNamePrompt() {
	_phase = PHASE_NAME;
	_intro.drawEmptyMenu(_page, 0);
	_intro.print(_page, 3, 17, "By what name shalt thou be known?", WHITE, 0);
	_intro.print(_page, 14, NAME_ROW, ":", WHITE, 0);
}

void CreateCharacter::showGenderPrompt() {
	_phase = PHASE_GENDER;
	_intro.print(_page, NAME_COLUMN, NAME_ROW, _name.c_str(), WHITE, 0);
	_intro.print(_page, 8, 21, "Art thou Male or Female? ", WHITE, 0);
}

void CreateCharacter::showIntro() {
	_phase = PHASE_INTRO;
	_page.clear();
	_images.draw(_page, PICTURE_TENT, 0, 0x60);

	Gfx::StoryLayout layout;
	layout._left[1] = 0xAF;
	layout._splitTop = 0x59;

	int x = 0, y = 9;
	_text.draw(_page, questionText(INTRO_TEXT_OFFSET), x, y, layout);
}

void CreateCharacter::showQuestion() {
	_phase = PHASE_QUESTION;
	_question = _logic.nextQuestion();

	_page.clear();
	_images.draw(_page, PICTURE_CARD_BACK, 0x10, 0);
	_images.draw(_page, PICTURE_CARD_BACK, 200, 0);
	_images.draw(_page, PICTURE_FIRST_CARD + _question._left, CARD_X[_question._left], CARD_Y[_question._left]);
	_images.draw(_page, PICTURE_FIRST_CARD + _question._right, CARD_X[_question._right] + RIGHT_CARD_OFFSET,
		CARD_Y[_question._right]);

	int x = 0, y = 0x98;
	_text.draw(_page, questionText(_question._textOffset), x, y, Gfx::StoryLayout());
}

void CreateCharacter::showResult() {
	_phase = PHASE_RESULT;
	_page.clear();
	_images.draw(_page, PICTURE_RESULT, 0xA8, 100);

	Gfx::StoryLayout layout;
	layout._right[1] = 0xA6;
	layout._splitTop = 0x5A;
	layout._spaceWidth = 4;

	int x = 0, y = 0;
	_text.draw(_page, questionText(RESULT_TEXT_OFFSET), x, y, layout);
}

void CreateCharacter::finish() {
	Data::Character &avatar = _newGame._party[0];
	Common::strlcpy(avatar._name, _name.c_str(), sizeof(avatar._name));
	_logic.finish(avatar);

	_G(savegame) = _newGame;
	close();
}

void CreateCharacter::draw() {
	auto s = getSurface();
	s.blitFrom(_page);

	if (_phase == PHASE_NAME) {
		_intro.print(s, NAME_COLUMN, NAME_ROW, _name.c_str(), WHITE, 0);
		_intro.drawCursor(s, NAME_COLUMN + _name.size(), NAME_ROW, _cursorFrame);
	}
}

void CreateCharacter::timeout() {
	_cursorFrame = (_cursorFrame + 1) & 3;
	redraw();
	delayFrames(FRAMES_PER_STEP);
}

bool CreateCharacter::nameKeypress(const KeypressMessage &msg) {
	switch (msg.keycode) {
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
		// No name cancels the new character
		if (_name.empty()) {
			close();
			return true;
		}

		showGenderPrompt();
		break;

	case Common::KEYCODE_BACKSPACE:
	case Common::KEYCODE_LEFT:
		if (!_name.empty())
			_name.deleteLastChar();
		break;

	case Common::KEYCODE_ESCAPE:
		_name.clear();
		break;

	default:
		if (msg.ascii >= 0x20 && msg.ascii < 0x80 && (int)_name.size() < MAX_NAME_LENGTH)
			_name += (char)msg.ascii;
		else
			return false;
		break;
	}

	redraw();
	return true;
}

bool CreateCharacter::genderKeypress(const KeypressMessage &msg) {
	const char letter = toupper(msg.ascii);
	if (letter != 'M' && letter != 'F')
		return false;

	_newGame._party[0]._gender = (letter == 'M') ? Data::GENDER_MALE : Data::GENDER_FEMALE;
	_logic.start(_newGame._party[0]);

	showIntro();
	redraw();
	return true;
}

bool CreateCharacter::questionKeypress(const KeypressMessage &msg) {
	const char letter = toupper(msg.ascii);
	if (letter != 'A' && letter != 'B')
		return false;

	_logic.answer(_question, letter == 'A');

	if (_logic.isFinished())
		showResult();
	else
		showQuestion();

	redraw();
	return true;
}

bool CreateCharacter::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	switch (_phase) {
	case PHASE_NAME:
		return nameKeypress(msg);
	case PHASE_GENDER:
		return genderKeypress(msg);
	case PHASE_INTRO:
		showQuestion();
		redraw();
		return true;
	case PHASE_QUESTION:
		return questionKeypress(msg);
	case PHASE_RESULT:
		finish();
		return true;
	}

	return false;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
