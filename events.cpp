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
#include "aesop/aesop.h"
#include "aesop/events.h"

namespace Aesop {

enum ButtonFlag { LEFT_BUTTON = 1, RIGHT_BUTTON = 2 };

Events::Events(AesopEngine *vm) : _vm(vm) {
	_cursorId = 0;
	_frameCounter = 1;
	_priorFrameTime = 0;
	_pressed = false;
}

Events::~Events() {
}

void Events::setCursor(int cursorId) {

}

void Events::showCursor() {
	CursorMan.showMouse(true);
}

void Events::hideCursor() {
	CursorMan.showMouse(false);
}

int Events::getCursor() const {
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
			}
			return;
		case Common::EVENT_KEYUP:
			return;
		case Common::EVENT_LBUTTONDOWN:
			_pressed = true;
			return;
		case Common::EVENT_LBUTTONUP:
			_pressed = false;
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

bool Events::checkForNextFrameCounter() {
	// Check for next game frame
	uint32 milli = g_system->getMillis();
	if ((milli - _priorFrameTime) >= GAME_FRAME_TIME) {
		++_frameCounter;
		_priorFrameTime = milli;

		// Give time to the debugger
		_vm->_debugger->onFrame();

		// Display the frame
		_vm->_screen->update();

		return true;
	}

	return false;
}

void Events::clearEvents() {
	_pressed = false;
	_pendingKeys.clear();
}

void Events::clearKeyboard() {
	_pendingKeys.clear();
}

void Events::delay(uint32 time) {
	// Different handling for really short versus extended times
	if (time < 10) {
		// For really short periods, simply delay by the desired amount
		pollEvents();
		g_system->delayMillis(time);
	} else {
		// For long periods go into a loop where we delay by 10ms at a time and then
		// check for events. This ensures for longer delays that responsiveness is
		// maintained
		uint32 delayEnd = g_system->getMillis() + time;

		while (!_vm->shouldQuit() && g_system->getMillis() < delayEnd) {
			pollEventsAndWait();
		}
	}
}

bool Events::isKeyPending() const {
	return !_pendingKeys.empty();
}

Common::KeyState Events::getKey() {
	return _pendingKeys.pop();
}


} // End of namespace Sherlock
