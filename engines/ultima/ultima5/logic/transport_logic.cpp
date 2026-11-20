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

#include "ultima/ultima5/logic/transport_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

// Tiles of the things the party can be in or can get on
constexpr byte TILE_HORSE = 0x10;
constexpr byte TILE_RIDING_HORSE = 0x12;
constexpr byte TILE_CARPET_OBJECT = 0x1B;
constexpr byte TILE_FLYING_CARPET = 0x14;
constexpr byte TILE_FOOT = 0x1C;
constexpr byte TILE_FOOT_HIDDEN = 0x1D;
constexpr byte TILE_FRIGATE_SAILING = 0x20;
constexpr byte TILE_FRIGATE_ROWING = 0x24;
constexpr byte TILE_FRIGATE_DOCKED = 0x25;
constexpr byte TILE_SKIFF = 0x28;
constexpr byte TILE_SKIFF_DOCKED = 0x29;
constexpr byte TILE_PIRATE = 0x2C;
constexpr byte TILE_BRIDGE_A = 0x6A;
constexpr byte TILE_DOCK = 0x47;
constexpr byte TILE_BREAKING = 3;
constexpr byte TILE_THORNS = 0x2F;
constexpr byte TILE_WHIRLPOOL = 0xEC;

constexpr int OUTDOORS = 0;
constexpr int FIRST_DUNGEON = 0x20;
constexpr int LAST_DUNGEON = 0x29;
constexpr int DANGER_HULL = 10;
constexpr int WEAK_HULL = 0x32;
constexpr int FULL_HULL = 99;
constexpr int DAMAGE_MAX = 0x1E;
constexpr int PLANS_FLAG = 0x7F;
constexpr int SKIFF_STATE_MASK = 0x3F;
constexpr int FRIGATE_ORDER = 0x7F;
constexpr int MIN_ORDER = 0x40;

// How the keys of the arrows are numbered, which is how the way a ship is heading is kept
constexpr byte KEY_WEST = 1;
constexpr byte KEY_EAST = 2;
constexpr byte KEY_NORTH = 3;
constexpr byte KEY_SOUTH = 4;

// Which way the winds push, by the wind
static const int8 WIND_PUSH_X[4] = { 0, 0, -1, 1 };
static const int8 WIND_PUSH_Y[4] = { 1, -1, 0, 0 };

static byte headingKey(Direction direction) {
	switch (direction) {
	case DIR_NORTH:
		return KEY_NORTH;
	case DIR_EAST:
		return KEY_EAST;
	case DIR_SOUTH:
		return KEY_SOUTH;
	default:
		return KEY_WEST;
	}
}

int TransportLogic::objectAt(int x, int y, int &index) {
	const Data::Savegame &sg = _G(savegame);

	for (index = 1; index < Data::ACTOR_COUNT; ++index) {
		const Data::ActorRecord actor = sg.getActor(index);
		if (actor._x == x && actor._y == y && actor._z == sg._mapLevel)
			return actor._tile;
	}

	index = 0;
	return 0;
}

bool TransportLogic::onFoot() {
	const byte tile = _G(savegame)._partyTile;
	if (tile == TILE_FOOT || tile == TILE_FOOT_HIDDEN)
		return true;

	_log.print("\nOn foot\n");
	return false;
}

bool TransportLogic::isLandNear(int x, int y) {
	int index;
	const int tile = objectAt(x, y, index);

	if (tile != 0) {
		if (tile == TILE_CARPET_OBJECT)
			return true;

		const int kind = tile & 0xFC;
		return kind == TILE_FOOT || kind == TILE_FRIGATE_ROWING || kind == TILE_HORSE || kind == TILE_SKIFF;
	}

	return WorldLogic::isWalkable(TILE_FOOT, g_engine->getMapTile(x, y));
}

bool TransportLogic::landNearby() const {
	const Data::Savegame &sg = _G(savegame);

	return isLandNear(sg._mapX, sg._mapY - 1) || isLandNear(sg._mapX, sg._mapY + 1) ||
		isLandNear(sg._mapX - 1, sg._mapY) || isLandNear(sg._mapX + 1, sg._mapY);
}

// A ship can only be taken over from something that can step across
bool TransportLogic::canBoardShip() {
	switch (_G(savegame)._partyTile) {
	case TILE_FLYING_CARPET:
	case TILE_FLYING_CARPET + 1:
	case TILE_FOOT:
	case TILE_FOOT_HIDDEN:
	case TILE_SKIFF:
	case TILE_SKIFF + 1:
	case TILE_SKIFF + 2:
	case TILE_SKIFF + 3:
		return true;
	default:
		_log.print("\nOn foot\n");
		return false;
	}
}

bool TransportLogic::board() {
	Data::Savegame &sg = _G(savegame);

	if (sg._mapId > FIRST_DUNGEON && sg._mapId < LAST_DUNGEON) {
		_log.print("\nNot here!\n");
		return true;
	}

	int index;
	const byte found = objectAt(sg._mapX, sg._mapY, index);

	if ((found & 0xFE) == TILE_HORSE) {
		if (sg._mapId != OUTDOORS) {
			// A horse that belongs to someone in a town isn't to be taken
			for (int i = 0; i < Data::NPC_COUNT; ++i) {
				const Data::NpcState state = sg.getNpcState(i);
				if (state._object == index && sg._npcTypes[i] != 0 && state._present != 0 && state._dialog != 0) {
					_log.print("\"Nay!\"\n");
					return true;
				}
			}
		}

		if (!onFoot())
			return true;

		_log.print("horse\n");
		sg._partyTile = found + 2;
	} else if (found == TILE_CARPET_OBJECT) {
		if (!onFoot())
			return true;

		_log.print("carpet\n");
		sg._partyTile = TILE_FLYING_CARPET;
	} else if ((found & 0xFC) == TILE_SKIFF) {
		if (!onFoot())
			return true;

		_log.print("skiff\n");
		sg._partyTile = found;
	} else if ((found & 0xFC) == TILE_FRIGATE_ROWING) {
		if (!canBoardShip())
			return true;

		_log.print("Ship\n");

		const Data::ActorRecord ship = sg.getActor(index);
		if (ship._hull < DANGER_HULL)
			_log.print("\nDANGER: SHIP BADLY DAMAGED!\n");

		Data::ActorRecord party = sg.getActor(0);
		party._hull = ship._hull;
		byte skiffs = ship._skiffs;

		if ((sg._partyTile & 0xFE) == TILE_FLYING_CARPET)
			++sg._carpets;

		if ((sg._partyTile & 0xFC) == TILE_SKIFF)
			++skiffs;

		if (skiffs == 0)
			_log.print("\nWARNING: NO SKIFFS ON BOARD!\n");

		sg._partyTile = found;
		party._skiffs = skiffs;
		sg.setActor(0, party);
	} else {
		_log.print("What?\n");
		return false;
	}

	// What was boarded is gone from the map
	sg.setActor(index, Data::ActorRecord());
	return true;
}

void TransportLogic::getOff() {
	Data::Savegame &sg = _G(savegame);

	const byte under = g_engine->getMapTile(sg._mapX, sg._mapY);
	byte left;
	byte skiffs = 0;
	Data::ActorRecord party = sg.getActor(0);

	switch (sg._partyTile & 0xFC) {
	case TILE_FOOT:
		_log.print("what?\n");
		return;

	case TILE_FRIGATE_SAILING:
		_log.print("\nUnder sail!\n");
		return;

	case TILE_FLYING_CARPET:
		if (landNearby() || WorldLogic::isWalkable(TILE_FOOT, under)) {
			_log.print("carpet!\n");
			left = TILE_CARPET_OBJECT;
			sg._partyTile = TILE_FOOT;
			break;
		}

		_log.print(landNearby() ? "\nNot here!\n" : "\nNo land nearby!\n");
		return;

	case TILE_HORSE:
		_log.print("horse!\n");
		left = sg._partyTile - 2;
		sg._partyTile = TILE_FOOT;
		break;

	case TILE_SKIFF:
		if (!landNearby()) {
			_log.print("\nNo land nearby!\n");
			return;
		}

		if ((under & 0xFE) == TILE_BRIDGE_A) {
			_log.print("\nNot here!\n");
			return;
		}

		_log.print("skiff!\n");
		left = sg._partyTile;
		sg._partyTile = TILE_FOOT;
		break;

	case TILE_FRIGATE_ROWING:
		_log.print("ship!\n");

		if (landNearby()) {
			left = sg._partyTile;
			sg._partyTile = TILE_FOOT;
			skiffs = party._skiffs;
		} else if (party._skiffs != 0) {
			left = sg._partyTile;
			sg._partyTile += 4;
			skiffs = party._skiffs - 1;
		} else if (sg._carpets != 0) {
			--sg._carpets;
			left = sg._partyTile;
			sg._partyTile = TILE_FLYING_CARPET;
			skiffs = party._skiffs;
		} else {
			_log.print("\nNo skiffs on board!\n");
			return;
		}
		break;

	default:
		return;
	}

	// What's left behind is put on the map, with how damaged it is
	const int slot = NpcLogic::findFreeObject(sg);
	if (slot != 0) {
		Data::ActorRecord object;
		object._tile = object._animTile = left;
		object._x = sg._mapX;
		object._y = sg._mapY;
		object._z = sg._mapLevel;
		object._hull = party._hull;
		object._skiffs = skiffs;
		sg.setActor(slot, object);
	}
}

bool TransportLogic::steer(Direction direction) {
	Data::Savegame &sg = _G(savegame);
	const int param = direction;
	bool turned = false;

	if ((sg._partyTile & 0xFC) == TILE_FRIGATE_SAILING) {
		const byte key = headingKey(direction);
		if (sg._sailHeading != key) {
			sg._sailHeading = key;
			sg._windTicks = 0;
		}

		sg._sailBreak = 0;
	}

	switch (sg._partyTile & 0xFC) {
	case TILE_HORSE:
		_log.print("Ride ");
		if (param == 1)
			sg._partyTile = TILE_RIDING_HORSE;
		else if (param == 3)
			sg._partyTile = TILE_RIDING_HORSE + 1;
		break;

	case TILE_FLYING_CARPET:
		_log.print("Fly ");
		if (param == 1)
			sg._partyTile = TILE_FLYING_CARPET;
		else if (param == 3)
			sg._partyTile = TILE_FLYING_CARPET + 1;
		break;

	case TILE_SKIFF:
		_log.print("Row ");
		sg._partyTile = param + (sg._partyTile & 0xFC);
		break;

	case TILE_FRIGATE_SAILING:
	case TILE_FRIGATE_ROWING: {
		const byte before = sg._partyTile;
		sg._partyTile = param + (sg._partyTile & 0xFC);

		if (before != sg._partyTile) {
			_log.print("Head ");

			switch (param) {
			case 0:
				_log.print("North\n");
				break;
			case 2:
				_log.print("South\n");
				break;
			case 1:
				_log.print("East\n");
				break;
			case 3:
				_log.print("West\n");
				break;
			}

			sg._sailBreak = 1;
			turned = true;

			if (sg.getActor(0)._hull < WEAK_HULL)
				_log.print("Hull weak!\n");
		} else if (sg._partyTile < TILE_FRIGATE_ROWING && sg._windDirection == 0) {
			// There's no wind to fill the sails
			turned = true;
		}
		break;
	}

	default:
		break;
	}

	return turned;
}

bool TransportLogic::canMove(int dx, int dy) {
	Data::Savegame &sg = _G(savegame);
	const int x = sg._mapX + dx;
	const int y = sg._mapY + dy;
	const byte party = sg._partyTile;

	if ((party & 0xFC) == TILE_FRIGATE_ROWING)
		_log.print("Rowing!\n");

	bool free = true;
	int index;
	const byte other = objectAt(x, y, index);

	if (other != 0) {
		free = false;

		if (party >= 0x30 || party < TILE_FRIGATE_SAILING) {
			if ((other >= TILE_FRIGATE_ROWING && other < TILE_PIRATE) || other == TILE_CARPET_OBJECT ||
					(other & 0xFE) == TILE_HORSE)
				free = true;
		} else if (party >= TILE_SKIFF && other >= TILE_FRIGATE_ROWING && other < TILE_SKIFF) {
			free = true;
		}
	}

	// Whatever's standing there is what the map shows, rather than the ground under it
	const byte ground = other != 0 ? 0 : g_engine->getMapTile(x, y);
	const bool ok = free && WorldLogic::isWalkable(party, ground);

	if (!ok) {
		if (sg._sailHeading != 0) {
			if (ground == TILE_BREAKING)
				_log.print("BREAKING UP!\n");
			else if (ground != TILE_DOCK)
				_log.print("COLLISION!\n");

			if (ground == TILE_DOCK) {
				_log.print("Docked!\n");
				sg._partyTile += 4;
			} else {
				damage();
			}

			sg._sailHeading = 0;
			sg._sailBreak = 1;
		} else if (party < TILE_FRIGATE_SAILING || (other & 0xFC) != TILE_WHIRLPOOL) {
			_log.print("Blocked!\n");

			if (ground == TILE_THORNS) {
				_log.print("OUCH!\n");
				PartyLogic::hurtParty();
			}
		}
	}

	return ok;
}

void TransportLogic::damage() {
	Data::Savegame &sg = _G(savegame);

	if ((sg._partyTile & 0xF8) != TILE_FRIGATE_SAILING) {
		PartyLogic::hurtParty();
		return;
	}

	Data::ActorRecord party = sg.getActor(0);
	const int amount = _G(gameRandom).range(1, DAMAGE_MAX);

	if (amount < party._hull) {
		party._hull -= amount;
		sg.setActor(0, party);
		return;
	}

	_log.print("Ship sunk!\n");

	if (party._skiffs > 0 || sg._carpets != 0) {
		_log.print("Abandon ship!\n");

		if (party._skiffs > 0) {
			sg._partyTile = (sg._partyTile & 3) + TILE_SKIFF;
		} else {
			--sg._carpets;
			sg._partyTile = _G(gameRandom).range(0, 1) + TILE_FLYING_CARPET;
		}
	} else {
		sg._partyTile = 0;
		_log.print("DROWNING!!!\n");

		// Everyone goes down with the ship
		for (;;) {
			bool alive = false;
			for (int i = 0; i < sg._partySize; ++i) {
				if (sg._party[i]._status == Data::STATUS_GOOD || sg._party[i]._status == Data::STATUS_POISONED ||
						sg._party[i]._status == Data::STATUS_SLEEP)
					alive = true;
			}

			if (!alive)
				break;

			PartyLogic::hurtParty();
		}
	}
}

int TransportLogic::heading() {
	const Data::Savegame &sg = _G(savegame);

	if ((sg._partyTile & 0xFC) != TILE_FRIGATE_SAILING)
		return -1;

	switch (sg._sailHeading) {
	case KEY_NORTH:
		return DIR_NORTH;
	case KEY_EAST:
		return DIR_EAST;
	case KEY_SOUTH:
		return DIR_SOUTH;
	case KEY_WEST:
		return DIR_WEST;
	default:
		return -1;
	}
}

bool TransportLogic::sailTick(bool &planTick) {
	Data::Savegame &sg = _G(savegame);
	const int direction = heading();
	if (direction < 0)
		return false;

	if (sg._windDirection != 0) {
		static const int8 DX[4] = { 0, 1, 0, -1 };
		static const int8 DY[4] = { -1, 0, 1, 0 };
		const int w = sg._windDirection - 1;

		// Sailing with the wind is slower than across it
		int mismatch = 1;
		if (WIND_PUSH_X[w] != DX[direction])
			++mismatch;
		if (WIND_PUSH_Y[w] != DY[direction])
			++mismatch;

		if (mismatch % 3 <= sg._windTicks) {
			sg._windTicks = 0;
			return true;
		}
	}

	if (sg._capePlans > PLANS_FLAG) {
		planTick = !planTick;
		TimeLogic::advance(sg, 1);
	} else {
		TimeLogic::advance(sg, 2);
	}

	++sg._windTicks;
	return false;
}

void TransportLogic::placePurchasedShip() {
	Data::Savegame &sg = _G(savegame);

	if (sg._shipOrder < MIN_ORDER)
		return;

	const int slot = NpcLogic::findFreeObject(sg);
	if (slot != 0) {
		Data::ActorRecord ship;
		ship._skiffs = sg._shipOrder & SKIFF_STATE_MASK;
		ship._x = sg._shipX;
		ship._y = sg._shipY;
		ship._z = 0;
		ship._tile = ship._animTile = sg._shipOrder > FRIGATE_ORDER ? TILE_FRIGATE_DOCKED : TILE_SKIFF_DOCKED;
		ship._hull = FULL_HULL;
		sg.setActor(slot, ship);
	}

	sg._shipOrder = 0;
}

void TransportLogic::clearHeadingIfNotSailing() {
	Data::Savegame &sg = _G(savegame);

	if ((sg._partyTile & 0xFC) != TILE_FRIGATE_SAILING)
		sg._sailHeading = 0;
}

} // namespace Ultima5
} // namespace Ultima
