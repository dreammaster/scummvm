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

#ifndef AESOP_EVENTS_H
#define AESOP_EVENTS_H

#include "common/scummsys.h"
#include "common/events.h"
#include "common/stack.h"

namespace Aesop {

#define GAME_FRAME_RATE 30
#define GAME_FRAME_TIME (1000 / GAME_FRAME_RATE)

class AesopEngine;

class Events {
private:
	AesopEngine *_vm;
	uint32 _frameCounter;
	uint32 _priorFrameTime;
	Common::Stack<Common::KeyState> _pendingKeys;

	/**
	 * Check whether it's time to display the next screen frame
	 */
	bool checkForNextFrameCounter();
public:
	int _cursorId;
	bool _pressed;
public:
	Events(AesopEngine *vm);
	~Events();

	/**
	 * Clear all pending events
	 */
	void clearEvents();

	/**
	 * Clear any queued keyboard events
	 */
	void clearKeyboard();

	/**
	 * Set the cursor to show
	 */
	void setCursor(int cursorId);

	/**
	 * Returns the cursor
	 */
	int getCursor() const;

	/**
	 * Returns true if the mouse cursor is visible
	 */
	bool isCursorVisible() const;

	/**
	 * Show the cursor
	 */
	void showCursor();

	/**
	 * Hide the cursor
	 */
	void hideCursor();

	/**
	 * Check for any pending events
	 */
	void pollEvents();

	/**
	 * Poll for events and introduce a small delay, to allow the system to
	 * yield to other running programs
	 */
	void pollEventsAndWait();

	/**
	 * Delay for a specified time
	 */
	void delay(uint32 time);

	/**
	 * Returns true if a keyboard key is pending
	 */
	bool isKeyPending() const;

	/**
	 * Get a pending key
	 */
	Common::KeyState getKey();

	/**
	 * Return the current frame
	 */
	uint32 frameCounter() const { return _frameCounter; }
};

} // End of namespace Aesop

#endif /* AESOP_EVENTS_H */
