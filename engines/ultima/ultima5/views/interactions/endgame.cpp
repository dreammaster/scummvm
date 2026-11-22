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

// The slides that follow, each a picture from one of the two end files beside text from end.dat. The text
// has two regions, the second being for the lines between the split limits
constexpr int SLIDE_COUNT = 6;
constexpr int SLIDE_TEXT_LENGTH = 2000;
constexpr int SCROLL_X = 0x28;
constexpr int TEXT_PICTURE = 0;
static const byte SLIDE_LEFT_A[SLIDE_COUNT] = { 0xAC, 0x00, 0x00, 0xB3, 0x00, 0x00 };
static const byte SLIDE_LEFT_B[SLIDE_COUNT] = { 0x00, 0x00, 0xC4, 0x00, 0xA1, 0x00 };
static const uint16 SLIDE_RIGHT_A[SLIDE_COUNT] = { 0x140, 0x140, 0x140, 0x140, 0x140, 0x9A };
static const uint16 SLIDE_RIGHT_B[SLIDE_COUNT] = { 0x140, 0x140, 0x140, 0x140, 0x140, 0x140 };
static const byte SLIDE_SPLIT_TOP[SLIDE_COUNT] = { 0x7E, 0x7E, 0x2A, 0x64, 0x52, 0x70 };
static const byte SLIDE_SPLIT_BOTTOM[SLIDE_COUNT] = { 0xC8, 0xC8, 0x94, 0xC8, 0xC8, 0xC8 };
static const byte SLIDE_START_X[SLIDE_COUNT] = { 0xAC, 0x00, 0x00, 0xB3, 0x00, 0x00 };
static const byte SLIDE_START_Y[SLIDE_COUNT] = { 0x42, 0x5C, 0x09, 0x26, 0x09, 0x00 };
static const byte SLIDE_PICTURE[SLIDE_COUNT] = { 0, 1, 2, 0, 1, 2 };
static const byte SLIDE_FILE[SLIDE_COUNT] = { 0, 0, 0, 1, 1, 1 };
static const byte SLIDE_PICTURE_X[SLIDE_COUNT] = { 0x00, 0x40, 0x00, 0x00, 0x00, 0xA0 };
static const byte SLIDE_PICTURE_Y[SLIDE_COUNT] = { 0x00, 0x00, 0x34, 0x00, 0x5C, 0x00 };
static const bool SLIDE_BANNERS[SLIDE_COUNT] = { true, false, false, true, false, false };
static const uint16 SLIDE_TEXT_OFFSET[SLIDE_COUNT] = { 0x0000, 0x01A8, 0x03BC, 0x05FA, 0x08E8, 0x0B74 };

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

	g_engine->cueReunion();

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
	g_engine->cueMusic(Music::TRACK_RULEBRIT);
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
	_slide = 0;
	_state = loadSlides() ? STATE_SLIDES : STATE_CERTIFICATE;
	cue();
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

	case STATE_SLIDES:
		if (++_slide >= SLIDE_COUNT)
			_state = STATE_CERTIFICATE;

		cue();
		return false;

	case STATE_REFUSED:
		g_engine->quitGame();
		return true;

	default:
		g_engine->quitGame();
		return true;
	}
}

// The music that goes with what is shown: while a slide is up, it is the tune of the one that follows
void Endgame::cue() {
	if (_state == STATE_CERTIFICATE) {
		g_engine->cueMusic(Music::TRACK_RULEBRIT);
		return;
	}

	g_engine->cueMusic(Music::trackForEndingScene(_slide + 1));
}

bool Endgame::loadSlides() {
	Common::File f;

	if (f.open("end.dat")) {
		_slideText.resize(f.size());
		f.read(&_slideText[0], _slideText.size());
	}

	const Data::RenderMode mode = g_engine->_renderMode;
	_slidesLoaded = !_slideText.empty() && _font.load() && _slideImages[0].load("end1", mode) &&
		_slideImages[1].load("end2", mode) && _banners.load("text", mode) && _scroll.load("endsc", mode);

	return _slidesLoaded;
}

Common::String Endgame::slideText(int slide) const {
	Common::String result;

	for (uint i = SLIDE_TEXT_OFFSET[slide]; i < _slideText.size() && i < (uint)SLIDE_TEXT_OFFSET[slide] + SLIDE_TEXT_LENGTH &&
			_slideText[i] != 0; ++i)
		result += (char)_slideText[i];

	return result;
}

void Endgame::drawSlide(View &view) {
	auto s = view.getSurface();
	const int n = _slide;

	s.fillRect(Common::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 0);

	// The first and fourth have the title lettering above their text
	if (SLIDE_BANNERS[n]) {
		if (n == 0) {
			_banners.draw(s, TEXT_PICTURE, 0xD8, 0);
			_banners.draw(s, 4, 0x98, 0x1C);
		} else {
			_banners.draw(s, 5, 0xE0, 0);
			_banners.draw(s, TEXT_PICTURE, 0xB0, 0);
		}
	}

	_slideImages[SLIDE_FILE[n]].draw(s, SLIDE_PICTURE[n], SLIDE_PICTURE_X[n], SLIDE_PICTURE_Y[n]);

	Gfx::StoryLayout layout;
	layout._left[0] = SLIDE_LEFT_A[n];
	layout._left[1] = SLIDE_LEFT_B[n];
	layout._right[0] = SLIDE_RIGHT_A[n];
	layout._right[1] = SLIDE_RIGHT_B[n];
	layout._splitTop = SLIDE_SPLIT_TOP[n];
	layout._splitBottom = SLIDE_SPLIT_BOTTOM[n];

	int x = SLIDE_START_X[n];
	int y = SLIDE_START_Y[n];
	_font.draw(s, slideText(n), x, y, layout);
}

// Draws the certificate on the scroll, in letters that are dark on the scroll and light off it
void Endgame::drawCertificate(View &view) {
	auto s = view.getSurface();

	s.fillRect(Common::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 0);

	if (_slidesLoaded)
		_scroll.draw(s, 0, SCROLL_X, 0);

	for (uint i = 0; i < _certificate.size(); ++i) {
		const EndgameLogic::Line &line = _certificate[i];
		const int column = (TEXT_COLUMNS - (int)line._text.size()) / 2;
		const byte color = line._inverse ? 0 : 15;
		Gfx::CharSet *font = line._runes ? g_engine->_runes.get() : g_engine->_charset.get();

		for (uint j = 0; j < line._text.size(); ++j)
			font->drawChar(s.surfacePtr(), (byte)line._text[j], (column + j) * 8, (CERTIFICATE_ROW + i) * 8, color);
	}
}

void Endgame::draw(View &view) {
	if (_state == STATE_SLIDES) {
		drawSlide(view);
		return;
	}

	if (_state == STATE_CERTIFICATE) {
		drawCertificate(view);
		return;
	}

	auto s = view.getSurface();

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
