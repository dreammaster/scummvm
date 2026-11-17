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

#include "ultima/ultima5/gfx/world_view.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int ACTOR_TILE_BASE = 0x100;
constexpr int PARTY_TILE_X = 5;
constexpr int PARTY_TILE_Y = 5;

constexpr byte TILE_UNSEEN = 0xFF;
constexpr byte TILE_BLOCKED_VIEW = 0x87;
constexpr byte TILE_CHAIR = 0x92;
constexpr byte TILE_OBSTRUCTED_A = 0x0A;
constexpr byte TILE_OBSTRUCTED_B = 0xEC;
constexpr byte TILE_FLOOR_UNDER = 0x57;
constexpr byte TILE_FLOOR_OVER = 0x38;
constexpr byte TILE_WATER_EDGE_A = 0x6A;
constexpr byte TILE_WATER_EDGE_B = 0x6B;

constexpr byte ACTOR_SLEEPING = 0x1E;
constexpr byte ACTOR_DEAD = 0x1F;
constexpr byte ACTOR_SLEEPING_ALT = 0x1D;
constexpr byte ACTOR_REMAINS = 0xE8;
constexpr byte ACTOR_MUSICIAN = 0x5C;
constexpr byte ACTOR_MUSICIAN_FRAME_OFFSET = 8;

// Whether an object is one that's hidden by some of the terrain it's on
static bool isHideable(byte tile) {
	return tile == 0x1C || (tile >= 0x12 && tile < 0x16) || tile >= 0x40 || (tile >= 0x28 && tile < 0x2C);
}

void WorldView::placeObjects(const Data::Savegame &sg, byte objects[VIEW_TILES][VIEW_TILES], byte ground[VIEW_TILES][VIEW_TILES]) {
	for (int idx = Data::ACTOR_COUNT - 1; idx >= 0; --idx) {
		const Data::ActorRecord actor = sg.getActor(idx);
		if (actor._tile == 0)
			continue;

		// The position in the window, which can wrap around the edge of the map
		const byte vx = actor._x - (sg._mapX - PARTY_TILE_X);
		const byte vy = actor._y - (sg._mapY - PARTY_TILE_Y);
		if (actor._z != sg._mapLevel || vx >= VIEW_TILES || vy >= VIEW_TILES)
			continue;

		byte frame = actor._animTile;
		if (frame == 0)
			continue;

		const byte under = ground[vy][vx];
		if (under == TILE_UNSEEN || under == TILE_BLOCKED_VIEW)
			continue;

		if ((actor._tile & 0xFC) == ACTOR_REMAINS || actor._tile == ACTOR_SLEEPING || actor._tile == ACTOR_DEAD) {
			// Remains stay beneath anything else
			if (objects[vy][vx] == 0)
				objects[vy][vx] = frame;
			continue;
		}

		if (frame == ACTOR_SLEEPING_ALT || frame == ACTOR_SLEEPING) {
			objects[vy][vx] = frame;
			continue;
		}

		if (actor._tile == ACTOR_MUSICIAN) {
			if (under == TILE_CHAIR) {
				objects[vy][vx] = frame;
				continue;
			}

			frame -= ACTOR_MUSICIAN_FRAME_OFFSET;
		}

		if (isHideable(frame)) {
			if (under == TILE_OBSTRUCTED_B || under == TILE_OBSTRUCTED_A)
				continue;

			if (under == TILE_FLOOR_UNDER) {
				ground[vy][vx] = TILE_FLOOR_OVER;
				continue;
			}

			if (under == TILE_WATER_EDGE_A || under == TILE_WATER_EDGE_B) {
				if ((frame & 0xF0) == 0x80 || (frame & 0xFC) == 0x28)
					continue;
			}
		}

		objects[vy][vx] = frame;
	}
}

void WorldView::draw(Graphics::ManagedSurface &dst, const Tileset &tileset, const Data::WorldMap &world,
		const Data::Savegame &sg) const {
	byte objects[VIEW_TILES][VIEW_TILES] = {};
	byte ground[VIEW_TILES][VIEW_TILES];

	for (int y = 0; y < VIEW_TILES; ++y) {
		for (int x = 0; x < VIEW_TILES; ++x)
			ground[y][x] = world.getTile(sg._mapX - PARTY_TILE_X + x, sg._mapY - PARTY_TILE_Y + y);
	}

	placeObjects(sg, objects, ground);

	for (int y = 0; y < VIEW_TILES; ++y) {
		for (int x = 0; x < VIEW_TILES; ++x) {
			const int tile = objects[y][x] != 0 ? ACTOR_TILE_BASE + objects[y][x] : ground[y][x];
			tileset.drawTile(dst, VIEW_X + x * TILE_SIZE, VIEW_Y + y * TILE_SIZE, tile);
		}
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
