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

#include "ultima/ultima5/views/dungeon_scene.h"
#include "ultima/ultima5/data/dungeon_data.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int WINDOW_LEFT = 0x10;
constexpr int WINDOW_TOP = 0x0E;
constexpr int WINDOW_RIGHT = 0xAF;
constexpr int WINDOW_BOTTOM = 0xB2;
constexpr int LAST_DEPTH = 4;
constexpr int MIRROR_AXIS = 0xBE;
constexpr int CENTRE_X = 0x60;
constexpr int FIRST_ITEM_FRAME = 0x1F;
constexpr byte DOOR_FEATURE_FRAME = 0x2F;

constexpr byte COLOR_BLACK_PEN = 0;
constexpr byte COLOR_BLUE_PEN = 1;
constexpr byte COLOR_BRIGHT_BLUE = 9;
constexpr byte COLOR_BRIGHT_GREEN = 10;
constexpr byte COLOR_BRIGHT_CYAN = 11;
constexpr byte COLOR_BRIGHT_RED = 12;
constexpr byte COLOR_BRIGHT_MAGENTA = 13;
constexpr byte COLOR_WHITE_PEN = 15;

// Sets a point of the picture
static void plot(Graphics::ManagedSurface *surface, int x, int y, byte color) {
	surface->fillRect(Common::Rect(x, y, x + 1, y + 1), color);
}

void DungeonScene::drawFrame(int frame, bool items, bool mirrored) {
	const int flags = mirrored ? Gfx::IMAGE_HFLIP : 0;

	if (frame < FIRST_ITEM_FRAME) {
		int x = Data::WALL_X[(mirrored ? 4 : 0) + (frame & 3)];

		if (mirrored && ((frame & 0xF8) == 8 || (frame & 0xFC) == 0x18))
			x = CENTRE_X;

		_walls.draw(*_surface, frame, x, WINDOW_TOP, flags);
	} else {
		const int image = ((frame + 1) >> 1) - 0x10;
		const int slot = ((frame + 1) & 6) >> 1;
		const int x = mirrored ? CENTRE_X : Data::ITEM_X[slot];
		int y;

		if (image < 8)
			y = items ? Data::ITEM_Y[4 + slot] : CENTRE_X;
		else
			y = Data::ITEM_Y[(items ? 4 : 0) + slot];

		_items.draw(*_surface, image, x, y, flags);
	}
}

// Moves a flame on a door a step, and shows it
int DungeonScene::drawFlame(int x, int y, int state, int) {
	if (state == 5)
		return 0;

	const byte color = state == 4 ? COLOR_BRIGHT_CYAN : COLOR_BLUE_PEN;
	_surface->hLine(x - 1, y, x + 1, color);
	_surface->vLine(x, y - 1, y + 1, color);

	if (state < 4)
		plot(_surface, x, y, COLOR_BRIGHT_BLUE);

	if (state != 0 || _G(gameRandom).range(0, 0x40) < 4)
		++state;

	return state;
}

void DungeonScene::drawSign(int number) {
	const int place = DungeonLogic::isDungeon(_G(savegame)._mapId) ? _G(savegame)._mapId - 0x21 : 0;

	if (number > Data::SIGN_COUNT[place])
		return;

	const int idx = Data::SIGN_FIRST[place] + number - 1;
	int col = Data::SIGN_LEFT[idx];
	int row = Data::SIGN_TOP[idx];

	for (const char *p = Data::SIGN_TEXT[idx]; *p; ++p) {
		if (*p == '\n') {
			++row;
			col = Data::SIGN_LEFT[idx];
		} else {
			_view->drawRune(col++, row, (byte)*p, COLOR_BLACK_PEN, COLOR_WHITE_PEN);
		}
	}
}

// The wandering monster of the dungeon, which moves about as it stands
void DungeonScene::drawMonster(int depth) {
	Data::Savegame &sg = _G(savegame);
	Data::ActorRecord monster = sg.getActor(1);

	if (monster._tile != _monsterTile) {
		const Common::String name = Common::String::format("mon%d", monster._tile);
		_monster.load(name.c_str(), g_engine->_renderMode);
		_monsterTile = monster._tile;
	}

	if (sg._icon == 'T')
		monster._unk6 = Data::WANDERER_MODE[monster._tile & 7];

	const bool upside = monster._skiffs == 0xFF;
	const int side = monster._unk6 & 0x90;
	const int mode = monster._unk6 & 0x60;
	int frame = monster._unk6 & 0xF;
	int left, right;

	if (sg._icon == 'T') {
		left = right = 1;
	} else {
		left = _G(gameRandom).range(0, 100) < 0x32;
		right = _G(gameRandom).range(0, 100) < 0x32;

		switch (mode) {
		case 0x20:
			left = right = frame = (frame == 0);
			break;

		case 0x60:
			if (--frame < 0)
				frame = 3;
			left = Data::MONSTER_CONTACT[frame] >> 1;
			right = Data::MONSTER_CONTACT[frame] & 1;
			break;
		}
	}

	if (side != 0)
		right = (side == 0x90) ^ left;

	const int y = Data::VIEW_MONSTER_Y[(upside ? 4 : 0) + depth];
	_monster.draw(*_surface, left * 3 + depth - 1, Data::VIEW_MONSTER_X[depth], y, upside ? Gfx::IMAGE_VFLIP : 0);
	_monster.draw(*_surface, right * 3 + depth - 1, CENTRE_X, y, (upside ? Gfx::IMAGE_VFLIP : 0) | Gfx::IMAGE_HFLIP);

	monster._unk6 = mode + side + frame;
	sg.setActor(1, monster);
}

// Draws what is in front of the party at a distance, and tells if the way goes on
bool DungeonScene::drawWall(int x, int y, int depth) {
	Data::Savegame &sg = _G(savegame);
	const byte c = DungeonLogic::seen(x, y);

	if (c < DungeonLogic::CELL_FIRST_WALL)
		return true;

	int frame = Data::WALL_FRAME[(c >> 4) - 0xA] + depth;
	if (depth == 0)
		frame = 0xC;

	drawFrame(frame, false, false);
	drawFrame(frame, false, true);

	if ((depth == 1 || depth == 2) && DungeonLogic::style() == 1 && (c & 0xF0) == DungeonLogic::CELL_DOOR_WALL) {
		const int state = c & 7;
		const int flame = drawFlame(0x5F, Data::DOOR_LIGHT_A[state + (depth - 1) * 5] + 0xE, state, depth);

		byte &stored = sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + (y & 7) * DUNGEON_SIZE + (x & 7)];
		stored = (stored & 0xF8) + flame;
	}

	if (depth == 1) {
		if ((c & 0xF0) == DungeonLogic::CELL_SIGN && (c & 0xF) != 0) {
			drawSign(c & 0xF);
		} else if (DungeonLogic::style() == 3 && (c & 0xF0) == DungeonLogic::CELL_DOOR_WALL && _G(gameRandom).range(0, 0x40) < 4) {
			_surface->hLine(0x5C, 0x57, 0x5D, COLOR_BRIGHT_RED);
			_surface->hLine(0x5B, 0x58, 0x5D, COLOR_BRIGHT_RED);
			_surface->hLine(0x61, 0x57, 0x62, COLOR_BRIGHT_RED);
			_surface->hLine(0x61, 0x58, 99, COLOR_BRIGHT_RED);
		}
	}

	return (c & 0xF0) == DungeonLogic::CELL_DOORWAY && depth == 0;
}

// The wall at the side of a cell, seen from the middle
void DungeonScene::drawSide(int x, int y, bool mirrored, int depth) {
	Data::Savegame &sg = _G(savegame);
	const byte c = DungeonLogic::seen(x, y);

	if (c < DungeonLogic::CELL_FIRST_WALL) {
		drawFrame(depth + 0x10, false, mirrored);
		return;
	}

	switch (c & 0xF0) {
	case DungeonLogic::CELL_DOOR_WALL: {
		drawFrame(depth + 0x14, false, mirrored);

		if (depth >= 2 || DungeonLogic::style() != 1)
			break;

		const int state = c & 7;
		int px = depth != 0 ? 0x43 : 0x21;
		if (mirrored)
			px = MIRROR_AXIS - px;

		const int flame = drawFlame(px, Data::DOOR_LIGHT_B[state + depth * 5] + 0xE, state, depth);

		byte &stored = sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + (y & 7) * DUNGEON_SIZE + (x & 7)];
		stored = (stored & 0xF8) + flame;
		break;
	}

	case DungeonLogic::CELL_FIRST_WALL:
	case DungeonLogic::CELL_DOORWAY:
	case DungeonLogic::CELL_ROOM:
		drawFrame(depth + 4, false, mirrored);
		break;

	default:
		drawFrame(depth, false, mirrored);
		break;
	}
}

// Flickering points where a wall torch is
void DungeonScene::drawTorchGlow(int depth, int frame) {
	switch (depth) {
	case 0:
		for (int i = 0; i < 8; ++i) {
			const int px = Data::FLAME_POINTS[frame * 16 + i * 2];
			const int py = Data::FLAME_POINTS[frame * 16 + i * 2 + 1];
			plot(_surface, px, py, COLOR_BRIGHT_BLUE);
			plot(_surface, MIRROR_AXIS - px, py, COLOR_BRIGHT_BLUE);
		}

		plot(_surface, Data::FAR_POINTS[frame * 2], Data::FAR_POINTS[frame * 2 + 1], COLOR_BRIGHT_BLUE);
		plot(_surface, MIRROR_AXIS - Data::FAR_POINTS[frame * 2], Data::FAR_POINTS[frame * 2 + 1], COLOR_BRIGHT_BLUE);
		break;

	case 1:
		for (int i = 0; i < 5; ++i) {
			const int px = Data::SHIMMER_POINTS[frame * 10 + i * 2];
			const int py = Data::SHIMMER_POINTS[frame * 10 + i * 2 + 1];
			plot(_surface, px + 0x48, py + 0x60, COLOR_BLUE_PEN);
			plot(_surface, 0x76 - px, py + 0x60, COLOR_BLUE_PEN);
		}
		break;

	case 2:
		for (int i = 0; i < 4; ++i) {
			const int px = Data::FIELD_POINTS[frame * 8 + i * 2];
			const int py = Data::FIELD_POINTS[frame * 8 + i * 2 + 1];
			plot(_surface, px + 0x50, py + 0x60, COLOR_BLUE_PEN);
			plot(_surface, 0x6E - px, py + 0x60, COLOR_BLUE_PEN);
		}
		break;

	case 3:
		plot(_surface, Data::NEAR_POINTS[frame * 2] + 0x58, Data::NEAR_POINTS[frame * 2 + 1] + 0x60, COLOR_BLUE_PEN);
		plot(_surface, 0x66 - Data::NEAR_POINTS[frame * 2], Data::NEAR_POINTS[frame * 2 + 1] + 0x60, COLOR_BLUE_PEN);
		break;
	}
}

// A field of energy, drawn as a spray of short lines
void DungeonScene::drawField(int depth, int kind) {
	static const byte COLORS[4] = { COLOR_BRIGHT_MAGENTA, COLOR_BRIGHT_GREEN, COLOR_BRIGHT_RED, COLOR_BRIGHT_BLUE };
	const byte color = COLORS[kind & 3];

	for (int i = 0; i < Data::SPARK_COUNT[depth]; ++i) {
		const int x = _G(gameRandom).range(Data::SPARK_FIRST[depth], Data::SPARK_LAST[depth] - Data::SPARK_WIDTH[depth]);
		const int y = _G(gameRandom).range(Data::SPARK_FIRST[depth], Data::SPARK_LAST[depth]);

		_surface->hLine(x, y, x + Data::SPARK_WIDTH[depth], color);
	}
}

// The things in a cell: ladders, chests, fountains and the like
void DungeonScene::drawFeatures(int x, int y, int depth) {
	const Data::Savegame &sg = _G(savegame);
	const byte c = DungeonLogic::seen(x, y);
	const int type = c >> 4;

	if (type > 0 && type < 8 && (type != 6 || (c & 7) == 0)) {
		if (Data::FEATURE_FRAME_LEFT[type] != 0) {
			drawFrame(Data::FEATURE_FRAME_LEFT[type] + depth * 2, true, false);
			drawFrame(Data::FEATURE_FRAME_LEFT[type] + depth * 2, true, true);
		}

		if (Data::FEATURE_FRAME_RIGHT[type] != 0) {
			drawFrame(Data::FEATURE_FRAME_RIGHT[type] + depth * 2, false, false);
			drawFrame(Data::FEATURE_FRAME_RIGHT[type] + depth * 2, false, true);
		}

		if (type == 5)
			drawTorchGlow(depth, _wobble);
	} else if (type == 8) {
		drawField(depth, c & 7);
	}

	if (type < 9 && (DungeonLogic::cell(x, y, sg._mapLevel) & 8) != 0) {
		drawFrame(DOOR_FEATURE_FRAME + depth * 2, true, false);
		drawFrame(DOOR_FEATURE_FRAME + depth * 2, true, true);
	}

	if (depth != 0) {
		const Data::ActorRecord monster = sg.getActor(1);
		if ((x & 7) == monster._x && (y & 7) == monster._y)
			drawMonster(depth);
	}

	if (depth == 0 && ++_wobble > 2)
		_wobble = 0;
}

void DungeonScene::draw(View &view) {
	const Data::Savegame &sg = _G(savegame);
	const int style = DungeonLogic::style();

	if (style != _style) {
		static const char *const WALL_FILES[] = { "", "dng1", "dng2", "dng3" };
		_walls.load(WALL_FILES[style], g_engine->_renderMode);
		_items.load("items", g_engine->_renderMode);
		_style = style;
	}

	auto s = view.getSurface();
	_view = &view;
	_surface = &s;

	s.fillRect(Common::Rect(WINDOW_LEFT, WINDOW_TOP, WINDOW_RIGHT + 1, WINDOW_BOTTOM + 1), COLOR_BLACK_PEN);

	// With no light there's nothing to be seen
	if (sg._unk0300 == 0 && sg._unk0301 == 0)
		return;

	const int facing = DungeonLogic::facing();
	static const int8 DX[4] = { 0, 1, 0, -1 };
	static const int8 DY[4] = { -1, 0, 1, 0 };
	const int dx = DX[facing];
	const int dy = DY[facing];
	const int sideX = Data::SIDE_X[facing];
	const int sideY = Data::SIDE_Y[facing];

	int x = sg._mapX;
	int y = sg._mapY;
	int depth;

	for (depth = 0; depth < LAST_DEPTH; ++depth) {
		if (!drawWall(x, y, depth))
			break;

		if (depth != 0 || (DungeonLogic::seen(sg._mapX, sg._mapY) != DungeonLogic::CELL_DOORWAY)) {
			drawSide(sideX + x, sideY + y, false, depth);
			drawSide(x - sideX, y - sideY, true, depth);
		}

		x += dx;
		y += dy;
	}

	// What is on the floor, from the farthest to the nearest
	for (--depth; depth >= 0; --depth) {
		x -= dx;
		y -= dy;
		drawFeatures(x, y, depth);
	}
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
