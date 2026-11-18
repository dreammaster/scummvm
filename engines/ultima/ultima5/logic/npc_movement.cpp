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

#include "common/endian.h"
#include "common/util.h"
#include "ultima/ultima5/logic/npc_movement.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/world_logic.h"

namespace Ultima {
namespace Ultima5 {

constexpr int SIZE = Data::LOCATION_SIZE;
constexpr int MOVEMENT_RECORD_SIZE = 32;
constexpr int GRID_SIZE = 0x440;
constexpr int NO_WALK = -1;
constexpr int NOT_FREE = 99;
constexpr int MAX_PERSON = 0x20;

// The states a person can be in
enum {
	STATE_ABSENT = 0, STATE_AT_PLACE = 1, STATE_WALKING = 2, STATE_ON_STAIRS_WALK = 3,
	STATE_TO_STAIRS_UP = 4, STATE_TO_STAIRS_DOWN = 5, STATE_FROM_BELOW = 6, STATE_FROM_ABOVE = 7
};

// What a person does at a place
enum {
	DOES_NOTHING = 0, WANDERS_NEAR = 1, WANDERS = 2, FLEES_NEAR = 3, FOLLOWS_NEAR = 4,
	FOLLOWS = 5, FLEES_CLOSE = 6, ALWAYS_FLEES = 7
};

// The grid the path finder fills in has a flag for blocked, and the way
// each place was reached in the high bits
constexpr byte GRID_BLOCKED = 0x90;
constexpr byte GRID_TARGET = 5;
constexpr byte GRID_START = 0x46;

constexpr byte TILE_LADDER_UP = 0xC8;
constexpr byte TILE_LADDER_DOWN = 0xC9;
constexpr byte TILE_STAIRS = 0xC4;
constexpr byte TILE_FURNITURE = 0x30;
constexpr byte TILE_CHAIR = 0x90;
constexpr byte TILE_FENCE = 0xA2;
constexpr byte TILE_FENCE_END = 0x43;
constexpr byte TILE_HORSE = 0x10;
constexpr byte FIRST_PERSON_TILE = 0x40;
constexpr byte WALKS_ANYWHERE = 0xFC;

constexpr byte ASK_TO_TALK = 0x74;
constexpr byte ASK_TO_ATTACK = 0x61;

constexpr int WALK_STUCK_LIMIT = 3;
constexpr int GIVE_UP = 200;
constexpr int GIVE_UP_END = 204;
constexpr int NEAR_PARTY = 4;

// The places people can't walk into, a bit per tile, which is set if they can't
static const byte NPC_BLOCKING_TILES[32] = {
	0x70, 0x0C, 0xFF, 0xF8, 0x01, 0xF3, 0xFF, 0xFF,
	0x72, 0x3F, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF,
	0xFE, 0xFF, 0x0F, 0xFF, 0xFF, 0xDF, 0xFF, 0x5F,
	0xF0, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

int NpcMovement::tileAt(int x, int y) const {
	if (x < 0 || y < 0 || x >= SIZE || y >= SIZE)
		return 0;
	return _map.getTile(x, y);
}

int NpcMovement::slotOf(int npc) const {
	return NpcLogic::scheduleSlot(_sg.getSchedule(npc), _sg._hour);
}

int NpcMovement::walkIndex(int npc) const {
	return READ_LE_INT16(&_sg._npcTalk[npc * 2]);
}

void NpcMovement::setWalkIndex(int npc, int index) {
	WRITE_LE_INT16(&_sg._npcTalk[npc * 2], index);
}

int NpcMovement::stuckCount(int npc) const {
	return READ_LE_INT16(&_sg._unk101C[npc * 2]);
}

void NpcMovement::setStuckCount(int npc, int count) {
	WRITE_LE_INT16(&_sg._unk101C[npc * 2], count);
}

byte &NpcMovement::walkData(int npc, int index) {
	// The walks are one after the other, so one that runs on is in the next
	static byte spare;
	const int offset = npc * MOVEMENT_RECORD_SIZE + index;

	if (offset < 0 || offset >= (int)sizeof(_sg._npcMovement)) {
		spare = 0;
		return spare;
	}

	return _sg._npcMovement[offset];
}

void NpcMovement::setActorPosition(int object, int x, int y) {
	Data::ActorRecord actor = _sg.getActor(object);
	actor._x = x;
	actor._y = y;
	_sg.setActor(object, actor);
}

void NpcMovement::moveTo(int npc, int x, int y) {
	Data::NpcState state = _sg.getNpcState(npc);
	setActorPosition(state._object, x, y);

	state._x = x;
	state._y = y;
	_sg.setNpcState(npc, state);
}

void NpcMovement::setState(int npc, int value) {
	Data::NpcState state = _sg.getNpcState(npc);
	state._present = value;
	_sg.setNpcState(npc, state);
}

void NpcMovement::setSlot(int npc, int slot) {
	Data::NpcState state = _sg.getNpcState(npc);
	state._slot = slot;
	_sg.setNpcState(npc, state);
}

int NpcMovement::actorAt(int x, int y, int level) const {
	for (int i = MAX_PERSON - 1; i >= 0; --i) {
		const Data::ActorRecord actor = _sg.getActor(i);
		if (actor._x == x && actor._y == y && actor._z == level)
			return actor._tile;
	}

	return 0;
}

int NpcMovement::distance(int x1, int y1, int x2, int y2) {
	return ABS(x1 - x2) + ABS(y1 - y2);
}

void NpcMovement::step(int direction) {
	switch (direction) {
	case 1:
		++_foundX;
		if (_foundY > 0x20)
			_foundX = 0x20;
		break;
	case 2:
		--_foundY;
		if (_foundX < 0)
			_foundY = 0;
		break;
	case 3:
		--_foundX;
		if (_foundY < 0)
			_foundX = 0;
		break;
	case 4:
		++_foundY;
		if (_foundX > 0x20)
			_foundY = 0x20;
		break;
	default:
		break;
	}
}

int NpcMovement::canStand(int x, int y, int level, int npc, int slot) const {
	const Data::NpcSchedule schedule = _sg.getSchedule(npc);
	if (slot > -1 && schedule._x[slot] == x && schedule._y[slot] == y && schedule._z[slot] == level)
		return 2;

	const byte tile = tileAt(x, y);
	if (_sg.getNpcState(npc)._present == STATE_ON_STAIRS_WALK && (tile == TILE_LADDER_UP || tile == TILE_LADDER_DOWN))
		return 1;

	return (NPC_BLOCKING_TILES[tile >> 3] & (0x80 >> (tile & 7))) == 0;
}

int NpcMovement::canStep(int x, int y, int npc, int slot) {
	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
		return 0;

	const int kind = tileAt(x, y) & 0xFC;
	int free;

	if (_sg._npcTypes[npc] == WALKS_ANYWHERE)
		free = 1;
	else if (kind == TILE_FURNITURE)
		free = 1;
	else if (kind == TILE_CHAIR && _sg.getNpcState(npc)._present != STATE_WALKING)
		free = 0;
	else
		free = canStand(x, y, _sg._mapLevel, npc, slot);

	if (free == 0 || actorAt(x, y, _sg._mapLevel) != 0)
		free = 0;

	return free;
}

void NpcMovement::buildGrid(int x, int y, int targetX, int targetY, int mode, int npc) {
	const byte ladder = mode == -1 ? TILE_LADDER_UP : TILE_LADDER_DOWN;
	const int slot = slotOf(npc);

	for (int row = 0; row < SIZE; ++row) {
		for (int col = 0; col < SIZE; ++col) {
			_grid[col + row * SIZE] = canStand(col, row, _sg._mapLevel, npc, slot) != 0 ? 0 : GRID_BLOCKED;

			if (mode < 0 && tileAt(col, row) == ladder)
				_grid[col + row * SIZE] = GRID_TARGET;
		}
	}

	// The search can step one past the map, which is never a way through
	memset(_grid + SIZE * SIZE, GRID_BLOCKED, sizeof(_grid) - SIZE * SIZE);

	const Data::NpcState state = _sg.getNpcState(npc);

	// Other people and the party get in the way when they're close
	for (int i = MAX_PERSON - 1; i > 0; --i) {
		const Data::ActorRecord actor = _sg.getActor(i);
		if (actor._tile >= FIRST_PERSON_TILE && distance(actor._x, actor._y, state._x, state._y) < NEAR_PARTY &&
				actor._x < SIZE && actor._y < SIZE)
			_grid[actor._y * SIZE + actor._x] = GRID_BLOCKED;
	}

	const Data::ActorRecord party = _sg.getActor(0);
	if (distance(party._x, party._y, state._x, state._y) < NEAR_PARTY && party._x < SIZE && party._y < SIZE)
		_grid[party._y * SIZE + party._x] = GRID_BLOCKED;

	if (mode > -1 && targetX >= 0 && targetY >= 0 && targetX < SIZE && targetY < SIZE)
		_grid[targetY * SIZE + targetX] = GRID_TARGET;

	_grid[y * SIZE + x] = GRID_START;
}

bool NpcMovement::search(int x, int y, int targetX, int targetY, int mode, int npc) {
	byte queueX[32], queueY[32];
	int result = 0, head = 0, tail = 1;

	buildGrid(x, y, targetX, targetY, mode, npc);

	queueX[0] = x;
	queueY[0] = y;

	do {
		if (result != 0)
			break;

		int cx = queueX[head];
		int cy = queueY[head];
		int dir = _grid[cy * SIZE + cx] >> 4;

		for (int i = 0; i < 4; ++i) {
			switch (dir) {
			case 1:
				--cx;
				if (cx < 0)
					result = 2;
				break;
			case 2:
				++cy;
				if (cy > 0x20)
					result = 2;
				break;
			case 3:
				++cx;
				if (cx > 0x20)
					result = 2;
				break;
			case 4:
				--cy;
				if (cy < 0)
					result = 2;
				break;
			default:
				break;
			}

			if (result != 2) {
				byte &cell = _grid[cy * SIZE + cx];

				if (cell < 0x10) {
					result = cell & 0xF;
					cell = dir << 4;

					if (result == GRID_TARGET) {
						result = 1;
						_foundX = cx;
						_foundY = cy;
						break;
					}

					if (tail != head) {
						queueX[tail] = cx;
						queueY[tail] = cy;
						++tail;
					}

					if (tail >= 0x20)
						tail = 0;
				}
			}

			result = 0;
			dir = (dir & 3) + 1;
			cx = queueX[head];
			cy = queueY[head];
		}

		if (result == 1)
			return true;

		++head;
		if (head == 0x20)
			head = 0;
	} while (head != tail);

	return result != 0;
}

bool NpcMovement::searchLadder(int x, int y, bool up, int npc) {
	return search(x, y, 0, 0, up ? -1 : -2, npc);
}

int NpcMovement::buildWalk(int npc, int, int targetX, int targetY) {
	int count = 0, index = 0;
	int cx = targetX, cy = targetY;

	setWalkIndex(npc, 0);

	auto cell = [this](int x, int y) -> int {
		const int offset = y * SIZE + x;
		return offset >= 0 && offset < GRID_SIZE ? _grid[offset] : 0;
	};

	int kind = cell(cx, cy) & 0xF;
	int dir = cell(cx, cy) >> 4;
	int previous = dir;

	do {
		switch (dir) {
		case 1:
			++cx;
			break;
		case 2:
			--cy;
			break;
		case 3:
			--cx;
			break;
		case 4:
			++cy;
			break;
		default:
			break;
		}

		if (previous == dir && kind != 6)
			++count;

		if (previous != dir || kind == 6) {
			walkData(npc, index++) = count;
			walkData(npc, index++) = previous;

			if (kind == 6)
				break;

			previous = dir;
			count = 1;
		}

		dir = cell(cx, cy) >> 4;
		kind = cell(cx, cy) & 0xF;
	} while (index < 0x20);

	// The walk was worked out from the end back, so turn it round
	int from = index - 2;
	if (from >= 0) {
		int to = 0;

		do {
			byte swap = walkData(npc, to);
			walkData(npc, to) = walkData(npc, from);
			walkData(npc, from) = swap;

			++from;
			++to;

			swap = ((walkData(npc, to) + 1) & 3) + 1;
			walkData(npc, to) = ((walkData(npc, from) + 1) & 3) + 1;
			walkData(npc, from) = swap;

			++to;
			from -= 3;
		} while (from >= to);
	}

	return index;
}

int NpcMovement::updateState(int npc, int hour) {
	Data::NpcState state = _sg.getNpcState(npc);
	const Data::NpcSchedule schedule = _sg.getSchedule(npc);
	const int8 level = _sg._mapLevel;
	int slot = -1;

	for (int i = 0; i < Data::NPC_TIME_SLOTS; ++i) {
		if (schedule._time[i] != hour)
			continue;

		slot = slotOf(npc);
		if (slot == (int)state._slot) {
			state._present = STATE_AT_PLACE;
			break;
		}

		const int8 there = schedule._z[slot];
		const int8 here = (int8)state._z;

		if (here != level && there != level) {
			state._present = 8;
		} else if (here == level) {
			if (there == level)
				state._present = STATE_WALKING;
			else if (level < there)
				state._present = STATE_FROM_BELOW;
			else
				state._present = STATE_FROM_ABOVE;
		} else if (level < here) {
			state._present = STATE_TO_STAIRS_UP;
		} else {
			state._present = STATE_TO_STAIRS_DOWN;
		}
		break;
	}

	if (slot > -1 && state._x == schedule._x[slot] && state._y == schedule._y[slot] &&
			state._z == schedule._z[slot]) {
		slot = 0;
		state._present = STATE_AT_PLACE;
	}

	_sg.setNpcState(npc, state);
	return slot == -1 ? 0 : slot + 1;
}

bool NpcMovement::onStairs(int npc, int slot) {
	const Data::NpcState state = _sg.getNpcState(npc);
	const Data::ActorRecord actor = _sg.getActor(state._object);
	const int tile = tileAt(actor._x, actor._y);
	const int8 level = _sg._mapLevel;
	const int8 there = (int8)_sg.getSchedule(npc)._z[slot];

	if (there < level) {
		if (tile == TILE_LADDER_DOWN)
			return true;
	} else if (tile == TILE_LADDER_UP) {
		return true;
	}

	return (tile & 0xF4) == TILE_STAIRS;
}

void NpcMovement::wander(int npc, int maxDistance, int slot) {
	if ((_random.range(0, 0xFF) & 8) == 0)
		return;

	const Data::NpcState state = _sg.getNpcState(npc);
	const Data::NpcSchedule schedule = _sg.getSchedule(npc);
	const int direction = (_random.range(0, 0x40) & 3) + 1;

	_foundX = state._x;
	_foundY = state._y;
	step(direction);
	const int x = _foundX;
	const int y = _foundY;

	if ((maxDistance == 0 || distance(schedule._x[slot], schedule._y[slot], x, y) <= maxDistance) &&
			canStep(x, y, npc, slot) != 0)
		moveTo(npc, x, y);
}

void NpcMovement::approachOrFlee(int npc, int slot) {
	const Data::NpcState state = _sg.getNpcState(npc);
	const Data::ActorRecord party = _sg.getActor(0);
	int kind = _sg.getSchedule(npc)._type[slot];
	int options[4];

	if (distance(party._x, party._y, state._x, state._y) == 1 && kind > 3) {
		if (kind == FOLLOWS_NEAR || kind == FOLLOWS) {
			if (state._dialog != 0) {
				_sg._unk1018 = ASK_TO_TALK;
				_sg._unk1019 = npc;
			}
		} else {
			_sg._unk1018 = ASK_TO_ATTACK;
			_sg._unk1019 = npc;
		}

		return;
	}

	for (int direction = 1; direction < 5; ++direction) {
		_foundX = state._x;
		_foundY = state._y;
		step(direction);

		if (canStep(_foundX, _foundY, npc, -1) != 0)
			options[direction - 1] = distance(party._x, party._y, _foundX, _foundY);
		else
			options[direction - 1] = NOT_FREE;
	}

	int choice = -1;
	const int current = distance(party._x, party._y, state._x, state._y);

	for (int i = 1; i < 8; ++i) {
		if (i < 5 && options[i - 1] != NOT_FREE) {
			if (kind == FLEES_NEAR) {
				if (current < options[i - 1]) {
					if (choice == -1 || _random.range(0, 1) != 0)
						choice = i;

					if (i == 4)
						break;
				}
			} else if (options[i - 1] < current) {
				choice = i;
				break;
			}
		}

		if (i > 4 && options[i - 5] != NOT_FREE && options[i - 5] == current) {
			choice = i;
			break;
		}
	}

	if ((kind == FOLLOWS || kind == ALWAYS_FLEES) && _random.range(0, 0x3F) < 0x10) {
		int other = choice;

		for (int i = 1; i < 5; ++i) {
			if (choice != i && options[i - 1] != NOT_FREE && (other == choice || _random.range(0, 0x3F) < 0x10))
				other = i;
		}

		choice = other;
	}

	if (choice > -1) {
		_foundX = state._x;
		_foundY = state._y;
		step(choice);
		moveTo(npc, _foundX, _foundY);
	}
}

void NpcMovement::act(int npc, int slot) {
	const Data::NpcSchedule schedule = _sg.getSchedule(npc);
	const Data::NpcState state = _sg.getNpcState(npc);

	switch (schedule._type[slot]) {
	case DOES_NOTHING:
		break;

	case WANDERS:
		wander(npc, 0, slot);
		break;

	case FOLLOWS_NEAR:
		if (distance(_sg._mapX, _sg._mapY, schedule._x[slot], schedule._y[slot]) < NEAR_PARTY)
			approachOrFlee(npc, slot);
		break;

	case WANDERS_NEAR:
		wander(npc, 3, slot);
		break;

	case FLEES_NEAR:
	case FLEES_CLOSE:
		if (distance(_sg._mapX, _sg._mapY, state._x, state._y) < NEAR_PARTY)
			approachOrFlee(npc, slot);
		break;

	case FOLLOWS:
	case ALWAYS_FLEES:
		approachOrFlee(npc, slot);
		break;

	default:
		break;
	}
}

void NpcMovement::moveHorses() {
	for (int i = 0; i < MAX_PERSON; ++i) {
		Data::ActorRecord actor = _sg.getActor(i);
		int tile = actor._tile;

		if ((tile & 0xFE) != TILE_HORSE || actor._z != _sg._mapLevel || _random.range(0, 1) != 0)
			continue;

		int x = actor._x;
		int y = actor._y;

		auto fence = [this](int fx, int fy) {
			const int t = tileAt(fx, fy);
			return t == TILE_FENCE || t == TILE_FENCE_END;
		};

		if (fence(x, y + 1) || fence(x + 1, y) || fence(x, y - 1) || fence(x - 1, y))
			continue;

		if (_random.range(0, 1) != 0) {
			const int delta = _random.range(0, 1) * 2 - 1;
			x += delta;
			tile = delta > 0 ? TILE_HORSE : TILE_HORSE + 1;
		} else {
			y += _random.range(0, 1) * 2 - 1;
		}

		if (x <= 0x1F && y <= 0x1F && x >= 0 && y >= 0 && WorldLogic::isWalkable(TILE_HORSE, tileAt(x, y)) &&
				actorAt(x, y, _sg._mapLevel) == 0) {
			actor._tile = actor._animTile = tile;
			actor._x = x;
			actor._y = y;
			_sg.setActor(i, actor);
		}
	}
}

void NpcMovement::turn() {
	const int hour = _sg._hour;
	const int level = _sg._mapLevel;
	int pathed = 0;

	WorldLogic::syncPartyObject(_sg);
	moveHorses();

	_sg._unk1018 = 0;
	_sg._unk1019 = 0;

	for (int npc = 1; npc < MAX_PERSON; ++npc) {
		if (_sg._npcTypes[npc] == 0)
			continue;

		int slot = NpcLogic::scheduleSlot(_sg.getSchedule(npc), hour);
		Data::NpcState state = _sg.getNpcState(npc);

		if (state._present <= STATE_AT_PLACE && updateState(npc, hour) == 0) {
			if (_sg.getNpcState(npc)._z == level)
				act(npc, slot);
			continue;
		}

		state = _sg.getNpcState(npc);
		const Data::NpcSchedule schedule = _sg.getSchedule(npc);

		if (state._present > 3) {
			if (state._present == STATE_TO_STAIRS_UP || state._present == STATE_TO_STAIRS_DOWN) {
				if (pathed == 1)
					continue;

				pathed = 1;
				const bool up = state._present == STATE_TO_STAIRS_UP;
				const int mode = up ? 3 : 4;
				int foundX = 0, foundY = 0;

				bool ok = searchLadder(schedule._x[slot], schedule._y[slot], up, npc);
				if (ok) {
					foundX = _foundX;
					foundY = _foundY;
					ok = search(foundX, foundY, schedule._x[slot], schedule._y[slot], mode, npc);
				}

				if (ok) {
					buildWalk(npc, mode, schedule._x[slot], schedule._y[slot]);

					const int tile = tileAt(foundX, foundY);
					if ((mode == 3 && tile == TILE_LADDER_UP) || (mode == 4 && tile == TILE_LADDER_DOWN) ||
							(tile & 0xFC) == TILE_STAIRS)
						NpcLogic::placeNpc(_sg, npc, foundX, foundY, level);

					setState(npc, STATE_WALKING);
				}
			} else if (state._present == STATE_FROM_BELOW || state._present == STATE_FROM_ABOVE) {
				if (onStairs(npc, slot)) {
					NpcLogic::placeNpc(_sg, npc, schedule._x[slot], schedule._y[slot], schedule._z[slot]);
					setSlot(npc, slot);
					setWalkIndex(npc, NO_WALK);
					setState(npc, STATE_AT_PLACE);
				} else if (pathed != 1) {
					pathed = 1;
					const bool below = state._present == STATE_FROM_BELOW;
					const int mode = below ? 1 : 2;

					bool ok = searchLadder(state._x, state._y, below, npc);
					if (ok) {
						const int foundX = _foundX;
						const int foundY = _foundY;
						ok = search(state._x, state._y, foundX, foundY, mode, npc);
						if (ok) {
							buildWalk(npc, mode, foundX, foundY);
							setState(npc, STATE_ON_STAIRS_WALK);
						}
					}
				}
			}

			continue;
		}

		const int index = walkIndex(npc);

		if (index > -1 && walkData(npc, index) != 0) {
			_foundX = state._x;
			_foundY = state._y;
			step(walkData(npc, index + 1));
			const int x = _foundX;
			const int y = _foundY;

			const int result = canStep(x, y, npc, slot);
			if (result != 0) {
				moveTo(npc, x, y);

				--walkData(npc, index);
				setStuckCount(npc, 0);

				if (walkData(npc, index) == 0) {
					int next = index + 1;
					walkData(npc, next++) = 0;
					setWalkIndex(npc, next);

					if (next >= 0x20 || walkData(npc, next) < 1)
						setWalkIndex(npc, NO_WALK);

					if (result == 2) {
						setSlot(npc, slot);
						setState(npc, STATE_AT_PLACE);
						setWalkIndex(npc, NO_WALK);
					}
				}
			} else {
				setStuckCount(npc, stuckCount(npc) + 1);
				wander(npc, 0, slot);

				if (stuckCount(npc) > WALK_STUCK_LIMIT) {
					setWalkIndex(npc, NO_WALK);
					setStuckCount(npc, 0);
				}
			}
		} else {
			if (walkIndex(npc) == NO_WALK && state._present == STATE_ON_STAIRS_WALK) {
				setState(npc, level < schedule._z[slot] ? STATE_FROM_BELOW : STATE_FROM_ABOVE);
				return;
			}

			if (pathed < 1) {
				if (state._present != STATE_AT_PLACE) {
					const int stuck = stuckCount(npc);

					if (stuck < GIVE_UP && (stuck == 0 || _random.range(0, 2) == 1)) {
						if (walkIndex(npc) == NO_WALK) {
							++pathed;

							if (search(state._x, state._y, schedule._x[slot], schedule._y[slot], 0, npc)) {
								buildWalk(npc, 0, schedule._x[slot], schedule._y[slot]);
								setStuckCount(npc, 0);
								continue;
							}

							setStuckCount(npc, GIVE_UP);
						}

						wander(npc, 0, slot);
					} else {
						if (stuck >= GIVE_UP)
							setStuckCount(npc, stuck + 1);

						if (stuckCount(npc) > GIVE_UP_END)
							setStuckCount(npc, 0);
					}
				}
			} else {
				act(npc, state._slot);
			}
		}
	}
}

} // namespace Ultima5
} // namespace Ultima
