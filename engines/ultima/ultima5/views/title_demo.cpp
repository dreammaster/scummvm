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

#include "ultima/ultima5/views/title_demo.h"
#include "common/file.h"
#include "common/system.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

// Where the script and the maps of the scenes are in miscmaps.dat
constexpr uint32 DATA_OFFSET = 0x2C0;
constexpr uint DATA_SIZE = 2000;
constexpr uint SCRIPT_START = 0x200;
constexpr int SCENE_COUNT = 4;
constexpr int SCENE_MAP_SIZE = 0x80;
constexpr int SCENE_COLUMNS = 0x13;
constexpr int ROWS = 4;
constexpr int MAP_STRIDE = 32;

// The part of the screen that the scenes are in, in the menu's place
constexpr int BAND_X = 8;
constexpr int BAND_Y = 128;
constexpr int TILE_SIZE = 16;
constexpr int BAND_WIDTH = SCENE_COLUMNS * TILE_SIZE;
constexpr int BAND_HEIGHT = ROWS * TILE_SIZE;
constexpr int SCREEN_WIDTH = 320;

constexpr int FRAME_MS = 50;
constexpr int ACTOR_COUNT = 32;
constexpr int ACTOR_TILE_BASE = 0x100;
constexpr byte TILE_CIRCLE = 0x16;
constexpr byte TILE_SKIP = 0xFE;
constexpr byte TILE_MOONGATE = 0xDC;
constexpr byte TILE_GRASS = 5;
constexpr int FIRST_ANIMATED_ACTOR = 0x34;
constexpr int REVEAL_PIXELS = 256;
constexpr int REVEAL_STEPS = 32;
constexpr int GATE_STEPS = 15;
constexpr int SLASH_STEPS = 5;
constexpr uint16 REVEAL_TAPS = 0xB8;
constexpr int SCENE_WATERFALL = 2;
constexpr int SCENE_CLOCK = 3;

static const char *const SCENE_NAMES[SCENE_COUNT] = { "The Summoning", "The Journey", "The Arrival", "The Welcoming" };
static const int8 MOVE_X[4] = { 0, 1, 0, -1 };
static const int8 MOVE_Y[4] = { -1, 0, 1, 0 };

// How many bytes follow each of the codes of the script
static const byte ARGUMENTS[16] = { 4, 1, 2, 1, 2, 0, 1, 1, 1, 0, 3, 2, 0, 2, 1, 0 };

enum Code {
	CODE_ACTOR, CODE_REMOVE, CODE_MOVE, CODE_WAIT, CODE_GATE_OPEN, CODE_GATE_CLOSE, CODE_SCENE, CODE_APPEAR,
	CODE_TERRAIN, CODE_END, CODE_TILE, CODE_SLASH, CODE_CLEAR, CODE_STEP, CODE_LOOP, CODE_AGAIN
};

bool TitleDemo::msgFocus(const FocusMessage &msg) {
	Common::File f;

	if (!loadTitleGraphics() || !f.open("miscmaps.dat") || !f.seek(DATA_OFFSET))
		error("Could not load the view");

	_script.clear();
	_script.resize(DATA_SIZE);
	memset(&_script[0], 0, _script.size());
	f.read(&_script[0], _script.size());

	// The shuffled order that the pixels of a tile come in
	uint16 seed = 1;
	_revealX[0] = _revealY[0] = 0;
	for (int i = 1; i < REVEAL_PIXELS; ++i) {
		_revealX[i] = seed >> 4;
		_revealY[i] = seed & 0xF;

		const bool carry = (seed & 1) != 0;
		seed >>= 1;
		if (carry)
			seed ^= REVEAL_TAPS;
	}

	_band.create(BAND_WIDTH, BAND_HEIGHT);
	_band.fillRect(Common::Rect(0, 0, BAND_WIDTH, BAND_HEIGHT), COLOR_BLACK);
	_cell.create(TILE_SIZE, TILE_SIZE);

	memset(_map, 0, sizeof(_map));
	memset(_copy, 0, sizeof(_copy));
	memset(_overlay, 0, sizeof(_overlay));
	for (int i = 0; i < ACTOR_COUNT; ++i)
		_actors[i] = Actor();

	expand();

	_eventIndex = 0;
	_wait = 0;
	_left = _right = 9;
	_widen = false;
	_scene = 0;
	_clockBeat = 0;
	_subtitleFrame = 0;
	_caption = nullptr;
	_nextFrame = g_system->getMillis();

	g_engine->cueMusic(Music::TRACK_THEME);

	delayFrames(1);
	return TitleBase::msgFocus(msg);
}

// Turns the script into the events that it comes to, which are the same each time that it goes round
void TitleDemo::expand() {
	byte copy[ROWS][MAP_STRIDE] = {};
	byte tile[ACTOR_COUNT] = {};
	byte x[ACTOR_COUNT] = {};
	byte y[ACTOR_COUNT] = {};
	int gateX = 0;
	int gateY = 0;
	int loopCount = 0;
	uint loopStart = 0;
	uint pos = SCRIPT_START;

	_events.clear();

	auto reveal = [&](int glyph, int cellX, int cellY, int steps) {
		for (int i = 0; i < steps; ++i)
			_events.push_back(Event(EVENT_REVEAL, glyph, cellX, cellY, (i * REVEAL_PIXELS / steps) | (((i + 1) * REVEAL_PIXELS / steps) << 8)));
	};

	while (pos < _script.size()) {
		const int code = _script[pos];
		if (code > CODE_AGAIN)
			break;

		auto arg = [&](int n) {
			return pos + n < _script.size() ? (int)_script[pos + n] : 0;
		};

		switch (code) {
		case CODE_ACTOR: {
			const int who = arg(1) & (ACTOR_COUNT - 1);
			tile[who] = arg(2);
			x[who] = arg(3);
			y[who] = arg(4);
			_events.push_back(Event(EVENT_ACTOR, who, tile[who], x[who], y[who]));
			break;
		}

		case CODE_REMOVE:
			_events.push_back(Event(EVENT_REMOVE, arg(1) & (ACTOR_COUNT - 1)));
			break;

		case CODE_MOVE:
		case CODE_STEP: {
			const int who = arg(1) & (ACTOR_COUNT - 1);
			const int dir = arg(2) & 3;

			_events.push_back(Event(EVENT_MOVE, who, dir));
			x[who] += MOVE_X[dir];
			y[who] += MOVE_Y[dir];

			if (code == CODE_STEP)
				_events.push_back(Event(EVENT_WAIT, 7));
			break;
		}

		case CODE_WAIT:
			_events.push_back(Event(EVENT_WAIT, arg(1)));
			break;

		case CODE_GATE_OPEN:
			gateX = arg(1);
			gateY = arg(2);
			_events.push_back(Event(EVENT_HIDE_CELL, gateX, gateY));
			reveal(TILE_MOONGATE, gateX, gateY, GATE_STEPS);
			_events.push_back(Event(EVENT_TILE, TILE_MOONGATE, gateX, gateY));
			_events.push_back(Event(EVENT_WAIT, 2));
			break;

		case CODE_GATE_CLOSE:
			_events.push_back(Event(EVENT_HIDE_CELL, gateX, gateY));
			reveal(TILE_GRASS, gateX, gateY, GATE_STEPS);
			_events.push_back(Event(EVENT_TILE, TILE_GRASS, gateX, gateY));
			_events.push_back(Event(EVENT_WAIT, 2));
			break;

		case CODE_SCENE: {
			const int scene = arg(1) & (SCENE_COUNT - 1);

			_events.push_back(Event(EVENT_SCENE, scene));
			for (int row = 0; row < ROWS; ++row) {
				for (int col = 0; col < SCENE_COLUMNS; ++col) {
					const uint at = scene * SCENE_MAP_SIZE + col + row * MAP_STRIDE;
					copy[row][col] = at < _script.size() ? _script[at] : 0;
				}
			}
			break;
		}

		case CODE_APPEAR: {
			const int who = arg(1) & (ACTOR_COUNT - 1);

			_events.push_back(Event(EVENT_SET_GLYPH, who, TILE_CIRCLE));
			reveal(ACTOR_TILE_BASE + tile[who], x[who], y[who], REVEAL_STEPS);
			_events.push_back(Event(EVENT_SET_GLYPH, who, tile[who]));
			break;
		}

		case CODE_TERRAIN: {
			const int who = arg(1) & (ACTOR_COUNT - 1);

			_events.push_back(Event(EVENT_SET_GLYPH, who, TILE_CIRCLE));
			reveal(copy[y[who] & 3][x[who] & 31], x[who], y[who], REVEAL_STEPS);
			_events.push_back(Event(EVENT_SET_GLYPH, who, tile[who]));
			break;
		}

		case CODE_END:
			_events.push_back(Event(EVENT_WAIT, 0));
			pos = _script.size();
			continue;

		case CODE_TILE: {
			const int cellX = arg(2) & 31;
			const int cellY = arg(3) & 3;

			copy[cellY][cellX] = arg(1);
			_events.push_back(Event(EVENT_TILE, arg(1), cellX, cellY));
			break;
		}

		case CODE_SLASH: {
			const int who = arg(2) & (ACTOR_COUNT - 1);

			for (int i = 0; i < SLASH_STEPS; ++i) {
				_events.push_back(Event(EVENT_WAIT, 1));
				_events.push_back(Event(EVENT_LINES, i));
			}

			_events.push_back(Event(EVENT_BLANK_CELL, x[who], y[who]));
			_events.push_back(Event(EVENT_NOISE));
			_events.push_back(Event(EVENT_WAIT, 3));
			break;
		}

		case CODE_CLEAR:
			_events.push_back(Event(EVENT_CLEAR_ACTORS));
			memset(tile, 0, sizeof(tile));
			break;

		case CODE_LOOP:
			loopCount = arg(1);
			loopStart = pos + 2;
			break;

		case CODE_AGAIN:
			if (--loopCount != 0) {
				pos = loopStart;
				continue;
			}
			break;
		}

		pos += 1 + ARGUMENTS[code];
	}
}

void TitleDemo::revealPixels(int glyph, int x, int y, int first, int last) {
	const int left = x * TILE_SIZE;
	const int top = y * TILE_SIZE;

	if (left < 0 || top < 0 || left + TILE_SIZE > BAND_WIDTH || top + TILE_SIZE > BAND_HEIGHT)
		return;

	// What the tile is, drawn over what's there, gives each of the pixels
	for (int row = 0; row < TILE_SIZE; ++row)
		memcpy(_cell.getBasePtr(0, row), _band.getBasePtr(left, top + row), TILE_SIZE);

	_G(tileset).drawTile(_cell, 0, 0, glyph);

	for (int i = first; i < last && i < REVEAL_PIXELS; ++i)
		*(byte *)_band.getBasePtr(left + _revealX[i], top + _revealY[i]) = *(const byte *)_cell.getBasePtr(_revealX[i], _revealY[i]);
}

void TitleDemo::apply(const Event &event) {
	switch (event._type) {
	case EVENT_SCENE:
		_scene = event._v0;
		_caption = SCENE_NAMES[_scene];

		for (int row = 0; row < ROWS; ++row) {
			for (int col = 0; col < SCENE_COLUMNS; ++col) {
				const uint at = _scene * SCENE_MAP_SIZE + col + row * MAP_STRIDE;
				_map[row][col] = _copy[row][col] = at < _script.size() ? _script[at] : 0;
			}
		}

		_left = _right = 9;
		_widen = false;
		break;

	case EVENT_ACTOR:
		_actors[event._v0]._tile = _actors[event._v0]._anim = event._v1;
		_actors[event._v0]._x = event._v2;
		_actors[event._v0]._y = event._v3;
		break;

	case EVENT_REMOVE: {
		Actor &actor = _actors[event._v0];

		// Whatever was under the actor is put back
		if (actor._x < MAP_STRIDE && actor._y < ROWS)
			_map[actor._y][actor._x] = _copy[actor._y][actor._x];

		actor._tile = actor._anim = 0;
		break;
	}

	case EVENT_MOVE: {
		Actor &actor = _actors[event._v0];

		if (actor._x < MAP_STRIDE && actor._y < ROWS)
			_map[actor._y][actor._x] = _copy[actor._y][actor._x];

		actor._x += MOVE_X[event._v1];
		actor._y += MOVE_Y[event._v1];
		break;
	}

	case EVENT_WAIT:
		_wait += event._v0;
		break;

	case EVENT_TILE:
		if (event._v1 < MAP_STRIDE && event._v2 < ROWS)
			_map[event._v2][event._v1] = _copy[event._v2][event._v1] = event._v0;
		break;

	case EVENT_CLEAR_ACTORS:
		for (int i = 0; i < ACTOR_COUNT; ++i)
			_actors[i]._tile = _actors[i]._anim = 0;
		break;

	case EVENT_HIDE_CELL:
		if (event._v0 < MAP_STRIDE && event._v1 < ROWS)
			_map[event._v1][event._v0] = _copy[event._v1][event._v0] = TILE_SKIP;
		break;

	case EVENT_SHOW_ACTOR:
		break;

	case EVENT_SET_GLYPH:
		_actors[event._v0]._tile = _actors[event._v0]._anim = event._v1;
		break;

	case EVENT_REVEAL:
		revealPixels(event._v0, event._v1, event._v2, event._v3 & 0xFF, event._v3 >> 8);
		_wait += 1;
		break;

	case EVENT_LINES: {
		// Two lines are drawn across, a stretch at a time
		const int i = event._v0;
		const int x = 0x78 + i * 9;
		const int y = 0x18 + i * 3;

		_band.drawLine(x, y, x + 9, y + 3, COLOR_WHITE);
		_band.drawLine(x, y + 1, x + 9, y + 4, COLOR_WHITE);
		break;
	}

	case EVENT_BLANK_CELL:
		_G(tileset).drawTile(_band, event._v0 * TILE_SIZE, event._v1 * TILE_SIZE, 0);
		break;

	case EVENT_NOISE:
		g_engine->sound().noise(1, 0x4B0, 4000);
		break;
	}
}

void TitleDemo::animateActors() {
	for (int i = 0; i < ACTOR_COUNT; ++i) {
		Actor &actor = _actors[i];

		if (actor._tile < FIRST_ANIMATED_ACTOR || actor._anim == TILE_CIRCLE || g_engine->getRandomNumber(1) != 0)
			continue;

		const int base = actor._tile & 0xFC;
		actor._anim = base + ((actor._anim - base + 1) & 3);
	}
}

void TitleDemo::drawCell(int x, int y) {
	const byte ground = _map[y][x];

	if (ground == TILE_SKIP)
		return;

	if (ground != 0) {
		_G(tileset).drawTile(_band, x * TILE_SIZE, y * TILE_SIZE, ground);
		return;
	}

	if (_overlay[y][x] == TILE_CIRCLE)
		return;

	_G(tileset).drawTile(_band, x * TILE_SIZE, y * TILE_SIZE, ACTOR_TILE_BASE + _overlay[y][x]);
}

// Goes through what happens in a frame of the scene
void TitleDemo::frame() {
	animateActors();
	_subtitleFrame = (_subtitleFrame + 1) & 3;

	for (int i = 0; i < ACTOR_COUNT; ++i) {
		const Actor &actor = _actors[i];

		if (actor._tile != 0 && actor._x < SCENE_COLUMNS && actor._y < ROWS) {
			_map[actor._y][actor._x] = 0;
			_overlay[actor._y][actor._x] = actor._anim;
		}
	}

	for (int x = _left; x <= _right && x < SCENE_COLUMNS; ++x) {
		for (int y = 0; y < ROWS; ++y)
			drawCell(x, y);
	}

	if (_left != 0 && _widen) {
		--_left;
		++_right;
	}

	_widen = !_widen;

	if (_scene == SCENE_WATERFALL) {
		g_engine->sound().noise(0x14, 0x3C, 10000);
	} else if (_scene == SCENE_CLOCK) {
		if (++_clockBeat > 7)
			_clockBeat = 0;

		if (_clockBeat == 0)
			g_engine->sound().tone(3000, 3);
		else if (_clockBeat == 4)
			g_engine->sound().tone(2000, 3);
	}
}

void TitleDemo::draw() {
	drawTitle(_subtitleFrame, _caption);

	auto s = getSurface();
	for (int y = 0; y < BAND_HEIGHT; ++y)
		memcpy(s.getBasePtr(BAND_X, BAND_Y + y), _band.getBasePtr(0, y), BAND_WIDTH);

	s.markAllDirty();
}

void TitleDemo::timeout() {
	const uint32 now = g_system->getMillis();

	if (now >= _nextFrame && !_events.empty()) {
		_nextFrame = now + FRAME_MS;

		// The script goes round again when it's done, and has something to wait for every time round
		for (uint guard = 0; _wait == 0 && guard <= _events.size(); ++guard) {
			if (_eventIndex >= _events.size())
				_eventIndex = 0;

			apply(_events[_eventIndex++]);
		}

		if (_wait > 0) {
			frame();
			--_wait;
		}

		redraw();
	}

	delayFrames(1);
}

bool TitleDemo::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	g_engine->sound().stop();
	close();
	return true;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
