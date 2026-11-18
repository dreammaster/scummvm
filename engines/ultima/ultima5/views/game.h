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

#ifndef ULTIMA5_VIEWS_GAME_H
#define ULTIMA5_VIEWS_GAME_H

#include "ultima/ultima5/gfx/tileset.h"
#include "ultima/ultima5/views/message_log.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int VIEW_TILES = 11;

/**
 * The main game screen: the map, with the party, the status and the messages
 * around it
 */
class Game : public View {
private:
	MessageLog _messages;

	/**
	 * Clears the screen and draws the blue frame and the outlines of the windows
	 */
	void drawFrame();

	/**
	 * Draws the names, hit points and condition of the first members of the party
	 */
	void drawParty();

	/**
	 * Draws the food, gold, date and the label between them
	 */
	void drawStatus();

	/**
	 * Draws the wind in the bottom border, or clears it below ground
	 */
	void drawWind();

	/**
	 * Draws the part of Britannia or the underworld around the party, with the
	 * party and the other objects on it
	 */
	void drawWorld();

	/**
	 * Works out which objects are seen in each tile of the window
	 */
	void placeObjects(byte objects[VIEW_TILES][VIEW_TILES], byte ground[VIEW_TILES][VIEW_TILES]);

public:
	Game();
	~Game() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
