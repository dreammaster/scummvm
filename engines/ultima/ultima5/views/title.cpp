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

#include "ultima/ultima5/views/title.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int FRAMES_PER_STEP = 3;

enum MenuChoice {
	CHOICE_JOURNEY, CHOICE_CREATE, CHOICE_TRANSFER, CHOICE_INTRODUCTION,
	CHOICE_ACKNOWLEDGEMENTS, CHOICE_VIEW
};

// The letter that picks each choice
static const char MENU_LETTERS[] = "JCTUAR";

// The part of the screen that has the logo, which is revealed pixel by pixel in a shuffled order
constexpr int REVEAL_WIDTH = 320;
constexpr int REVEAL_HEIGHT = 101;
constexpr int REVEAL_TICKS = 120;
constexpr int REVEAL_MS = 2000;
constexpr uint16 REVEAL_TAPS = 0x6000;

void Title::startReveal() {
	// A shift register of fifteen bits goes through each number but nought, and those that are in the logo are the pixels
	const int count = REVEAL_WIDTH * REVEAL_HEIGHT;
	uint16 value = 1;

	_revealOrder.clear();
	do {
		if (value < count)
			_revealOrder.push_back(value);

		const bool carry = (value & 1) != 0;
		value >>= 1;
		if (carry)
			value ^= REVEAL_TAPS;
	} while (value != 1);
	_revealOrder.push_back(0);

	_shown.clear();
	_shown.resize(count);
	_revealed = 0;
	_revealing = true;

	g_engine->sound().titleReveal(REVEAL_MS);
}

void Title::finishReveal() {
	g_engine->sound().stop();

	_revealing = false;
	_introDone = true;
}

bool Title::msgFocus(const FocusMessage &msg) {
	if (!loadTitleGraphics())
		error("Could not load the title graphics");

	_selected = 0;
	_animFrame = 0;
	_showNotice = false;

	g_engine->cueMusic(Music::TRACK_THEME);

	if (!_introDone)
		startReveal();

	delayFrames(_revealing ? 1 : FRAMES_PER_STEP);
	return TitleBase::msgFocus(msg);
}

void Title::draw() {
	if (_revealing) {
		drawLogo();

		auto s = getSurface();
		if (s.format.bytesPerPixel == 1) {
			for (int i = 0; i < REVEAL_WIDTH * REVEAL_HEIGHT; ++i) {
				if (!_shown[i])
					*(byte *)s.getBasePtr(i % REVEAL_WIDTH, i / REVEAL_WIDTH) = COLOR_BLACK;
			}
		}

		return;
	}

	drawTitle(_animFrame);

	if (_showNotice) {
		drawMessage("No active game.", "Please create a character or transfer", "one from Ultima IV.");
	} else {
		drawMenu(_selected);
		drawPrompt(_animFrame);
	}
}

void Title::timeout() {
	if (_revealing) {
		const int target = MIN<int>(_revealOrder.size(), _revealed + (int)_revealOrder.size() / REVEAL_TICKS + 1);

		for (; _revealed < target; ++_revealed)
			_shown[_revealOrder[_revealed]] = 1;

		if (_revealed >= (int)_revealOrder.size())
			finishReveal();

		redraw();
		delayFrames(_revealing ? 1 : FRAMES_PER_STEP);
		return;
	}

	_animFrame = (_animFrame + 1) & 3;
	redraw();
	delayFrames(FRAMES_PER_STEP);
}

void Title::choose(int index, bool activate) {
	_selected = index;
	redraw();

	if (activate)
		this->activate(index);
}

void Title::activate(int index) {
	switch (index) {
	case CHOICE_JOURNEY:
		if (_G(savegame).hasActiveGame()) {
			replaceView("Game", true);
		} else {
			_showNotice = true;
			redraw();
		}
		break;
	case CHOICE_CREATE:
		addView("CreateCharacter");
		break;
	case CHOICE_TRANSFER:
		addView("TransferCharacter");
		break;
	case CHOICE_INTRODUCTION:
		addView("Introduction");
		break;
	case CHOICE_ACKNOWLEDGEMENTS:
		addView("Acknowledgements");
		break;
	default:
		break;
	}
}

bool Title::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	// A key shows the whole logo at once
	if (_revealing) {
		finishReveal();
		redraw();
		return true;
	}

	if (_showNotice) {
		_showNotice = false;
		redraw();
		return true;
	}

	switch (msg.keycode) {
	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_UP:
		choose((_selected + MENU_ITEM_COUNT - 1) % MENU_ITEM_COUNT, false);
		return true;
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_DOWN:
		choose((_selected + 1) % MENU_ITEM_COUNT, false);
		return true;
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
	case Common::KEYCODE_SPACE:
		activate(_selected);
		return true;
	default:
		break;
	}

	const char letter = toupper(msg.ascii);
	for (int i = 0; MENU_LETTERS[i]; ++i) {
		if (letter == MENU_LETTERS[i]) {
			choose(i, true);
			return true;
		}
	}

	return false;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
