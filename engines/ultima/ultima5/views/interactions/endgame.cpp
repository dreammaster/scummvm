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

#include "ultima/ultima5/views/interactions/endgame.h"
#include "common/file.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int MAP_OFFSET = 0x210;
constexpr int MAP_SIZE = 11;
constexpr int MAP_STRIDE = 16;
constexpr int VIEW_X = 8;
constexpr int VIEW_Y = 8;
constexpr int ACTOR_TILE_BASE = 0x100;
constexpr int LORD_BRITISH = 31;
constexpr int BOX = 6;
constexpr byte TILE_LORD_BRITISH = 0x7C;
constexpr byte TILE_BOX = 0x0E;
constexpr byte TILE_GEM = 0x08;
constexpr byte TILE_FLOOR = 0x44;
constexpr byte TILE_GATE = 0xDC;
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 200;
constexpr int TEXT_COLUMNS = 40;
constexpr int CERTIFICATE_ROW = 1;

// Where the members of the party stand in front of him
static const byte STAND_X[6] = { 5, 4, 6, 3, 5, 7 };
static const byte STAND_Y[6] = { 5, 6, 6, 7, 7, 7 };

// What he says as he tells of the box, in turn
static const int STORY[6] = { 0xD3, 0x128, 0x167, 0x1C9, 0x211, 0x24B };

void Endgame::print(int offset) {
	_log.print(EndgameLogic::message(offset));
}

Endgame::Endgame(MessageLog &log) : Interaction(log) {
	Data::Savegame &sg = _G(savegame);
	Common::File f;

	if (f.open("miscmaps.dat") && f.seek(MAP_OFFSET))
		f.read(_map, sizeof(_map));

	_sprites[LORD_BRITISH]._tile = TILE_LORD_BRITISH;
	_sprites[LORD_BRITISH]._x = 5;
	_sprites[LORD_BRITISH]._y = 3;

	// The party comes in, and the dead among it are brought back
	for (int i = 0; i < sg._partySize && i < 6; ++i) {
		Data::Character &c = sg._party[i];

		if (c._status == Data::STATUS_DEAD) {
			_log.printChar('\n');
			_log.print(c._name);
			_log.print(" lives!\n");
			c._status = Data::STATUS_GOOD;
			c._hp = c._maxHp;
		}

		_sprites[i]._tile = EndgameLogic::memberTile(c._class);
		_sprites[i]._x = STAND_X[i];
		_sprites[i]._y = STAND_Y[i];
	}

	print(0);
	_log.print(sg._party[0]._name);
	_log.print("!\"\n\n");
	_state = STATE_GREETED;
}

void Endgame::askBox(int offset, State next) {
	print(offset);
	_state = next;
}

// What is done when he is told yes or no to his question
void Endgame::answered(bool yes) {
	const Data::Savegame &sg = _G(savegame);

	if (!yes && _state == STATE_BOX) {
		_log.print("No\n\n");
		askBox(0x49, STATE_BOX_AGAIN);
		return;
	}

	_log.print(yes ? "Yes\n\n" : "No\n\n");

	if (yes && sg._sandalwoodBox != 0) {
		_sprites[0]._x = 5;
		_sprites[0]._y = 5;
		_sprites[BOX]._tile = TILE_BOX;
		_sprites[BOX]._x = 5;
		_sprites[BOX]._y = 4;

		print(0xAB);
		_log.print("\n\nHe says:\n\n");
		_step = 0;
		_state = STATE_STORY;
		return;
	}

	// The box was not brought, and they are left to sit and wait
	_log.print("\"I see...\n");
	print(0x2D5);
	_sprites[0]._x = 8;
	_sprites[0]._y = 4;
	_sprites[2]._x = 8;
	_sprites[2]._y = 6;
	_sprites[LORD_BRITISH]._x = 4;
	_sprites[LORD_BRITISH]._y = 1;
	_state = STATE_REFUSED;
}

// The orb is opened, and they go through the gate it makes
void Endgame::ending() {
	for (int i = 0; i < 32; ++i)
		_sprites[i] = Sprite();

	_map[4 * MAP_STRIDE + 5] = TILE_FLOOR;
	_certificate = EndgameLogic::certificate();
	_state = STATE_CERTIFICATE;
}

bool Endgame::keypress(const KeypressMessage &msg) {
	const int key = toupper(msg.ascii);

	switch (_state) {
	case STATE_GREETED:
		askBox(0x21, STATE_BOX);
		return false;

	case STATE_BOX:
	case STATE_BOX_AGAIN:
		if (key != 'Y' && key != 'N')
			return false;

		answered(key == 'Y');
		return false;

	case STATE_STORY:
		if (_step < 6) {
			print(STORY[_step]);

			if (_step == 5)
				_sprites[BOX]._tile = TILE_GEM;

			++_step;
			return false;
		}

		ending();
		return false;

	case STATE_REFUSED:
		g_engine->quitGame();
		return true;

	default:
		g_engine->quitGame();
		return true;
	}
}

void Endgame::draw(View &view) {
	auto s = view.getSurface();

	if (_state == STATE_CERTIFICATE) {
		s.fillRect(Common::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 0);

		for (uint i = 0; i < _certificate.size(); ++i) {
			const EndgameLogic::Line &line = _certificate[i];
			const int column = (TEXT_COLUMNS - (int)line._text.size()) / 2;
			const byte fg = line._inverse ? 0 : 15;
			const byte bg = line._inverse ? 15 : 0;

			if (!line._runes) {
				view.print(column, CERTIFICATE_ROW + i, line._text, fg, bg);
				continue;
			}

			for (uint j = 0; j < line._text.size(); ++j)
				view.drawRune(column + j, CERTIFICATE_ROW + i, line._text[j], fg, bg);
		}
		return;
	}

	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x)
			_G(tileset).drawTile(s, VIEW_X + x * Gfx::TILE_SIZE, VIEW_Y + y * Gfx::TILE_SIZE, _map[y * MAP_STRIDE + x]);
	}

	for (int i = 0; i < 32; ++i) {
		if (_sprites[i]._tile != 0)
			_G(tileset).drawTile(s, VIEW_X + _sprites[i]._x * Gfx::TILE_SIZE, VIEW_Y + _sprites[i]._y * Gfx::TILE_SIZE,
				ACTOR_TILE_BASE + _sprites[i]._tile);
	}
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
