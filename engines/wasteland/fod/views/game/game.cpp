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

#include "wasteland/fod/views/game/game.h"
#include "wasteland/fod/fod.h"
#include "wasteland/keymapping.h"

namespace Wasteland {
namespace FOD {
namespace Views {

static const char *WEEKDAYS[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

Game::Game() : BaseView("Game") {
}

bool Game::msgFocus(const FocusMessage &msg) {
	setMode(MODE_NORMAL);
	Keymapping::setKeybindingMode(KBMODE_NORMAL);

	if (msg._priorView->getName() == "Title")
		_infoText = "Welcome to the beautiful island of Florida!\n";

	int mapNum = g_engine->_disk1._maps[g_engine->_disk1._mapIndex];

	g_engine->_disk.loadMap(mapNum);

	return true;
}

void Game::draw() {
	drawBorders(0);

	writeParty();
	writeTime();
	writeInfo();
	drawMap();
}

bool Game::msgKeypress(const KeypressMessage &msg) {
	assert(_mode == MODE_NORMAL);

	switch (msg.keycode) {
	case Common::KEYCODE_q:
		addView("QuitDialog");
		break;

	default:
		break;
	}

	return true;
}

bool Game::msgMouseDown(const MouseDownMessage &msg) {
	if (BaseView::msgMouseDown(msg))
		return true;

	for (uint partyNum = 0; partyNum < g_engine->_disk1._partyCount; ++partyNum) {
		const Common::Rect r(4 + partyNum * 28, 0, 52 + partyNum * 56, 24);
		if (r.contains(msg._pos)) {
			send("CharacterInfo", GameMessage("INFO", partyNum));
			return true;
		}
	}

	if (Common::Rect(280, 0, 320, 24).contains(msg._pos)) {
		addView("AllCharacterInfo");
		return true;
	}

	return false;
}

bool Game::msgAction(const ActionMessage &msg) {
	switch (msg._action) {
	case KEYBIND_VIEW_PARTY1:
	case KEYBIND_VIEW_PARTY2:
	case KEYBIND_VIEW_PARTY3:
	case KEYBIND_VIEW_PARTY4:
	case KEYBIND_VIEW_PARTY5: {
		int partyNum = msg._action - KEYBIND_VIEW_PARTY1;
		if (partyNum < g_engine->_disk1._partyCount)
			send("CharacterInfo", GameMessage("INFO", partyNum));
		return true;
	}

	case KEYBIND_VIEW_PARTY_ALL:
		addView("AllCharacterInfo");
		return true;

	case KEYBIND_NORTH:
		move(Logic::DIR_NORTH);
		break;
	case KEYBIND_SOUTH:
		move(Logic::DIR_SOUTH);
		break;
	case KEYBIND_EAST:
		move(Logic::DIR_EAST);
		break;
	case KEYBIND_WEST:
		move(Logic::DIR_WEST);
		break;

	default:
		break;
	}

	return false;
}

void Game::setMode(Mode newMode) {
	_mode = newMode;
	_infoText.clear();
}

void Game::writeParty() {
	for (uint partyNum = 0; partyNum < g_engine->_disk1._partyCount; ++partyNum) {
		const Data::PartyMember &member = g_engine->_disk1._party[partyNum];

		Gfx::Window win(1, 1, 6, 1);
		win.translate(7 * partyNum, 0);
		Surface s = getSurface(win);

		if (member._afflicted) {
			s.clear(15);
			s.setInverseColor(true);
		}

		Common::String tmp = (member._name.size() <= 6) ? member._name :
			Common::String(member._name.c_str(), 6);
		s.writeCenteredString(tmp, 0);
	}
}

void Game::writeTime() {
	Surface s = getSurface(Gfx::Window(1, 22, 5, 23));
	s.writeString(WEEKDAYS[g_engine->_disk1._timeWeekday], 1, 0);
	s.writeString(Common::String::format("%2.2d:%.2d", g_engine->_disk1._timeHours, g_engine->_disk1._timeMinutes),
		0, 1);
}

void Game::writeInfo() {
	Surface s = getSurface(Gfx::Window(7, 22, 39, 24));
	s.writeString(_infoText);
}

void Game::drawMap() {
	Surface s = getSurface(Gfx::Window(1, 3, 39, 21));
	Data::Map &map = g_engine->_disk._map;
	int defaultTile = map._flags & ~Data::MAPFLAG_8000;
	int mapOffsetX = g_engine->_disk1._mapPosX - 9;
	int mapOffsetY = g_engine->_disk1._mapPosY - 4;
	int tileId;

	for (int mapY = 0; mapY < 9; ++mapY) {
		int mapCurrentY = mapOffsetY + mapY;

		for (int mapX = 0; mapX < 19; ++mapX) {
 			int mapCurrentX = mapOffsetX + mapX;

			if (mapCurrentX >= map._width || mapCurrentY >= map._height ||
					mapCurrentX < 0 || mapCurrentY < 0) {
				tileId = defaultTile;
			} else {
				const Data::Map::MapTile &tile = map._tiles[mapCurrentY * map._width + mapCurrentX];

				tileId = tile._id;
				if (tile._flags & 0x40) {
					tileId = (tileId & 0x7ff) | 0x1800;
				} else if (tile._flags & 0x80) {
					tileId = (tileId & 0x7ff) | 0x1000;
				}
			}

			drawTileAt(s, mapX, mapY, tileId);
		}
	}

	map._mapState &= 0x7ff;
	map._mapState |= 0x800;
}

void Game::drawTileAt(Surface &s, int mapX, int mapY, int tileId) {
	int index = tileId & 0x7ff;
	g_engine->_tiles.drawTile(&s, index, mapX * TILE_W, mapY * TILE_H);

	if (tileId & 0xf800) {
		// Drawing party icon. The paragraph represents the icon offset * 16.
		// And since each icon is 128 bytes, the index can be calculated
		int iconParagraph = _personIcons[((tileId & 0xf800) >> 8) >> 3];
		assert((iconParagraph % (128 / 16)) == 0);
		index = iconParagraph / (128 / 16);

		g_engine->_tiles.drawTile(&s, index, mapX * TILE_W, mapY * TILE_H);
	}
}

void Game::move(Logic::Direction dir) {
	_infoText.clear();
	Logic::Game::move(dir, false);

	redraw();
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
