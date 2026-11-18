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

#include "common/array.h"
#include "ultima/ultima5/logic/visibility_logic.h"

namespace Ultima {
namespace Ultima5 {

constexpr int CENTER = VIEW_TILES / 2;
constexpr int LIGHT_REACH = 10;
constexpr int NEIGHBOR_COUNT = 8;

// Tiles that block the view
static const byte OPAQUE_TILES[] = {
	0x09, 0x0A, 0x0C, 0x0D, 0x4D, 0x4E, 0x4F, 0x5A, 0x97, 0xB8, 0xB9, 0xBC,
	0xD0, 0xD1, 0xD2, 0xD3, 0xF8, 0xFE, 0xFF
};

// Doors and the like, which can only be seen through from right beside them
static const byte NEAR_ONLY_TILES[] = { 0x4B, 0x4A, 0xBA, 0xBB, 0x98 };

// Tiles that give off light
static const byte LIGHT_SOURCE_TILES[] = { 0xDC, 0xBD, 0xBE, 0xB2, 0xDE, 0xBF, 0xB0, 0xB1, 0xB3, 0xBC };

// The tiles around one are looked at in turn, starting to the left and going round,
// each a step on from the last
static const int8 NEIGHBOR_DX[NEIGHBOR_COUNT] = { -1, 0, 1, 1, 0, 0, -1, -1 };
static const int8 NEIGHBOR_DY[NEIGHBOR_COUNT] = { 0, 1, 0, 0, -1, -1, 0, 0 };

static bool contains(const byte *list, int count, byte tile) {
	for (int i = 0; i < count; ++i) {
		if (list[i] == tile)
			return true;
	}

	return false;
}

bool VisibilityLogic::isTransparent(byte tile, int distance) {
	if (contains(NEAR_ONLY_TILES, ARRAYSIZE(NEAR_ONLY_TILES), tile))
		return distance == 1;

	return !contains(OPAQUE_TILES, ARRAYSIZE(OPAQUE_TILES), tile);
}

int VisibilityLogic::distanceFromParty(int x, int y) {
	if (x < 0 || y < 0 || x >= VIEW_TILES || y >= VIEW_TILES)
		return 0;

	const int dx = CENTER - x;
	const int dy = CENTER - y;
	return dx * dx + dy * dy;
}

byte VisibilityLogic::litAt(int x, int y) const {
	const int index = y * VISIBILITY_MAP_SIZE + x;
	return (index >= 0 && index < VISIBILITY_MAP_SIZE * VISIBILITY_MAP_SIZE) ? _lit[index] : 0;
}

void VisibilityLogic::clearView() {
	memset(_view, 0xFF, sizeof(_view));
}

void VisibilityLogic::flood(int light, int originY, int originX, int outY, int outX, Target target) {
	const bool viewTarget = target == TARGET_VIEW;
	byte *map = viewTarget ? _view : _lit;
	const int mapRows = viewTarget ? VIEW_TILES : VISIBILITY_MAP_SIZE;

	if (light <= 0)
		return;

	// Where from are the tiles, whose neighbors are to be looked at
	int queue[VIEW_TILES * VIEW_TILES * 2 + 2];
	int readPos = 0, writePos = 0;

	++light;
	queue[writePos++] = CENTER;
	queue[writePos++] = CENTER;
	map[outY * VISIBILITY_MAP_SIZE + outX + CENTER * VISIBILITY_MAP_SIZE + CENTER] =
		_source->tileAt(_originX + originX + CENTER, _originY + originY + CENTER);

	while (readPos != writePos) {
		const int parentY = queue[readPos++];
		const int parentX = queue[readPos++];
		int y = parentY, x = parentX;

		for (int i = 0; i < NEIGHBOR_COUNT; ++i) {
			x += NEIGHBOR_DX[i];
			y += NEIGHBOR_DY[i];

			if (x < 0 || x >= VIEW_TILES || y < 0 || y >= VIEW_TILES)
				continue;

			const int row = outY + y;
			const int col = outX + x;
			if (row < 0 || col < 0 || row >= mapRows || col >= VISIBILITY_MAP_SIZE)
				continue;

			byte *out = &map[row * VISIBILITY_MAP_SIZE + col];

			// Each tile is only looked at once
			if (!viewTarget) {
				if (_view[y * VISIBILITY_MAP_SIZE + x] == 0)
					continue;

				_view[y * VISIBILITY_MAP_SIZE + x] = 0;
			} else if (*out != 0xFF) {
				continue;
			}

			byte tile = _source->tileAt(_originX + originX + x, _originY + originY + y);
			const int distance = distanceFromParty(x, y);

			if (distance < light) {
				*out = tile;
			} else if (viewTarget) {
				// Beyond the reach of the party's light, a tile can still be seen if it's lit
				// by a light of the map
				const int mapX = originX + x;
				const int mapY = originY + y;
				const int parentMapX = originX + parentX;
				const int parentMapY = originY + parentY;

				if (!isTransparent(tile, distance)) {
					if (parentMapY >= VISIBILITY_MAP_SIZE || parentMapX >= VISIBILITY_MAP_SIZE || mapY < 0 ||
							parentMapX < 0 || _view[parentY * VISIBILITY_MAP_SIZE + parentX] == 0 ||
							litAt(parentMapX, parentMapY) == 0 || litAt(mapX, mapY) == 0) {
						tile = *out = 0xFF;
					} else {
						*out = tile;
					}
				} else if (mapY < 0 || mapX < 0 || mapX >= VISIBILITY_MAP_SIZE || mapY >= VISIBILITY_MAP_SIZE) {
					*out = 0;
				} else {
					*out = litAt(mapX, mapY) != 0 ? tile : 0;
				}
			} else {
				tile = 0xFF;
			}

			if (isTransparent(tile, distance)) {
				queue[writePos++] = y;
				queue[writePos++] = x;
			}
		}
	}
}

void VisibilityLogic::buildLitMap(const TileSource &source, int originX, int originY) {
	_source = &source;
	_originX = originX;
	_originY = originY;
	memset(_lit, 0xFF, sizeof(_lit));

	Common::Array<byte> sourceY, sourceX;
	for (int y = 0; y < VISIBILITY_MAP_SIZE; ++y) {
		for (int x = 0; x < VISIBILITY_MAP_SIZE; ++x) {
			const byte tile = source.tileAt(originX + x, originY + y);

			if (contains(LIGHT_SOURCE_TILES, ARRAYSIZE(LIGHT_SOURCE_TILES), tile)) {
				sourceY.push_back(y);
				sourceX.push_back(x);
				_lit[y * VISIBILITY_MAP_SIZE + x] = tile;
			}
		}
	}

	for (uint i = 0; i < sourceY.size(); ++i) {
		const int top = sourceY[i] - CENTER;
		const int left = sourceX[i] - CENTER;

		clearView();
		flood(LIGHT_REACH, top, left, top, left, TARGET_LIT_MAP);
	}

	for (int i = 0; i < VISIBILITY_MAP_SIZE * VISIBILITY_MAP_SIZE; ++i) {
		if (_lit[i] == 0xFF)
			_lit[i] = 0;
	}

	_litValid = true;
}

void VisibilityLogic::computeView(const TileSource &source, int originX, int originY, int partyX, int partyY,
		int light, byte view[VIEW_TILES][VIEW_TILES]) {
	if (!_litValid || _originX != originX || _originY != originY)
		buildLitMap(source, originX, originY);

	_source = &source;

	clearView();

	if (light > 0) {
		// The map wraps around, so where the party is within the 32x32 tiles is too
		const int localX = (partyX - originX) & 0xFF;
		const int localY = (partyY - originY) & 0xFF;
		flood(light, localY - CENTER, localX - CENTER, 0, 0, TARGET_VIEW);

		for (int i = 0; i < VIEW_TILES * VISIBILITY_MAP_SIZE; ++i) {
			if (_view[i] == 0)
				_view[i] = 0xFF;
		}
	}

	for (int y = 0; y < VIEW_TILES; ++y)
		memcpy(view[y], &_view[y * VISIBILITY_MAP_SIZE], VIEW_TILES);
}

} // namespace Ultima5
} // namespace Ultima
