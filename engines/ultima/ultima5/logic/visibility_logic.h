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

#ifndef ULTIMA5_LOGIC_VISIBILITY_LOGIC_H
#define ULTIMA5_LOGIC_VISIBILITY_LOGIC_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {

constexpr int VIEW_TILES = 11;
constexpr int VISIBILITY_MAP_SIZE = 32;

// What is shown in place of anything that can't be seen
constexpr byte TILE_HIDDEN = 0xFF;

/**
 * Gives the tiles of a map to work out visibility over, by their
 * position on the map
 */
class TileSource {
public:
	virtual ~TileSource() {}

	virtual byte tileAt(int x, int y) const = 0;
};

/**
 * Works out what the party can see of the map around it. What can be seen is what
 * is within the radius of the light the party has, or of a light of the map such as a
 * torch or a window, and isn't hidden behind walls and the like. It works over a
 * square of 32x32 tiles of the map, which is the whole of a location, or the
 * part of Britannia that's loaded
 */
class VisibilityLogic {
private:
	enum Target {
		TARGET_LIT_MAP,
		TARGET_VIEW
	};

	// Which tiles are lit by a light of the map
	byte _lit[VISIBILITY_MAP_SIZE * VISIBILITY_MAP_SIZE];

	// The tiles seen from the party, or what's been looked at already when finding
	// what a light lights
	byte _view[VIEW_TILES * VISIBILITY_MAP_SIZE];

	const TileSource *_source = nullptr;
	int _originX = 0;
	int _originY = 0;
	bool _litValid = false;

	/**
	 * Returns true if the tile lets what's beyond it be seen, from a distance
	 */
	static bool isTransparent(byte tile, int distance);

	/**
	 * Returns how far from the party a tile of the view is, as the square of the distance
	 */
	static int distanceFromParty(int x, int y);

	/**
	 * Spreads out from a tile of the 11x11 area around a position, to see what can be
	 * seen from it
	 * @param light		How bright the light is, as the square of how far it reaches
	 * @param y,x		Where the area starts, on the map
	 * @param outY,outX	Where the area starts in the result
	 */
	void flood(int light, int y, int x, int outY, int outX, Target target);

	void clearView();

public:
	/**
	 * Returns what lights a tile of the map, which is nothing if it's outside it
	 */
	byte litAt(int x, int y) const;

	/**
	 * Forgets which tiles are lit, for when the map or its lights have changed
	 */
	void invalidate() {
		_litValid = false;
	}

	/**
	 * Works out which tiles are lit by the lights of the map
	 * @param originX,originY	Where the 32x32 tiles start on the map
	 */
	void buildLitMap(const TileSource &source, int originX, int originY);

	/**
	 * Works out the tiles the party sees of the 11x11 around it, with
	 * the hidden ones set to TILE_HIDDEN
	 * @param partyX,partyY		Where the party is
	 * @param light				How bright the light the party has is
	 */
	void computeView(const TileSource &source, int originX, int originY, int partyX, int partyY, int light,
		byte view[VIEW_TILES][VIEW_TILES]);
};

} // namespace Ultima5
} // namespace Ultima

#endif
