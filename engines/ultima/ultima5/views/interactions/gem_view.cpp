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

#include "ultima/ultima5/views/interactions/gem_view.h"
#include "ultima/ultima5/data/view_data.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

// The area the map is drawn in, and where the tiles of a town start in it
constexpr int AREA_LEFT = 8;
constexpr int AREA_TOP = 8;
constexpr int AREA_SIZE = 176;
constexpr int TOWN_TILES = 32;
constexpr int TOWN_LEFT = 0x20;
constexpr int TILE_PIXELS = 4;
constexpr int BRIGHT = 8;

// The dungeon map is made of cells of text
constexpr int DUNGEON_CELLS = 22;
constexpr int DUNGEON_START = 11;

// The kinds of tile there are, as the table of them gives
constexpr int KIND_TOWER = 0x10;

// The glyphs of the cells of the dungeon, which the runes have most of
constexpr byte GLYPH_PARTY = 0x60;
constexpr byte GLYPH_VISITED = 0x18;
constexpr byte GLYPH_LADDER_UP = 0x2E;
constexpr byte GLYPH_LADDER_DOWN = 0x2D;
constexpr byte GLYPH_LADDER = 0x2F;
constexpr byte GLYPH_CHEST = 0x70;
constexpr byte GLYPH_PIT_TEXT = 0x19;
constexpr byte GLYPH_PIT = 0x71;
constexpr byte GLYPH_BOMB_TEXT = 0x12;
constexpr byte GLYPH_TRAP = 0x72;
constexpr byte GLYPH_ROOM = 0x73;
constexpr byte GLYPH_SIGN_TEXT = 0x7F;
constexpr byte GLYPH_SIGN = 0x74;
constexpr byte GLYPH_SECRET = 0x75;
constexpr byte GLYPH_DOOR = 0x76;
constexpr byte GLYPH_DOORWAY = 0x77;

struct Colors {
	byte _red, _white, _blue, _green, _magenta, _yellow, _gray;
	bool _cga;
};

static Colors colors() {
	if (g_engine->_renderMode == Data::RENDER_CGA)
		return { 2, 3, 1, 1, 2, 3, 3, true };

	return { 4, 15, 1, 2, 5, 14, 7, false };
}

template<class S>
static void dot(S &s, int x, int y, byte color) {
	s.fillRect(Common::Rect(x, y, x + 1, y + 1), color);
}

template<class S>
static void block(S &s, int x0, int y0, int x1, int y1, byte color) {
	s.fillRect(Common::Rect(x0, y0, x1 + 1, y1 + 1), color);
}

// What colour the dots of water, or of other things that differ, are
static byte altColor(const Colors &c) {
	return c._cga ? c._magenta : c._blue + BRIGHT;
}

// Draws a tile of a town, in a block of four pixels square
template<class S>
static void drawTile(S &s, const Colors &c, int tile, int x, int y) {
	const byte green = c._green + BRIGHT;
	const byte kind = Data::VIEW_KIND[tile];

	switch (kind) {
	case 1:
		dot(s, x + 1, y, green);
		dot(s, x + 1, y + 2, green);
		dot(s, x + 3, y + 1, green);
		dot(s, x + 3, y + 3, green);
		break;

	case 2:
		block(s, x, y, x + 3, y + 3, green);
		break;

	case 3:
		block(s, x, y, x + 3, y + 3, c._red);
		break;

	case 4:
		s.drawLine(x, y, x + 3, y, c._white);
		s.drawLine(x, y + 3, x + 3, y + 3, c._white);
		break;

	case 5:
		s.drawLine(x + 1, y + 1, x + 2, y + 1, c._white);
		s.drawLine(x + 1, y + 2, x + 2, y + 2, c._white);
		break;

	case 6:
		s.drawLine(x, y, x + 3, y, c._white);
		s.drawLine(x, y + 3, x + 3, y + 3, c._white);
		s.drawLine(x, y + 1, x, y + 2, c._white);
		s.drawLine(x + 3, y + 1, x + 3, y + 2, c._white);
		break;

	case 7:
		block(s, x, y, x + 3, y + 3, c._white);
		break;

	case 8:
		s.drawLine(x, y, x + 1, y, c._yellow);
		s.drawLine(x, y + 1, x + 1, y + 1, c._yellow);
		s.drawLine(x + 2, y + 2, x + 3, y + 2, c._yellow);
		s.drawLine(x + 2, y + 3, x + 3, y + 3, c._yellow);
		break;

	case 9:
		s.drawLine(x, y, x + 3, y, green);
		s.drawLine(x, y + 2, x + 3, y + 2, green);
		dot(s, x + 2, y + 1, green);
		dot(s, x, y + 3, green);
		break;

	case 10: {
		// Roads show which way they lead
		byte dots[4];
		int bit = 8;

		for (int i = 0; i < 4; ++i, bit >>= 1)
			dots[i] = (tile & 0xF0) == 0x60 && (Data::VIEW_ROAD_BITS[tile & 0xF] & bit) == 0 ? green : altColor(c);

		dot(s, x + 1, y, dots[0]);
		dot(s, x + 3, y + 1, dots[1]);
		dot(s, x + 1, y + 2, dots[2]);
		dot(s, x + 3, y + 3, dots[3]);
		break;
	}

	case 11:
		dot(s, x, y, altColor(c));
		dot(s, x + 2, y + 2, altColor(c));
		break;

	case 12:
		dot(s, x + 2, y + 2, altColor(c));
		break;

	case 13:
		dot(s, x + 1, y, green);
		dot(s, x + 3, y + 1, green);
		dot(s, x, y + 2, altColor(c));
		dot(s, x + 2, y + 3, altColor(c));
		break;

	case 14:
		s.drawLine(x + 1, y, x + 1, y + 3, c._white);
		s.drawLine(x + 2, y, x + 2, y + 3, c._white);
		break;

	case 15:
		block(s, x, y, x + 3, y + 3, c._blue + BRIGHT);
		break;

	case KIND_TOWER: {
		dot(s, x + 1, y, green);
		dot(s, x + 1, y + 2, green);
		dot(s, x + 3, y + 1, green);
		dot(s, x + 3, y + 3, green);
		block(s, x + 1, y + 1, x + 2, y + 2, c._red);

		const byte walls = Data::VIEW_TOWER_BITS[tile - 0x20];
		if (walls & 8)
			s.drawLine(x + 1, y, x + 2, y, c._red);
		if (walls & 4)
			s.drawLine(x + 3, y + 1, x + 3, y + 2, c._red);
		if (walls & 2)
			s.drawLine(x + 1, y + 3, x + 2, y + 3, c._red);
		if (walls & 1)
			s.drawLine(x, y + 1, x, y + 2, c._red);

		switch (tile) {
		case 0x22:
			dot(s, x + 1, y + 2, 0);
			break;
		case 0x23:
			dot(s, x + 1, y + 1, 0);
			break;
		case 0x24:
			dot(s, x + 2, y + 1, 0);
			break;
		case 0x25:
			dot(s, x + 2, y + 2, 0);
			break;
		default:
			break;
		}
		break;
	}

	default:
		break;
	}
}

// Draws the map of a town, or of the outdoors around the party
template<class S>
static void drawTown(S &s, const Colors &c) {
	const Data::Savegame &sg = _G(savegame);
	const int left = sg._mapId == 0 ? sg._chunkX : 0;
	const int top = sg._mapId == 0 ? sg._chunkY : 0;

	for (int y = 0; y < TOWN_TILES; ++y) {
		for (int x = 0; x < TOWN_TILES; ++x)
			drawTile(s, c, g_engine->getMapTile(left + x, top + y), TOWN_LEFT + x * TILE_PIXELS, TOWN_LEFT + y * TILE_PIXELS);
	}

	// Where the party is
	const int px = TOWN_LEFT + ((sg._mapX - left) & 0xFF) * TILE_PIXELS;
	const int py = TOWN_LEFT + ((sg._mapY - top) & 0xFF) * TILE_PIXELS;
	block(s, px, py, px + 3, py + 3, c._white);
}

// Draws one cell of the dungeon map. Gives true if the cells around it are to be drawn
static bool drawDungeonCell(View &view, const Colors &c, byte *seen, int gx, int gy) {
	const Data::Savegame &sg = _G(savegame);
	auto s = view.getSurface();

	if (gx < 0 || gx >= DUNGEON_CELLS || gy < 0 || gy >= DUNGEON_CELLS || seen[gy * DUNGEON_CELLS + gx])
		return false;

	seen[gy * DUNGEON_CELLS + gx] = 1;

	const byte cell = DungeonLogic::cell((gx + sg._mapX - DUNGEON_START) & 7, (gy + sg._mapY - DUNGEON_START) & 7, sg._mapLevel);
	const int col = gx + 1;
	const int row = gy + 1;
	const int px = gx * 8 + 8;
	const int py = gy * 8 + 8;
	const byte gray = c._gray;
	const byte yellow = c._yellow;

	switch (cell >> 4) {
	case 0:
		if (cell & 8)
			view.drawCell(col, row, GLYPH_VISITED, gray, 0);
		break;

	case 1:
		view.drawRune(col, row, GLYPH_LADDER_UP, gray, 0);
		break;

	case 2:
		view.drawRune(col, row, GLYPH_LADDER_DOWN, gray, 0);
		break;

	case 3:
		view.drawRune(col, row, GLYPH_LADDER, gray, 0);
		break;

	case 4:
		view.drawRune(col, row, GLYPH_CHEST, yellow, 0);
		break;

	case 5:
		s.drawLine(px + 1, py + 4, px + 6, py + 4, c._white);
		s.drawLine(px + 2, py + 5, px + 5, py + 5, c._white);
		s.drawLine(px + 1, py + 6, px + 2, py + 6, c._white);
		s.drawLine(px + 5, py + 6, px + 6, py + 6, c._white);
		dot(s, px + 1, py + 2, c._blue + BRIGHT);
		dot(s, px + 2, py + 1, c._blue + BRIGHT);
		s.drawLine(px + 3, py + 2, px + 4, py + 2, c._blue + BRIGHT);
		s.drawLine(px + 3, py + 3, px + 4, py + 3, c._blue + BRIGHT);
		dot(s, px + 5, py + 1, c._blue + BRIGHT);
		dot(s, px + 6, py + 2, c._blue + BRIGHT);
		break;

	case 6:
		switch (cell) {
		case 0x60:
			view.drawCell(col, row, GLYPH_PIT_TEXT, gray, 0);
			break;
		case 0x61:
		case 0x69:
			view.drawRune(col, row, GLYPH_PIT, gray, 0);
			break;
		case 0x68:
			view.drawCell(col, row, GLYPH_BOMB_TEXT, gray, 0);
			break;
		default:
			view.drawRune(col, row, GLYPH_TRAP, c._red + BRIGHT, 0);
			break;
		}
		break;

	case 8: {
		const byte bars[4] = { (byte)(c._magenta + BRIGHT), (byte)(c._red + BRIGHT), (byte)(c._blue + BRIGHT), (byte)(c._green + BRIGHT) };

		for (int i = 0; i < 4; ++i) {
			s.drawLine(px + 1, py + i * 2, px + 6, py + i * 2, bars[i]);
			s.drawLine(px + 1, py + i * 2 + 1, px + 6, py + i * 2 + 1, bars[i]);
		}
		break;
	}

	case 0xA:
	case 0xF:
		view.drawRune(col, row, GLYPH_ROOM, yellow, 0);
		break;

	case 0xB:
		if (cell == 0xB0)
			view.drawCell(col, row, GLYPH_SIGN_TEXT, c._white, 0);
		else
			view.drawRune(col, row, GLYPH_SIGN, c._white, 0);
		return false;

	case 0xC:
		view.drawRune(col, row, GLYPH_SECRET, c._white, c._blue);
		return false;

	case 0xD:
		view.drawRune(col, row, GLYPH_DOOR, c._white, c._cga ? c._blue : c._gray);
		return false;

	case 0xE:
		view.drawRune(col, row, GLYPH_DOORWAY, yellow, 0);
		break;

	default:
		break;
	}

	return true;
}

// Draws the map of the level of a dungeon, spreading from the party until the walls
static void drawDungeon(View &view, const Colors &c) {
	byte seen[DUNGEON_CELLS * DUNGEON_CELLS];
	Common::Array<Common::Point> queue;

	memset(seen, 0, sizeof(seen));
	seen[DUNGEON_START * DUNGEON_CELLS + DUNGEON_START] = 1;
	queue.push_back(Common::Point(DUNGEON_START, DUNGEON_START));

	view.drawRune(DUNGEON_START + 1, DUNGEON_START + 1, GLYPH_PARTY, c._green + BRIGHT, 0);

	for (uint head = 0; head < queue.size(); ++head) {
		const Common::Point p = queue[head];

		for (int dy = -1; dy <= 1; ++dy) {
			for (int dx = -1; dx <= 1; ++dx) {
				if ((dx != 0 || dy != 0) && drawDungeonCell(view, c, seen, p.x + dx, p.y + dy))
					queue.push_back(Common::Point(p.x + dx, p.y + dy));
			}
		}
	}
}

void GemView::draw(View &view) {
	const Colors c = colors();

	{
		auto s = view.getSurface();
		block(s, AREA_LEFT, AREA_TOP, AREA_LEFT + AREA_SIZE - 1, AREA_TOP + AREA_SIZE - 1, 0);

		if (!DungeonLogic::isDungeon(_G(savegame)._mapId)) {
			drawTown(s, c);
			return;
		}
	}

	drawDungeon(view, c);
}

// The tiles shown about the party, and where its tile is among them
constexpr int EYE_TILES = 11;
constexpr int EYE_CENTRE = 5;
constexpr int EYE_ACTOR_BASE = 0x100;

void WizardEye::draw(View &view) {
	const Data::Savegame &sg = _G(savegame);
	auto s = view.getSurface();

	for (int y = 0; y < EYE_TILES; ++y) {
		for (int x = 0; x < EYE_TILES; ++x) {
			_G(tileset).drawTile(s, AREA_LEFT + x * Gfx::TILE_SIZE, AREA_TOP + y * Gfx::TILE_SIZE,
				g_engine->getMapTile(sg._mapX + x - EYE_CENTRE, sg._mapY + y - EYE_CENTRE));
		}
	}

	for (int i = 0; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord a = sg.getActor(i);
		const int x = (byte)(a._x - sg._mapX) + EYE_CENTRE;
		const int y = (byte)(a._y - sg._mapY) + EYE_CENTRE;

		if (a._tile == 0 || a._z != sg._mapLevel || x < 0 || y < 0 || x >= EYE_TILES || y >= EYE_TILES)
			continue;

		_G(tileset).drawTile(s, AREA_LEFT + x * Gfx::TILE_SIZE, AREA_TOP + y * Gfx::TILE_SIZE, EYE_ACTOR_BASE + a._animTile);
	}
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
