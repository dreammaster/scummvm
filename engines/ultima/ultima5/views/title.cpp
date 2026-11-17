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

bool Title::msgFocus(const FocusMessage &msg) {
	if (!_screen.load())
		error("Could not load the title graphics");

	_selected = 0;
	_animFrame = 0;
	_showNotice = false;

	delayFrames(FRAMES_PER_STEP);
	return Shared::View::msgFocus(msg);
}

void Title::draw() {
	auto s = getSurface();
	_screen.drawTitle(s, _animFrame);

	if (_showNotice) {
		if (_G(savegame).hasActiveGame())
			_screen.drawMessage(s, "Journey Onward is not yet playable.");
		else
			_screen.drawMessage(s, "No active game.", "Please create a character or transfer",
				"one from Ultima IV.");
	} else {
		_screen.drawMenu(s, _selected);
		_screen.drawPrompt(s, _animFrame);
	}
}

void Title::timeout() {
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
		_showNotice = true;
		redraw();
		break;
	case CHOICE_CREATE:
		addView("CreateCharacter");
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

	if (_showNotice) {
		_showNotice = false;
		redraw();
		return true;
	}

	switch (msg.keycode) {
	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_UP:
		choose((_selected + Gfx::MENU_ITEM_COUNT - 1) % Gfx::MENU_ITEM_COUNT, false);
		return true;
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_DOWN:
		choose((_selected + 1) % Gfx::MENU_ITEM_COUNT, false);
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
