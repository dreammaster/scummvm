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

#include "ultima/ultima5/views/interactions/capture.h"
#include "common/file.h"
#include "ultima/ultima5/logic/endgame_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int MAP_SIZE = 11;
constexpr int MAP_STRIDE = 16;
constexpr int VIEW_X = 8;
constexpr int VIEW_Y = 8;
constexpr int VIEW_PIXELS = 176;
constexpr int ACTOR_TILE_BASE = 0x100;
constexpr byte TILE_AVATAR = 0x1C;
constexpr byte TILE_CIRCLE = 0x16;
constexpr byte TILE_GUARD = 0x70;
constexpr byte TILE_BLACKTHORN = 0x78;
constexpr int ACTOR_GUARD_A = 6;
constexpr int ACTOR_GUARD_B = 7;
constexpr int ACTOR_BLACKTHORN = 8;
constexpr int ACTOR_SECOND = 1;
constexpr int QUESTIONS = 4;
constexpr int ANSWER_MAX = 14;
constexpr int PENDULUM_X = 5;
constexpr int PENDULUM_Y = 9;
constexpr byte PENDULUM_TILES[3] = { 0xEA, 0xEB, 0xE8 };
constexpr int SLICED_X = 5;
constexpr int SLICED_Y = 7;
constexpr byte TILE_BLOOD = 0x80;
constexpr int MINUTES_PER_ANSWER = 2;
constexpr int GENDER_MALE = 11;
constexpr int GENDER_FEMALE = 12;

// The text of the scene, by where it is in the file of it
constexpr int TEXT_THREAT_BEFORE = 0x229;
constexpr int TEXT_THREAT_AFTER = 0x25F;
constexpr int TEXT_ORDER = 0x32C;
constexpr int TEXT_ALONE_RIGHT = 0x29C;
constexpr int TEXT_ALONE_WRONG = 0x2DC;
constexpr int TEXT_SACRIFICE_RIGHT = 0x12D;
constexpr int TEXT_SACRIFICE_WRONG = 0xF1;
constexpr int TEXT_AFTER_SACRIFICE = 0x1A0;

void Capture::add(StepType type, int value) {
	Step step;
	step._type = type;
	step._value = value;

	if (_insert >= 0)
		_steps.insert_at(_insert++, step);
	else
		_steps.push_back(step);
}

void Capture::add(StepType type, const Common::String &text) {
	Step step;
	step._type = type;
	step._text = text;

	if (_insert >= 0)
		_steps.insert_at(_insert++, step);
	else
		_steps.push_back(step);
}

void Capture::addScript(const byte *script) {
	Step step;
	step._type = STEP_SCRIPT;
	step._script = script;

	if (_insert >= 0)
		_steps.insert_at(_insert++, step);
	else
		_steps.push_back(step);
}

void Capture::addPlace(int actor, int tile, int x, int y) {
	Step step;
	step._type = STEP_PLACE;
	step._value = actor;
	step._x = x;
	step._y = y;
	step._tile = tile;

	if (_insert >= 0)
		_steps.insert_at(_insert++, step);
	else
		_steps.push_back(step);
}

void Capture::addMap(int x, int y, int tile) {
	Step step;
	step._type = STEP_MAP;
	step._value = tile;
	step._x = x;
	step._y = y;

	if (_insert >= 0)
		_steps.insert_at(_insert++, step);
	else
		_steps.push_back(step);
}

Capture::Capture(MessageLog &log) : Interaction(log) {
	Data::Savegame &sg = _G(savegame);

	sg._partyTile = TILE_AVATAR;
	_log.print("\nThou art subdued and blindfolded!");

	_shrine = CaptureLogic::shrineToTake();
	_alive = CaptureLogic::aliveCount();

	if (_shrine >= 0)
		planScene();

	add(STEP_FINISH);
	run();
}

void Capture::planScene() {
	const Data::Savegame &sg = _G(savegame);

	add(STEP_BLACKOUT);
	add(STEP_WAIT, 8);
	add(STEP_PRINT, "\n\nStrong guards drag thee away!");
	add(STEP_WAIT, 4);
	add(STEP_CELL);
	add(STEP_PRINT, "\n\nThou hast been chained and manacled!");
	add(STEP_WAIT, 8);
	add(STEP_PRINT, "\n\nFootsteps!");
	add(STEP_WAIT, 8);

	addPlace(ACTOR_GUARD_A, TILE_GUARD, 4, 10);
	addPlace(ACTOR_GUARD_B, TILE_GUARD, 6, 10);
	addScript(CaptureLogic::SCRIPT_ARRIVE);
	add(STEP_WAIT, 4);
	addPlace(ACTOR_BLACKTHORN, TILE_CIRCLE, 5, 5);
	add(STEP_WAIT, 3);
	addPlace(ACTOR_BLACKTHORN, TILE_BLACKTHORN, 5, 5);
	add(STEP_WAIT, 4);

	add(STEP_PRINT, Common::String("\n\nBlackthorn says:\n\n\"Ah, ") + sg._party[0]._name +
		"!\n'Tis indeed an honour to meet thee at last! ");
	add(STEP_KEY);

	Common::String order = "\n\nGUARD! Release this good";
	if (sg._party[0]._gender == GENDER_FEMALE)
		order += " lady ";
	else if (sg._party[0]._gender == GENDER_MALE)
		order += "man ";
	add(STEP_PRINT, order + "at once!\"");
	addScript(CaptureLogic::SCRIPT_ORDER);
	add(STEP_PRINT, CaptureLogic::message(TEXT_ORDER));
	add(STEP_KEY);
	add(STEP_PRINT, "\n\n");
	add(STEP_QUIZ);
	add(STEP_LEAVE);
}

void Capture::loadCell() {
	Common::File f;

	memset(_map, 0, sizeof(_map));
	if (f.open("miscmaps.dat"))
		f.read(_map, sizeof(_map));

	for (int i = 0; i < 32; ++i)
		_sprites[i] = Sprite();

	placeParty();
	_screen = SCREEN_CELL;
}

void Capture::placeParty() {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < _alive && i < 6; ++i) {
		const int place = CaptureLogic::CHAIN_ORDER[(_alive - 1) * 8 + i];

		_sprites[i]._tile = EndgameLogic::memberTile(sg._party[i]._class);
		_sprites[i]._x = CaptureLogic::CHAIN_X[place];
		_sprites[i]._y = CaptureLogic::CHAIN_Y[place];
	}
}

void Capture::startScript(const byte *script) {
	_script = script;
	_repeat = 1;
	_pair = false;
	_sound = true;
	_moveLeft = 0;
}

int Capture::actorOf(byte code, int &dx, int &dy) {
	static const int ACTORS[5] = { ACTOR_GUARD_A, ACTOR_GUARD_B, ACTOR_BLACKTHORN, 0, ACTOR_SECOND };
	const int group = ((code & 0xFC) - 0x10) / 4;

	dx = dy = 0;
	switch (code & 3) {
	case 0:
		dy = -1;
		break;
	case 1:
		dx = 1;
		break;
	case 2:
		dy = 1;
		break;
	default:
		dx = -1;
		break;
	}

	return group >= 0 && group < 5 ? ACTORS[group] : 0;
}

void Capture::moveUnit() {
	_sprites[_first]._x += _firstX;
	_sprites[_first]._y += _firstY;

	if (_second >= 0) {
		_sprites[_second]._x += _secondX;
		_sprites[_second]._y += _secondY;
	}

	--_moveLeft;
}

void Capture::scriptTick() {
	if (_moveLeft > 0) {
		moveUnit();
		return;
	}

	while (_script) {
		const byte op = *_script++;

		if (op == 0) {
			_script = nullptr;
			return;
		}

		if (op < 0x10) {
			switch (op) {
			case 1:
				_pair = true;
				break;
			case 2:
				_repeat = *_script++;
				break;
			case 3:
				_sound = true;
				break;
			case 4:
				_sound = false;
				break;
			case 5:
				_delay = (*_script++ + 1) / 2;
				return;
			case 6: {
				const int tile = _script[0];
				const int x = _script[1];
				const int y = _script[2];
				_script += 3;

				if (x < MAP_SIZE && y < MAP_SIZE)
					_map[y * MAP_STRIDE + x] = tile;
				break;
			}
			case 8:
				_delay = _repeat;
				_repeat = 1;
				return;
			case 9:
				_sprites[*_script++]._tile = 0;
				break;
			default:
				break;
			}

			continue;
		}

		_first = actorOf(op, _firstX, _firstY);
		_second = -1;
		if (_pair)
			_second = actorOf(*_script++, _secondX, _secondY);

		_moveLeft = _repeat;
		_repeat = 1;
		_pair = false;

		if (_moveLeft <= 0)
			continue;

		if (!_sound) {
			while (_moveLeft > 0)
				moveUnit();
			continue;
		}

		moveUnit();
		return;
	}
}

void Capture::run() {
	while (_state == STATE_RUNNING && _delay == 0 && !_script && _next < _steps.size()) {
		const Step step = _steps[_next++];

		switch (step._type) {
		case STEP_PRINT:
			_log.print(step._text);
			break;
		case STEP_WAIT:
			_delay = step._value;
			break;
		case STEP_SCRIPT:
			startScript(step._script);
			break;
		case STEP_KEY:
			_state = STATE_KEY;
			break;
		case STEP_BLACKOUT:
			_screen = SCREEN_BLACK;
			break;
		case STEP_CELL:
			loadCell();
			break;
		case STEP_PLACE:
			_sprites[step._value]._tile = step._tile;
			_sprites[step._value]._x = step._x;
			_sprites[step._value]._y = step._y;
			break;
		case STEP_MAP:
			_map[step._y * MAP_STRIDE + step._x] = step._value;
			break;
		case STEP_QUIZ:
			_insert = _next;
			askQuestion();
			break;
		case STEP_LEAVE:
			if (_sprites[ACTOR_BLACKTHORN]._tile != 0) {
				_insert = _next;
				addScript(CaptureLogic::SCRIPT_LEAVE);
				_insert = -1;
			}
			break;
		case STEP_FINISH:
			CaptureLogic::release();
			_screen = SCREEN_GAME;
			_state = STATE_DONE;
			break;
		}
	}
}

void Capture::askQuestion() {
	_log.print(CaptureLogic::question(_question, _shrine));
	_log.print("\n\nYour response?\n:");
	_line.clear();
	_state = STATE_ANSWER;
}

// Has the second of the party who is fit be killed by the blade
void Capture::sliceOff(bool wrong) {
	add(STEP_PRINT, CaptureLogic::message(wrong ? TEXT_SACRIFICE_WRONG : TEXT_SACRIFICE_RIGHT));
	add(STEP_WAIT, 10);

	addPlace(ACTOR_SECOND, 0, 0, 0);
	addMap(SLICED_X, SLICED_Y, TILE_BLOOD);

	const Common::String name = CaptureLogic::sacrifice();

	if (wrong) {
		add(STEP_PRINT, Common::String("\n\n") + name + " is sliced in half! ");
		add(STEP_KEY);
		add(STEP_PRINT, CaptureLogic::message(TEXT_AFTER_SACRIFICE));
	}

	add(STEP_PRINT, "\n");
}

void Capture::endQuestions() {
	_insert = -1;
	_state = STATE_RUNNING;
	run();
}

void Capture::answered() {
	Data::Savegame &sg = _G(savegame);

	_insert = _next;
	_log.print("\n\n");

	if (CaptureLogic::isMantra(_line.text(), _shrine)) {
		CaptureLogic::takeShrine(_shrine);

		if (_alive > 1)
			sliceOff(false);
		else
			add(STEP_PRINT, CaptureLogic::message(TEXT_ALONE_RIGHT));

		add(STEP_KEY);
		addScript(CaptureLogic::SCRIPT_RELEASE);
		endQuestions();
		return;
	}

	if (_alive < 2) {
		add(STEP_PRINT, CaptureLogic::message(TEXT_ALONE_WRONG));
		add(STEP_KEY);
		addScript(CaptureLogic::SCRIPT_RELEASE);
		endQuestions();
		return;
	}

	if (_threatened) {
		TimeLogic::advance(sg, MINUTES_PER_ANSWER);

		if (_question < 3)
			addMap(PENDULUM_X, PENDULUM_Y, PENDULUM_TILES[_question]);
		else
			sliceOff(true);
	} else {
		_threatened = true;

		add(STEP_PRINT, CaptureLogic::message(TEXT_THREAT_BEFORE));
		addScript(CaptureLogic::SCRIPT_THREAT);
		add(STEP_PRINT, CaptureLogic::message(TEXT_THREAT_AFTER) + sg._party[1]._name + " die!\" ");
		add(STEP_KEY);
		add(STEP_PRINT, "\n\n");
	}

	if (++_question < QUESTIONS)
		add(STEP_QUIZ);

	endQuestions();
}

bool Capture::keypress(const KeypressMessage &msg) {
	switch (_state) {
	case STATE_KEY:
		_state = STATE_RUNNING;
		run();
		break;

	case STATE_ANSWER:
		if (_line.handleKey(msg, _log, ANSWER_MAX))
			answered();
		break;

	default:
		break;
	}

	return _state == STATE_DONE;
}

int Capture::tick() {
	if (_state == STATE_DONE)
		return TICK_DONE;
	if (_state != STATE_RUNNING)
		return TICK_NONE;

	if (_delay > 0)
		--_delay;
	else if (_script)
		scriptTick();

	run();
	return _state == STATE_DONE ? TICK_DONE : TICK_REDRAW;
}

void Capture::draw(View &view) {
	auto s = view.getSurface();

	if (_screen == SCREEN_BLACK) {
		s.fillRect(Common::Rect(VIEW_X, VIEW_Y, VIEW_X + VIEW_PIXELS, VIEW_Y + VIEW_PIXELS), 0);
		return;
	}

	if (_screen != SCREEN_CELL)
		return;

	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x)
			_G(tileset).drawTile(s, VIEW_X + x * Gfx::TILE_SIZE, VIEW_Y + y * Gfx::TILE_SIZE, _map[y * MAP_STRIDE + x]);
	}

	for (int i = 0; i < 32; ++i) {
		const Sprite &sprite = _sprites[i];

		if (sprite._tile != 0 && sprite._x >= 0 && sprite._x < MAP_SIZE && sprite._y >= 0 && sprite._y < MAP_SIZE)
			_G(tileset).drawTile(s, VIEW_X + sprite._x * Gfx::TILE_SIZE, VIEW_Y + sprite._y * Gfx::TILE_SIZE,
				ACTOR_TILE_BASE + sprite._tile);
	}
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
