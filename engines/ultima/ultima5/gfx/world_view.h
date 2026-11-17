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

#ifndef ULTIMA5_GFX_WORLD_VIEW_H
#define ULTIMA5_GFX_WORLD_VIEW_H

#include "graphics/managed_surface.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/data/world_map.h"
#include "ultima/ultima5/gfx/tileset.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int VIEW_TILES = 11;
constexpr int VIEW_X = 8;
constexpr int VIEW_Y = 8;

/**
 * Draws the part of Britannia or the underworld around the party, with the
 * party and the other objects on it, in the window at the left of the screen
 */
class WorldView {
private:
	/**
	 * Works out which objects are seen in each tile of the window
	 */
	static void placeObjects(const Data::Savegame &savegame, byte objects[VIEW_TILES][VIEW_TILES], byte ground[VIEW_TILES][VIEW_TILES]);

public:
	void draw(Graphics::ManagedSurface &dst, const Tileset &tileset, const Data::WorldMap &world,
		const Data::Savegame &savegame) const;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
