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

#include "ultima/ultima5/views/game.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/views/interactions/look.h"
#include "ultima/ultima5/views/interactions/new_order.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

// The id of the map of Britannia and the underworld
constexpr byte OUTDOOR_MAP = 0;

// How long a command takes
constexpr int MINUTES_PER_TURN = 2;

// Maps from the first dungeon up are not out in the open, and neither is a certain one
constexpr byte FIRST_DUNGEON_MAP = 0x21;
constexpr byte NO_WIND_MAP = 0x19;

// The windows on the right of the screen, in cells
constexpr int PANEL_COLUMN = 24;
constexpr int PANEL_WIDTH = 15;
constexpr int VISIBLE_PARTY_MEMBERS = 6;
constexpr int NAME_WIDTH = 9;
constexpr int STATUS_ROW = 8;
constexpr int DATE_ROW = 9;
constexpr int ICON_COLUMN = 30;
constexpr int ICON_ROW = 7;
constexpr int GOLD_COLUMN = 8;

// The message window, in pixels
constexpr int LOG_LEFT = 192;
constexpr int LOG_TOP = 88;
constexpr int LOG_BOTTOM = 192;

constexpr byte GLYPH_ACTIVE_MARKER = 0x1A;
constexpr byte GLYPH_CORNER_TOP_LEFT = 0x7B;
constexpr byte GLYPH_CORNER_TOP_RIGHT = 0x7C;
constexpr byte GLYPH_CORNER_BOTTOM_LEFT = 0x7D;

// The wind caption is in the bottom border, and is shown in place of the
// blank border below ground
constexpr int WIND_COLUMN = 6;
constexpr int WIND_ROW = 23;
static const char *const WIND_NAMES[] = { "Calm ", "North", "South", "East ", "West " };

// The map window, in pixels, and where the party is in it
constexpr int VIEW_X = 8;
constexpr int VIEW_Y = 8;
constexpr int PARTY_TILE_X = 5;
constexpr int PARTY_TILE_Y = 5;
constexpr int ACTOR_TILE_BASE = 0x100;

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

Game::Game() : View("Game"), _messages(this) {
	_messages.setBounds(Common::Rect(LOG_LEFT, LOG_TOP, 320, LOG_BOTTOM));
}

bool Game::msgFocus(const FocusMessage &msg) {
	if (_G(savegame)._mapId == OUTDOOR_MAP && !g_engine->loadWorld())
		error("Could not load the map");

	return View::msgFocus(msg);
}

void Game::draw() {
	drawFrame();

	if (_G(savegame)._mapId == OUTDOOR_MAP)
		drawWorld();

	drawParty();
	drawStatus();
	drawWind();

	if (_interaction)
		_interaction->draw(*this);
}

void Game::drawFrame() {
	auto s = getSurface();
	s.clear();

	s.fillRect(Common::Rect(0, 0, 320, 7), COLOR_BLUE);
	s.fillRect(Common::Rect(0, 185, 192, 192), COLOR_BLUE);
	s.fillRect(Common::Rect(0, 0, 7, 192), COLOR_BLUE);
	s.fillRect(Common::Rect(185, 0, 192, 192), COLOR_BLUE);
	s.fillRect(Common::Rect(313, 0, 320, 88), COLOR_BLUE);
	s.fillRect(Common::Rect(192, 80, 313, 88), COLOR_BLUE);
	s.fillRect(Common::Rect(192, 0x39, 313, 64), COLOR_BLUE);

	drawCell(0, 0, GLYPH_CORNER_TOP_LEFT, COLOR_BLUE, COLOR_BLACK);
	drawCell(39, 0, GLYPH_CORNER_TOP_RIGHT, COLOR_BLUE, COLOR_BLACK);
	drawCell(0, 23, GLYPH_CORNER_BOTTOM_LEFT, COLOR_BLUE, COLOR_BLACK);

	// The map, the party, the status line and the messages
	s.frameRect(Common::Rect(7, 7, 185, 185), COLOR_WHITE);
	s.drawLine(191, 191, 191, 87, COLOR_WHITE);
	s.drawLine(191, 87, 319, 87, COLOR_WHITE);
	s.frameRect(Common::Rect(191, 7, 313, 57), COLOR_WHITE);
	s.frameRect(Common::Rect(191, 63, 313, 81), COLOR_WHITE);
}

void Game::drawParty() {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < VISIBLE_PARTY_MEMBERS; ++i) {
		Common::String line;

		if (i < sg._partySize) {
			const Data::Character &c = sg._party[i];
			line = c._name;
			while ((int)line.size() < NAME_WIDTH)
				line += ' ';

			const bool active = i == sg._activeChar && c._status != Data::STATUS_DEAD &&
				c._status != Data::STATUS_SLEEP;
			line += active ? (char)GLYPH_ACTIVE_MARKER : ' ';
			line += Common::String::format("%4d", c._hp);
			line += (char)c._status;
		}

		while ((int)line.size() < PANEL_WIDTH)
			line += ' ';

		print(PANEL_COLUMN, 1 + i, line, COLOR_WHITE, COLOR_BLACK);
	}
}

void Game::drawStatus() {
	const Data::Savegame &sg = _G(savegame);

	// Food, then the gold lined up to its right
	Common::String food = Common::String::format("F:%d", sg._food);
	while ((int)food.size() < GOLD_COLUMN)
		food += ' ';

	Common::String gold;
	if (sg._gold < 1000)
		gold += ' ';
	if (sg._gold < 100)
		gold += ' ';
	if (sg._gold < 10)
		gold += ' ';
	gold += Common::String::format(" G:%d", sg._gold);

	Common::String line = food + gold;
	while ((int)line.size() < PANEL_WIDTH)
		line += ' ';
	print(PANEL_COLUMN, STATUS_ROW, line, COLOR_WHITE, COLOR_BLACK);

	// Month, day and year
	line = "   ";
	if (sg._month < 10 && sg._day < 10)
		line += ' ';
	line += Common::String::format("%d-%d-%03d", sg._month, sg._day, sg._year);
	while ((int)line.size() < PANEL_WIDTH)
		line += ' ';
	print(PANEL_COLUMN, DATE_ROW, line, COLOR_WHITE, COLOR_BLACK);

	// What the party is doing, in a cap-ended label over the border, or the plain border
	if (sg._icon != 0) {
		drawCaps(ICON_COLUMN * 8, ICON_ROW * 8, true);
		drawCell(ICON_COLUMN + 1, ICON_ROW, sg._icon, COLOR_WHITE, COLOR_BLACK);
		drawCaps((ICON_COLUMN + 2) * 8, ICON_ROW * 8, false);
	} else {
		auto s = getSurface();
		s.fillRect(Common::Rect(0xBF, 0x39, 0x139, 0x3F), COLOR_BLUE);
		s.hLine(0xC0, 0x38, 0x137, COLOR_WHITE);
		s.hLine(0xC0, 0x3F, 0x137, COLOR_WHITE);
	}
}

void Game::drawWind() {
	const Data::Savegame &sg = _G(savegame);
	if (sg._mapId >= FIRST_DUNGEON_MAP || sg._mapId == NO_WIND_MAP)
		return;

	if (sg._mapLevel < Data::MAP_LEVEL_UNDERGROUND) {
		const Common::String text = Common::String(WIND_NAMES[MIN<int>(sg._windDirection, 4)]) + " Winds";

		drawCaps(WIND_COLUMN * 8, WIND_ROW * 8, true);
		print(WIND_COLUMN + 1, WIND_ROW, text, COLOR_WHITE, COLOR_BLACK);
		drawCaps((WIND_COLUMN + 1 + text.size()) * 8, WIND_ROW * 8, false);
	} else {
		auto s = getSurface();
		s.hLine(0x30, 0xB8, 0x98, COLOR_WHITE);
		s.fillRect(Common::Rect(0x30, 0xB9, 0x99, 0xC0), COLOR_BLUE);
	}
}

// Whether an object is one that's hidden by some of the terrain it's on
static bool isHideable(byte tile) {
	return tile == 0x1C || (tile >= 0x12 && tile < 0x16) || tile >= 0x40 || (tile >= 0x28 && tile < 0x2C);
}

void Game::placeObjects(byte objects[VIEW_TILES][VIEW_TILES], byte ground[VIEW_TILES][VIEW_TILES]) {
	const Data::Savegame &sg = _G(savegame);

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

void Game::drawWorld() {
	Data::Savegame &sg = _G(savegame);
	WorldLogic::syncPartyObject(sg);

	byte objects[VIEW_TILES][VIEW_TILES] = {};
	byte ground[VIEW_TILES][VIEW_TILES];

	for (int y = 0; y < VIEW_TILES; ++y) {
		for (int x = 0; x < VIEW_TILES; ++x)
			ground[y][x] = _G(world).getTile(sg._mapX - PARTY_TILE_X + x, sg._mapY - PARTY_TILE_Y + y);
	}

	placeObjects(objects, ground);

	auto s = getSurface();
	for (int y = 0; y < VIEW_TILES; ++y) {
		for (int x = 0; x < VIEW_TILES; ++x) {
			const int tile = objects[y][x] != 0 ? ACTOR_TILE_BASE + objects[y][x] : ground[y][x];
			_G(tileset).drawTile(s, VIEW_X + x * Gfx::TILE_SIZE, VIEW_Y + y * Gfx::TILE_SIZE, tile);
		}
	}
}

void Game::startInteraction(Interactions::Interaction *interaction) {
	_interaction.reset(interaction);
	redraw();
}

void Game::endTurn() {
	TimeLogic::advance(_G(savegame), MINUTES_PER_TURN);
	redraw();
}

bool Game::handleCommand(const KeypressMessage &msg) {
	const bool outdoors = _G(savegame)._mapId == OUTDOOR_MAP;
	WorldLogic world(_messages);
	PartyLogic party(_messages);

	switch (msg.keycode) {
	case Common::KEYCODE_UP:
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_DOWN:
	case Common::KEYCODE_LEFT:
		if (!outdoors)
			return false;

		if (world.walk(msg.keycode == Common::KEYCODE_UP ? DIR_NORTH :
				msg.keycode == Common::KEYCODE_RIGHT ? DIR_EAST :
				msg.keycode == Common::KEYCODE_DOWN ? DIR_SOUTH : DIR_WEST))
			endTurn();
		else
			redraw();
		return true;

	case Common::KEYCODE_SPACE:
		world.pass();
		endTurn();
		return true;

	default:
		break;
	}

	switch (toupper(msg.ascii)) {
	case 'I':
		party.igniteTorch();
		endTurn();
		return true;

	case 'L':
		if (!outdoors)
			return false;

		_messages.print("Look-");
		startInteraction(new Interactions::Look(_messages));
		return true;

	case 'N':
		_messages.print("New Order");
		startInteraction(new Interactions::NewOrder(_messages));
		return true;

	default:
		return false;
	}
}

bool Game::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	if (_interaction) {
		if (_interaction->keypress(msg)) {
			_interaction.reset();
			endTurn();
		} else {
			redraw();
		}

		return true;
	}

	return handleCommand(msg);
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
