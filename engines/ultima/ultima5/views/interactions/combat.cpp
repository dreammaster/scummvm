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

#include "ultima/ultima5/views/interactions/combat.h"
#include "ultima/ultima5/views/interactions/ready.h"
#include "ultima/ultima5/views/interactions/use.h"
#include "ultima/ultima5/views/interactions/ztats.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int KEY_ESCAPE = 27;
constexpr int KEY_ENTER = 13;
constexpr int KEY_SPACE = ' ';
constexpr int KEY_HOME = 0xD3;
constexpr int KEY_END = 0xD4;
constexpr int KEY_PAGE_UP = 0xD5;
constexpr int KEY_PAGE_DOWN = 0xD6;

constexpr int VIEW_X = 8;
constexpr int VIEW_Y = 8;
constexpr int ACTOR_TILE_BASE = 0x100;
constexpr byte ACTOR_REMAINS = 0xE8;
constexpr byte ACTOR_SLEEPING = 0x1E;
constexpr byte ACTOR_DEAD = 0x1F;
constexpr byte COLOR_AIM = 15;

static const char *const MAP_FILE = "brit.cbt";

Combat::Combat(MessageLog &log) : Interaction(log), _logic(log) {
}

Combat::~Combat() {
	delete _next;
}

bool Combat::startDungeon(const DungeonLogic::Event &event) {
	_event = event;
	_dungeon = true;

	if (!DungeonLogic(_log).prepareFight(_logic, _event))
		return false;

	if (carryOn())
		_state = STATE_DONE;

	return true;
}

bool Combat::start(int kind, int map, bool lone) {
	if (!_logic.loadMap(MAP_FILE, map * COMBAT_MAP_BYTES))
		return false;

	_logic.begin(kind, lone, CombatLogic::KIND_NORMAL, _G(savegame)._mapId);
	if (carryOn())
		_state = STATE_DONE;

	return true;
}

bool Combat::startCamp(int guard, int hours, bool &apparition) {
	if (!_logic.loadMap(MAP_FILE, 0))
		return false;

	const CombatLogic::CampResult result = _logic.camp(guard, hours, _G(savegame)._mapId, apparition);
	if (result != CombatLogic::CAMP_AMBUSHED || carryOn())
		_state = STATE_DONE;

	return true;
}

Combat *Combat::forObject(MessageLog &log, int object) {
	AttackLogic::Fight fight;
	fight._started = true;
	fight._actor = object;
	AttackLogic(log).prepare(fight);

	Combat *combat = new Combat(log);
	combat->setAftermath(fight);

	if (!combat->start(fight._kind, fight._map, fight._lone) || combat->done()) {
		delete combat;
		return nullptr;
	}

	return combat;
}

bool Combat::carryOn() {
	if (_logic.advance() == CombatLogic::STATE_FINISHED) {
		_logic.finish();

		if (_hasFight)
			AttackLogic::afterFight(_fight, _logic.won());

		if (_dungeon) {
			// What the fight led to may be a room, with another fight
			const DungeonLogic::Event next = DungeonLogic(_log).finishFight(_logic, _event);

			if (next._kind != DungeonLogic::Event::EVENT_NONE) {
				Combat *combat = new Combat(_log);

				if (combat->startDungeon(next) && !combat->done())
					_next = combat;
				else
					delete combat;
			}
		}

		return true;
	}

	_state = STATE_COMMAND;
	return false;
}

bool Combat::handle(CombatLogic::Outcome outcome, int key) {
	switch (outcome) {
	case CombatLogic::OUTCOME_AIM:
		_state = STATE_AIM;
		return false;

	case CombatLogic::OUTCOME_CHOICE:
		_state = STATE_CHOICE;
		return false;

	case CombatLogic::OUTCOME_DIRECTION:
		_state = STATE_DIRECTION;
		return false;

	case CombatLogic::OUTCOME_SUBCOMMAND:
		_subKey = key;
		_state = STATE_SUB;

		if (key == 'R') {
			_sub.reset(new Ready(_log));
		} else if (key == 'Z') {
			_sub.reset(new Ztats(_log));
		} else {
			Use *use = new Use(_log);
			if (use->done()) {
				delete use;
				_logic.commandDone(key);
				return carryOn();
			}

			_sub.reset(use);
		}
		return false;

	default:
		return carryOn();
	}
}

bool Combat::keypress(const KeypressMessage &msg) {
	if (_state == STATE_SUB) {
		if (!_sub->keypress(msg))
			return false;

		_sub.reset();
		_logic.commandDone(_subKey);
		return carryOn();
	}

	const int key = originalKey(msg);

	if (_state == STATE_CHOICE) {
		if (key == CombatLogic::KEY_UP || key == 'U')
			return handle(_logic.klimbChosen(false), key);
		if (key == CombatLogic::KEY_DOWN || key == 'D')
			return handle(_logic.klimbChosen(true), key);
		return false;
	}

	if (_state == STATE_DIRECTION) {
		int dx = 0;
		int dy = 0;

		switch (key) {
		case CombatLogic::KEY_UP:
			dy = -1;
			break;
		case CombatLogic::KEY_DOWN:
			dy = 1;
			break;
		case CombatLogic::KEY_RIGHT:
			dx = 1;
			break;
		case CombatLogic::KEY_LEFT:
			dx = -1;
			break;
		case KEY_SPACE:
			return handle(_logic.klimbDirection(0, 0, true), key);
		default:
			return false;
		}

		return handle(_logic.klimbDirection(dx, dy, false), key);
	}

	if (_state == STATE_AIM) {
		int dx = 0;
		int dy = 0;

		switch (key) {
		case KEY_HOME:
			dx = dy = -1;
			break;
		case KEY_PAGE_DOWN:
			dx = dy = 1;
			break;
		case KEY_PAGE_UP:
			dx = 1;
			dy = -1;
			break;
		case KEY_END:
			dx = -1;
			dy = 1;
			break;
		case CombatLogic::KEY_UP:
			dy = -1;
			break;
		case CombatLogic::KEY_DOWN:
			dy = 1;
			break;
		case CombatLogic::KEY_RIGHT:
			dx = 1;
			break;
		case CombatLogic::KEY_LEFT:
			dx = -1;
			break;

		case KEY_SPACE:
			if (_logic.aimOnSelf())
				return handle(_logic.aimCancel(), key);
			// Fall through

		case KEY_ENTER:
		case 'A':
			if (!_logic.aimOnSelf() && _logic.aimAccept())
				return handle(_logic.aimFire(), key);
			break;

		case KEY_ESCAPE:
			return handle(_logic.aimCancel(), key);

		default:
			break;
		}

		_logic.aimMove(dx, dy);
		return false;
	}

	return handle(_logic.command(key), key);
}

void Combat::draw(View &view) {
	if (_state == STATE_SUB) {
		_sub->draw(view);
		return;
	}

	auto s = view.getSurface();
	byte objects[COMBAT_SIZE][COMBAT_SIZE] = {};

	for (int idx = COMBAT_ENTITIES - 1; idx >= 0; --idx) {
		const Data::ActorRecord &a = _logic.actor(idx);
		if (a._tile == 0 || a._animTile == 0 || a._x >= COMBAT_SIZE || a._y >= COMBAT_SIZE)
			continue;

		byte &cell = objects[a._y][a._x];
		if ((a._tile & 0xFC) == ACTOR_REMAINS || a._tile == ACTOR_SLEEPING || a._tile == ACTOR_DEAD) {
			// Remains stay beneath anything else
			if (cell == 0)
				cell = a._animTile;
		} else {
			cell = a._animTile;
		}
	}

	for (int y = 0; y < COMBAT_SIZE; ++y) {
		for (int x = 0; x < COMBAT_SIZE; ++x) {
			const int tile = objects[y][x] != 0 ? ACTOR_TILE_BASE + objects[y][x] : _logic.tile(x, y);
			_G(tileset).drawTile(s, VIEW_X + x * Gfx::TILE_SIZE, VIEW_Y + y * Gfx::TILE_SIZE, tile);
		}
	}

	if (_state == STATE_AIM) {
		const int px = VIEW_X + _logic.aimX() * Gfx::TILE_SIZE;
		const int py = VIEW_Y + _logic.aimY() * Gfx::TILE_SIZE;
		s.frameRect(Common::Rect(px, py, px + Gfx::TILE_SIZE, py + Gfx::TILE_SIZE), COLOR_AIM);
	}
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
