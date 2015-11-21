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

#ifndef LEGEND_EVENTS_H
#define LEGEND_EVENTS_H

#include "common/scummsys.h"
#include "common/events.h"
#include "common/stack.h"

namespace Legend {

#define GAME_FRAME_RATE 30
#define GAME_FRAME_TIME (1000 / GAME_FRAME_RATE)

enum CursorId { INVALID_CURSOR = -1 };

class LegendEngine;

class Events {
private:
	LegendEngine *_vm;
	uint32 _frameCounter;
	uint32 _priorFrameTime;

	/**
	 * Check for the next frame
	 */
	void checkForNextFrameCounter();
public:
	CursorId _cursorId;
public:
	Events(LegendEngine *vm);
	~Events();

	/**
	 * Load a set of cursors from the specified file
	 */
	void loadCursors(const Common::String &filename);

	/**
	 * Set the cursor to show
	 */
	void setCursor(CursorId cursorId);

	/**
	 * Show the mouse cursor
	 */
	void showCursor();

	/**
	 * Hide the mouse cursor
	 */
	void hideCursor();

	/**
	 * Returns the cursor
	 */
	CursorId getCursor() const;

	/**
	 * Returns true if the mouse cursor is visible
	 */
	bool isCursorVisible() const;

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
	 * Get the current mouse position within the scene, adjusted by the scroll position
	 */
	Common::Point mousePos() const;

	/**
	 * Return the current game frame number
	 */
	uint32 getFrameCounter() const { return _frameCounter; }
};

} // End of namespace Legend

#endif /* LEGEND_EVENTS_H */
