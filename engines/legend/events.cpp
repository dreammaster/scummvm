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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/scummsys.h"
#include "common/events.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/cursorman.h"
#include "legend/legend.h"
#include "legend/events.h"

namespace Legend {

enum ButtonFlag { LEFT_BUTTON = 1, RIGHT_BUTTON = 2 };

Events::Events(LegendEngine *vm): _vm(vm) {
	_cursorId = INVALID_CURSOR;
	_frameCounter = 1;
	_priorFrameTime = 0;
}

Events::~Events() {
}

void Events::loadCursors(const Common::String &filename) {
}

void Events::setCursor(CursorId cursorId) {
}

void Events::showCursor() {
	CursorMan.showMouse(true);
}

void Events::hideCursor() {
	CursorMan.showMouse(false);
}

CursorId Events::getCursor() const {
	return _cursorId;
}

bool Events::isCursorVisible() const {
	return CursorMan.isVisible();
}

void Events::pollEvents() {
	checkForNextFrameCounter();

	Common::Event event;
	while (g_system->getEventManager()->pollEvent(event)) {
		// Handle events
		switch (event.type) {
		case Common::EVENT_QUIT:
		case Common::EVENT_RTL:
			return;

		case Common::EVENT_KEYDOWN:
			// Check for debugger
			if (event.kbd.keycode == Common::KEYCODE_d && (event.kbd.flags & Common::KBD_CTRL)) {
				// Attach to the debugger
				_vm->_debugger->attach();
				_vm->_debugger->onFrame();
			} else {
				// TODO
			}
			return;
		case Common::EVENT_KEYUP:
			return;
		case Common::EVENT_LBUTTONDOWN:
			return;
		case Common::EVENT_RBUTTONDOWN:
			return;
		case Common::EVENT_LBUTTONUP:
			return;
		case Common::EVENT_RBUTTONUP:
			return;
		default:
 			break;
		}
	}
}

void Events::pollEventsAndWait() {
	pollEvents();
	g_system->delayMillis(10);
}

Common::Point Events::mousePos() const {
	return g_system->getEventManager()->getMousePos();
}

void Events::checkForNextFrameCounter() {
	// Check for next game frame
	uint32 milli = g_system->getMillis();
	if ((milli - _priorFrameTime) >= GAME_FRAME_RATE) {
		++_frameCounter;
		_priorFrameTime = milli;

		// Give time to the debugger
		_vm->_debugger->onFrame();

		// Display the frame
		_vm->_screen->update();
	}
}

} // End of namespace Legend
