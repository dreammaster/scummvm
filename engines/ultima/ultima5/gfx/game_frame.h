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

#ifndef ULTIMA5_GFX_GAME_FRAME_H
#define ULTIMA5_GFX_GAME_FRAME_H

#include "graphics/managed_surface.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/gfx/text_screen.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int VISIBLE_PARTY_MEMBERS = 6;

/**
 * Draws the main game screen around the map: the blue frame, the party
 * and their hit points, the food, gold and date, and the wind
 */
class GameFrame : public TextScreen {
public:
	/**
	 * Loads the font
	 */
	void load();

	/**
	 * Clears the screen and draws the frame and the window outlines
	 */
	void drawFrame(Graphics::ManagedSurface &dst) const;

	/**
	 * Draws the names, hit points and condition of the first members of the party
	 */
	void drawParty(Graphics::ManagedSurface &dst, const Data::Savegame &savegame) const;

	/**
	 * Draws the food, gold, date and the label between them
	 */
	void drawStatus(Graphics::ManagedSurface &dst, const Data::Savegame &savegame) const;

	/**
	 * Draws the wind in the bottom border, or clears it below ground
	 */
	void drawWind(Graphics::ManagedSurface &dst, const Data::Savegame &savegame) const;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
