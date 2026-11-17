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

#include "ultima/ultima5/views/acknowledgements.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int SCROLL_LEFT = 0;
constexpr int SCROLL_CREDITS = 1;
constexpr int SCROLL_RIGHT = 2;

constexpr int TOP = 0x3F;
constexpr int BOTTOM = 199;
constexpr int CENTER_LEFT = 0x90;
constexpr int CENTER_RIGHT = 0xA0;
constexpr int SLIDE_STEP = 8;
constexpr int SLIDE_DISTANCE = 0x88;
constexpr int RISE_STEPS_PER_FRAME = 4;
constexpr int SLIDE_FRAMES = 3;
constexpr int MENU_ITEM_ACKNOWLEDGEMENTS = 4;

bool Acknowledgements::msgFocus(const FocusMessage &msg) {
	if (!_intro.load() || !_scroll.load("startsc", _G(renderMode)) || _scroll.size() < 3)
		error("Could not load the credits");

	_screen.create(320, 200);
	_credits.create(320, 200);
	_menu.create(320, 200);

	_intro.drawTitle(_screen, 0);
	_intro.drawMenu(_screen, MENU_ITEM_ACKNOWLEDGEMENTS);
	_intro.drawPrompt(_screen, 0);

	_credits.clear();
	_scroll.draw(_credits, SCROLL_CREDITS, 0x10, TOP);

	_menu.clear();
	_intro.drawSubtitle(_menu, 0);
	_intro.drawMenuFrame(_menu);
	_intro.drawMenu(_menu, MENU_ITEM_ACKNOWLEDGEMENTS);

	_phase = PHASE_RISE;
	_pos = BOTTOM;
	delayFrames(1);
	return Shared::View::msgFocus(msg);
}

void Acknowledgements::drawScrollEdges(int leftX, int rightX, int y) {
	_scroll.draw(_screen, SCROLL_LEFT, leftX, y);
	_scroll.draw(_screen, SCROLL_RIGHT, rightX, y);
}

void Acknowledgements::restore(const Graphics::ManagedSurface &src, int x1, int y1, int x2, int y2) {
	const Common::Rect r(x1, y1, x2 + 1, y2 + 1);
	_screen.blitFrom(src, r, Common::Point(x1, y1));
}

void Acknowledgements::step() {
	switch (_phase) {
	case PHASE_RISE:
		for (int i = 0; i < RISE_STEPS_PER_FRAME && _pos >= TOP; ++i, --_pos)
			drawScrollEdges(CENTER_LEFT, CENTER_RIGHT, _pos);

		if (_pos < TOP) {
			_phase = PHASE_OPEN;
			_pos = 0;
		}
		break;

	case PHASE_OPEN:
		_scroll.draw(_screen, SCROLL_LEFT, 0x88 - _pos, TOP);
		restore(_credits, 0x98 - _pos, TOP, 0x9F - _pos, BOTTOM);
		_scroll.draw(_screen, SCROLL_RIGHT, _pos + 0xA8, TOP);
		restore(_credits, _pos + 0xA0, TOP, _pos + 0xA7, BOTTOM);

		_pos += SLIDE_STEP;
		if (_pos > SLIDE_DISTANCE)
			_phase = PHASE_WAIT;
		break;

	case PHASE_CLOSE:
		_scroll.draw(_screen, SCROLL_LEFT, CENTER_LEFT - _pos, TOP);
		restore(_menu, 0x88 - _pos, TOP, 0x8F - _pos, BOTTOM);
		_scroll.draw(_screen, SCROLL_RIGHT, _pos + CENTER_RIGHT, TOP);
		restore(_menu, _pos + 0xB0, TOP, _pos + 0xB7, BOTTOM);

		_pos -= SLIDE_STEP;
		if (_pos < 0) {
			_phase = PHASE_DROP;
			_pos = TOP;
		}
		break;

	case PHASE_DROP:
		for (int i = 0; i < RISE_STEPS_PER_FRAME && _pos < BOTTOM; ++i, ++_pos) {
			drawScrollEdges(CENTER_LEFT, CENTER_RIGHT, _pos + 1);
			restore(_menu, CENTER_LEFT, _pos, CENTER_LEFT + 0x1F, _pos);
		}

		if (_pos >= BOTTOM) {
			restore(_menu, CENTER_LEFT, BOTTOM, CENTER_LEFT + 0x1F, BOTTOM);
			close();
			return;
		}
		break;

	default:
		break;
	}

	redraw();
}

void Acknowledgements::draw() {
	auto s = getSurface();
	s.blitFrom(_screen);
}

void Acknowledgements::timeout() {
	step();

	// The scroll rises quickly, but unfurls and closes at a steady pace
	delayFrames(_phase == PHASE_RISE || _phase == PHASE_DROP ? 1 : SLIDE_FRAMES);
}

bool Acknowledgements::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode) || _phase != PHASE_WAIT)
		return false;

	_phase = PHASE_CLOSE;
	_pos = SLIDE_DISTANCE;
	return true;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
