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

#include "ultima/ultima5/views/interactions/shrine.h"
#include "common/file.h"
#include "ultima/ultima5/data/shrine_data.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

// Where the maps of the shrines are in their file, and how they are laid out
constexpr int SHRINE_MAP_OFFSET = 0xB0;
constexpr int CODEX_MAP_OFFSET = 0x160;
constexpr int MAP_SIZE = 11;
constexpr int MAP_STRIDE = 16;
constexpr int VIEW_X = 8;
constexpr int VIEW_Y = 8;
constexpr int ACTOR_TILE_BASE = 0x100;
constexpr int AVATAR_TILE = 0x1C;
constexpr int KNEELING_TILE = 0x6C;
constexpr int AVATAR_COLUMN = 5;
constexpr int SHRINE_AVATAR_ROW = 6;
constexpr int CODEX_AVATAR_ROW = 3;
constexpr int MAX_TYPED = 12;
constexpr int MANTRA_COUNT = 3;
constexpr int VISIT_MINUTES = 16;
constexpr int FULL_CODEX = 0xFF;
constexpr int GOLD_PER_HUNDRED = 100;

// Where the words that the shrines say are, in what the game has of them
constexpr int TEXT_KNEEL = 0x36D;
constexpr int TEXT_MEDITATE = 0x398;
constexpr int TEXT_UNFOCUSED = 0x3C0;
constexpr int TEXT_ORDAINED = 0x3E1;
constexpr int TEXT_QUEST = 0x40E;
constexpr int TEXT_RETURN = 0x44B;
constexpr int TEXT_OFFER = 0x474;
constexpr int TEXT_POOR = 0x49B;
constexpr int TEXT_WELL_DONE = 0x4B9;
constexpr int TEXT_CODEX_OPEN = 0x4E5;
constexpr int TEXT_CODEX_PAGE = 0x515;
constexpr int TEXT_CODEX_NONE = 0x53E;
constexpr int TEXT_CODEX_WIND = 0x555;
constexpr int TEXT_APPROACH = 0x6AE;
constexpr int TEXT_CODEX_APPROACH = 0x6DB;
static const int TEXT_CODEX_READING[4] = { 0x57F, 0x5D1, 0x60C, 0x680 };

void Shrine::print(int offset) {
	_log.print(ShrineLogic::message(offset));
}

void Shrine::printRunes(int offset) {
	_log.setRunes(true);
	print(offset);
	_log.setRunes(false);
}

Shrine::Shrine(MessageLog &log, bool codex, bool stones) : Interaction(log), _codex(codex), _stones(stones) {
	const Data::Savegame &sg = _G(savegame);
	Common::File f;

	if (f.open("miscmaps.dat") && f.seek(codex ? CODEX_MAP_OFFSET : SHRINE_MAP_OFFSET))
		f.read(_map, sizeof(_map));

	_shrine = ShrineLogic::shrineAt(sg._mapX, sg._mapY);

	if (!codex) {
		print(TEXT_APPROACH);
		_kneeling = true;
		print(TEXT_KNEEL);
		askVirtue();
		return;
	}

	print(TEXT_CODEX_APPROACH);

	// Those who were left behind are in urns
	const int urns = ShrineLogic::urnCount();
	if (urns > 0) {
		_log.print("\n\nThou dost see\n");
		_log.print(urns == 1 ? "an urn marked:\n\n" : "urns marked:\n\n");
		_log.print(ShrineLogic::urnNames());
	}

	wait(STEP_CODEX_OPEN);
}

void Shrine::askVirtue() {
	print(TEXT_MEDITATE);
	_typed.clear();
	_state = STATE_VIRTUE;
}

void Shrine::askMantra() {
	_log.print("\nMantra:");
	_typed.clear();
	_state = STATE_MANTRA;
}

void Shrine::askOffer() {
	print(TEXT_OFFER);
	_state = STATE_OFFER;
}

void Shrine::wait(Step step) {
	_step = step;
	_state = STATE_WAIT;
}

void Shrine::finish() {
	_state = STATE_DONE;
	_kneeling = false;
	TimeLogic::advance(_G(savegame), VISIT_MINUTES);
}

// What the shrine does once the meditation has been right or wrong
void Shrine::meditated() {
	if (!_focused) {
		print(TEXT_UNFOCUSED);
		finish();
		return;
	}

	switch (ShrineLogic::stage(_shrine)) {
	case ShrineLogic::STAGE_ORDAIN:
		ShrineLogic::ordain(_shrine);
		print(TEXT_ORDAINED);
		_kneeling = false;
		wait(STEP_QUEST);
		break;

	case ShrineLogic::STAGE_OFFER:
		askOffer();
		break;

	default:
		print(TEXT_WELL_DONE);
		g_engine->sound().flare(0xC1C, 0x96);
		ShrineLogic::reward(_shrine, _log);
		finish();
		break;
	}
}

// Takes a key for a line that is typed, and says if the line is finished
bool Shrine::lineKey(const KeypressMessage &msg) {
	switch (msg.keycode) {
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
		return true;

	case Common::KEYCODE_ESCAPE:
		_typed.clear();
		return true;

	case Common::KEYCODE_BACKSPACE:
		if (!_typed.empty()) {
			_typed.deleteLastChar();
			_log.backspace(1);
		}
		return false;

	default:
		break;
	}

	if (msg.ascii >= ' ' && msg.ascii < 0x7F && (int)_typed.size() < MAX_TYPED) {
		_typed += (char)msg.ascii;
		_log.printChar(msg.ascii);
	}

	return false;
}

bool Shrine::codexStep() {
	switch (_step) {
	case STEP_CODEX_OPEN:
		print(TEXT_CODEX_OPEN);
		wait(STEP_CODEX_PAGE);
		return false;

	case STEP_CODEX_PAGE:
		print(TEXT_CODEX_PAGE);
		wait(STEP_CODEX_FIND);
		return false;

	case STEP_CODEX_FIND: {
		Data::Savegame &sg = _G(savegame);
		_answer = ShrineLogic::codexVirtue();

		if (_answer < 0) {
			print(TEXT_CODEX_NONE);
			finish();
			return true;
		}

		sg._shrineDone |= 1 << _answer;
		_log.printChar('"');
		print(Data::SHRINE_CODEX_TEXT[_answer]);
		_log.print("\"\n\n");
		wait(STEP_CODEX_ANSWER);
		return false;
	}

	case STEP_CODEX_ANSWER:
		if ((_G(savegame)._shrineDone & 0xFF) != FULL_CODEX) {
			finish();
			return true;
		}

		print(TEXT_CODEX_WIND);
		wait(STEP_CODEX_WIND);
		return false;

	case STEP_CODEX_WIND:
		_log.print("Thou dost read:\n\n");
		printRunes(TEXT_CODEX_READING[0]);
		_reading = 1;
		wait(STEP_CODEX_READ);
		return false;

	default:
		if (_reading < 4) {
			printRunes(TEXT_CODEX_READING[_reading++]);
			wait(STEP_CODEX_READ);
			return false;
		}

		finish();
		return true;
	}
}

// Takes the key that was waited for, and goes on to what follows
bool Shrine::waitKey() {
	switch (_step) {
	case STEP_QUEST:
		print(TEXT_QUEST);
		print(Data::SHRINE_QUEST_TEXT[_shrine]);
		_log.print("\"\n");
		wait(STEP_RETURN);
		return false;

	case STEP_RETURN:
		print(TEXT_RETURN);
		finish();
		return true;

	default:
		return codexStep();
	}
}

bool Shrine::keypress(const KeypressMessage &msg) {
	switch (_state) {
	case STATE_VIRTUE:
		if (!lineKey(msg))
			return false;

		if (_typed.empty()) {
			finish();
			return true;
		}

		_focused = ShrineLogic::find(Data::SHRINE_VIRTUE_WORDS[_shrine], _typed.c_str()) != -1;
		_log.printChar('\n');
		_mantras = 0;
		askMantra();
		return false;

	case STATE_MANTRA:
		if (!lineKey(msg))
			return false;

		if (_typed.empty()) {
			finish();
			return true;
		}

		if (ShrineLogic::find(Data::SHRINE_MANTRAS[_shrine], _typed.c_str()) == -1)
			_focused = false;

		if (++_mantras < MANTRA_COUNT) {
			askMantra();
			return false;
		}

		meditated();
		return _state == STATE_DONE;

	case STATE_OFFER: {
		if (msg.ascii < '0' || msg.ascii > '9')
			return false;

		_log.printChar(msg.ascii);
		const int hundreds = msg.ascii - '0';

		if (hundreds == 0) {
			_log.print(" gp\n");
			finish();
			return true;
		}

		_log.print("00 gp\n\n");

		if (!ShrineLogic::offer(_shrine, hundreds)) {
			print(TEXT_POOR);
			askOffer();
			return false;
		}

		_log.setRunes(true);
		_log.print("ALAKAZAM");
		_log.setRunes(false);
		_log.print("!\n");
		g_engine->sound().flare(0xA8C, 200);
		finish();
		return true;
	}

	case STATE_WAIT:
		return waitKey();

	default:
		return true;
	}
}

void Shrine::draw(View &view) {
	auto s = view.getSurface();

	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x)
			_G(tileset).drawTile(s, VIEW_X + x * Gfx::TILE_SIZE, VIEW_Y + y * Gfx::TILE_SIZE, _map[y * MAP_STRIDE + x]);
	}

	const int row = _codex ? CODEX_AVATAR_ROW : SHRINE_AVATAR_ROW;
	_G(tileset).drawTile(s, VIEW_X + AVATAR_COLUMN * Gfx::TILE_SIZE, VIEW_Y + row * Gfx::TILE_SIZE,
		ACTOR_TILE_BASE + (_kneeling ? KNEELING_TILE : AVATAR_TILE));
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
