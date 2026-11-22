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

#include "ultima/ultima5/logic/dungeon_items.h"
#include "ultima/ultima5/data/combat_data.h"
#include "ultima/ultima5/logic/attack_logic.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

// The objects that can be found, as they are numbered
constexpr int OBJECT_CHEST = 0x01;
constexpr int OBJECT_GOLD = 0x02;
constexpr int OBJECT_POTION = 0x03;
constexpr int OBJECT_SCROLL = 0x04;
constexpr int OBJECT_WEAPON = 0x05;
constexpr int OBJECT_SHIELD = 0x06;
constexpr int OBJECT_KEY = 0x07;
constexpr int OBJECT_GEM = 0x08;
constexpr int OBJECT_HELM = 0x09;
constexpr int OBJECT_RING = 0x0A;
constexpr int OBJECT_ARMOUR = 0x0B;
constexpr int OBJECT_AMULET = 0x0C;
constexpr int OBJECT_TORCH = 0x0D;
constexpr int OBJECT_FOOD = 0x0F;
constexpr int OBJECT_BOX = 0x0E;
constexpr int OBJECT_MOONSTONE = 0x19;
constexpr int OBJECT_CARPET = 0x1B;
constexpr int OBJECT_SHARD = 0xB4;
constexpr int OBJECT_CROWN = 0xB5;
constexpr int OBJECT_SCEPTRE = 0xB6;
constexpr int OBJECT_LB_AMULET = 0xB7;

// The carpet in this town is somebody's, and the box's owner is the last person of it
constexpr int CARPET_TOWN = 0x11;
constexpr int CARPET_NPC = 0x16;
constexpr int BOX_KILLED_BYTE = 0x43;
constexpr byte BOX_KILLED_BIT = 0x80;

constexpr int ARROWS = 0x1B;
constexpr int QUARRELS = 0x1D;
constexpr int ODD_KEY_FLAG = 0x80;
constexpr int CAPE_PLANS = 0xFF;
constexpr int MAX_STACK = 99;
constexpr byte CELL_VISITED = 8;
constexpr int LOOT_ENTRIES = 7;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_LEFT = 1;

static const char *const POTION_COLORS[8] = { "blue", "yellow", "red", "green", "orange", "purple", "black", "white" };
static const char *const SCROLL_RUNES[8] = { "VL", "RH", "IS", "IA", "IQW", "KXC", "IMC", "AT" };

// The chances, sizes and kinds of what is in a chest of a dungeon
static const byte LOOT_ODDS[8] = { 0x02, 0x04, 0x05, 0x0A, 0x14, 0x19, 0x19, 0x00 };
static const byte LOOT_SIZE[8] = { 0x1F, 0x00, 0x03, 0x03, 0x03, 0x07, 0x07, 0x00 };
static const byte LOOT_KIND[8] = { 0x0F, 0x02, 0x07, 0x08, 0x0D, 0xFF, 0xFF, 0x00 };

// What the trap of a chest turns out to be
static const byte TRAP_KINDS[8] = { 0, 0, 0, 1, 1, 2, 2, 3 };

static Data::Savegame &save() {
	return _G(savegame);
}

static void raise(byte &value, int amount, int limit) {
	value = value + amount < limit ? value + amount : limit;
}

static void raiseStack(byte &value) {
	if (++value == MAX_STACK + 1)
		value = MAX_STACK;
}

byte &DungeonItems::cellAt(int x, int y) {
	Data::Savegame &sg = save();
	return sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + (y & 7) * DUNGEON_SIZE + (x & 7)];
}

void DungeonItems::target(int key, int &x, int &y) {
	static const int8 DX[4] = { 0, 1, 0, -1 };
	static const int8 DY[4] = { -1, 0, 1, 0 };
	const Data::Savegame &sg = save();
	int facing = DungeonLogic::facing();

	x = sg._mapX;
	y = sg._mapY;

	switch (key) {
	case KEY_UP:
		break;

	case KEY_DOWN:
		return;

	case KEY_RIGHT:
		facing = (facing + 1) % 4;
		break;

	default:
		facing = (facing + 3) % 4;
		break;
	}

	x += DX[facing];
	y += DY[facing];
}

int DungeonItems::skill(int who) {
	const Data::Savegame &sg = save();
	return (sg._mapLevel * 2 - sg._party[who]._dex + 0x1E) >> 1;
}

void DungeonItems::search(int who, int x, int y) {
	Data::Savegame &sg = save();
	const byte found = cellAt(x, y);
	const int level = sg._mapLevel;
	const int worth = skill(who);

	_log.print("You find:\n");

	switch (found & 0xF0) {
	case 0x00:
		_log.print("Nothing of note.\n");
		break;

	case 0x10:
	case 0x20:
	case 0x30:
		_log.print("Nothing hidden on the ladder.\n");
		break;

	case 0x40: {
		int size;

		if (_G(gameRandom).range(1, 0x1E) > worth) {
			if (found == 0x40) {
				_log.print("No trap\n");
				break;
			}

			size = level;
		} else {
			size = _G(gameRandom).range(1, 8);
		}

		if (size < 4)
			_log.print("A simple trap\n");
		else if (size >= 7)
			_log.print("A complex trap\n");
		else
			_log.print("A trap\n");
		break;
	}

	case 0x50:
		_log.print("Nothing hidden on the fountain.\n");
		break;

	case 0x60:
		switch (found) {
		case 0x60:
			_log.print("Nothing hidden\nin the pit.\n");
			break;

		case 0x61:
			_log.print("A pit!\n");
			cellAt(x, y) = (found & CELL_VISITED) + 0x60;
			if (level < 7)
				sg._mapData[(level + 1) * DUNGEON_LEVEL_BYTES + (y & 7) * DUNGEON_SIZE + (x & 7)] |= CELL_VISITED;
			break;

		case 0x62:
			if (worth < _G(gameRandom).range(1, 0x1E)) {
				_log.print("A bomb trap!\n");
				cellAt(x, y) &= CELL_VISITED;
			} else {
				_log.print("Nothing of note.\n");
			}
			break;
		}
		break;

	case 0x70:
		_log.print("Treasure!\n");
		break;

	case 0x80:
		switch (found) {
		case 0x80:
			_log.print("A sleep field.\n");
			break;
		case 0x81:
			_log.print("A poison gas field.\n");
			break;
		case 0x82:
			_log.print("A wall of fire.\n");
			break;
		case 0x83:
			_log.print("An electric field.\n");
			break;
		default:
			_log.print("An energy field.\n");
			break;
		}
		break;

	case 0x90:
		_log.print("This tile is impossible.\n");
		break;

	case 0xA0:
		_log.print("Nothing hidden on the door.\n");
		break;

	case 0xB0:
		_log.print("Nothing hidden on the wall.\n");
		break;

	case 0xC0:
		switch (DungeonLogic::style() & 0xF) {
		case 2:
			_log.print("Nothing in the caved in passage.\n");
			break;

		case 1:
			_log.print("Nothing on the stalactite.\n");
			break;

		default:
			_log.print("Nothing hidden on the skeleton.\nIt crumbles away.\n");
			cellAt(x, y) = (found & CELL_VISITED) - 0x50;
			break;
		}
		break;

	case 0xD0:
		_log.print("A hidden door!\n");
		cellAt(x, y) = (found & CELL_VISITED) - 0x20;
		break;

	default:
		_log.print("Nothing hidden on the door.\n");
		break;
	}
}

bool DungeonItems::look(int x, int y) {
	byte seen = cellAt(x, y);

	_log.print("You see:\n");

	if (seen == 0x61)
		seen = 0;

	if ((seen & 0xF0) == 0x80) {
		switch (seen) {
		case 0x80:
			_log.print("A sleep field.\n");
			break;
		case 0x81:
			_log.print("A poison gas field.\n");
			break;
		case 0x82:
			_log.print("A wall of fire.\n");
			break;
		case 0x83:
			_log.print("An electric field.\n");
			break;
		default:
			_log.print("An energy field.\n");
			break;
		}

		return false;
	}

	if ((seen & 0xF0) == 0xC0) {
		const int style = DungeonLogic::style() & 0xF;

		if (style == 1)
			_log.print("a dripping stalactite.\n");
		else if (style == 2)
			_log.print("a caved in passage.\n");
		else if (_G(gameRandom).range(1, 0xFF) == 0xFF)
			_log.print("an unfortunate software pirate.\n");
		else
			_log.print("a less fortunate adventurer.\n");

		return false;
	}

	switch (seen & 0xF0) {
	case 0x00:
		_log.print("a passage.\n");
		break;
	case 0x10:
		_log.print("an up ladder.\n");
		break;
	case 0x20:
		_log.print("a down ladder.\n");
		break;
	case 0x30:
		_log.print("a ladder.\n");
		break;
	case 0x40:
		_log.print("a wooden chest.\n");
		break;
	case 0x50:
		_log.print("a fountain.\n");
		break;
	case 0x60:
		_log.print("a pit.\n");
		break;
	case 0x70:
		_log.print("an open chest.\n");
		break;
	case 0x80:
		_log.print("an energy field.\n");
		break;
	case 0x90:
		_log.print("nothing of note.\n");
		break;
	case 0xA0:
	case 0xE0:
	case 0xF0:
		_log.print("a heavy door.\n");
		break;
	case 0xB0:
	case 0xD0:
		_log.print("a wall.\n");
		break;
	default:
		_log.print("SPEC WALL ERR.\n");
		break;
	}

	if ((seen & 0xF0) == 0x50) {
		_log.print("Will you drink?\n");
		return true;
	}

	return false;
}

void DungeonItems::drink(int who, int x, int y) {
	Data::Character &c = save()._party[who];

	_log.print("Yes.  Gulp!\n");

	switch (cellAt(x, y)) {
	case 0x50:
		_log.print("Cured!\n");
		c._status = Data::STATUS_GOOD;
		break;

	case 0x51:
		_log.print("Healed!\n");
		c._hp = c._maxHp;
		break;

	case 0x52:
		_log.print("Poisoned!\n");
		c._status = Data::STATUS_POISONED;
		break;

	default:
		_log.print("Bad taste.\n");
		PartyLogic::hurtMember(who, _G(gameRandom).range(0, 7));
		break;
	}
}

void DungeonItems::chestTrap(int who) {
	Data::Savegame &sg = save();

	switch (TRAP_KINDS[_G(gameRandom).range(0, 7)]) {
	case 0: {
		_log.print("ACID!\n");
		int roll = _G(gameRandom).range(0, 0x3C) / 2;
		PartyLogic::hurtMember(who, roll == 0 ? 1 : roll);
		break;
	}

	case 1:
		_log.print("POISON!\n");
		if (who < sg._partySize && sg._party[who]._status != Data::STATUS_DEAD)
			sg._party[who]._status = Data::STATUS_POISONED;
		break;

	case 2:
		_log.print("BOMB!\n");
		PartyLogic::hurtParty();
		break;

	default:
		_log.print("GAS!\n");
		for (int i = 0; i < 6; ++i) {
			if (i < sg._partySize && sg._party[i]._status != Data::STATUS_DEAD)
				sg._party[i]._status = Data::STATUS_POISONED;
		}
		break;
	}
}

void DungeonItems::open(int who) {
	Data::Savegame &sg = save();
	const byte found = cellAt(sg._mapX, sg._mapY);

	if ((found & 7) != 0)
		chestTrap(who);

	cellAt(sg._mapX, sg._mapY) = (found & CELL_VISITED) + 0x70;
	_log.print("\nChest opened\n");
}

void DungeonItems::jimmy(int who) {
	Data::Savegame &sg = save();
	const byte found = cellAt(sg._mapX, sg._mapY);

	if ((found & 0xF7) == 0x40) {
		if (sg._keys == 0) {
			_log.print("No keys!\n");
		} else {
			_log.print("Key broke!\n");
			g_engine->sound().alert();
			--sg._keys;
		}
	} else if ((found & 0xF0) == 0x40) {
		if (sg._keys == 0) {
			_log.print("No keys!\n");
		} else if (skill(who) < _G(gameRandom).range(1, 0x1E)) {
			_log.print("Chest unlocked\n");
			cellAt(sg._mapX, sg._mapY) = (found & CELL_VISITED) + 0x40;
		} else {
			_log.print("Key broke!\n");
			g_engine->sound().alert();
			--sg._keys;
		}
	} else if ((found & 0xF0) == 0x70) {
		_log.print("Already open!\n");
	} else {
		_log.print("What?\n");
	}
}

void DungeonItems::take() {
	Data::Savegame &sg = save();
	const byte found = cellAt(sg._mapX, sg._mapY);

	_log.print("Get\n");

	if ((found & 0xF0) == 0x40) {
		_log.print("Must open first!\n");
		return;
	}

	if ((found & 0xF0) != 0x70) {
		_log.print("Not here!\n");
		return;
	}

	cellAt(sg._mapX, sg._mapY) &= CELL_VISITED;
	_log.print("contents\nof chest\nYou find:\n");

	for (int i = 0; i < LOOT_ENTRIES; ++i) {
		if (LOOT_ODDS[i] <= _G(gameRandom).range(1, sg._mapLevel * 4 + 4)) {
			if (i == 5) {
				award(OBJECT_POTION, _G(gameRandom).range(0, 7));
			} else if (i == 6) {
				award(OBJECT_SCROLL, _G(gameRandom).range(0, 7));
			} else {
				// The original takes the roll over a range of nothing on the first level
				const int amount = i == 1 ? _G(gameRandom).range(1, MAX<int>(sg._mapLevel << 3, 1)) :
					_G(gameRandom).range(1, LOOT_SIZE[i]);

				award(LOOT_KIND[i], amount);
			}
		}
	}
}

bool DungeonItems::award(int tile, int amount, int object) {
	Data::Savegame &sg = save();

	switch (tile) {
	case OBJECT_CHEST:
		_log.print("Open it first!\n");
		return false;

	case OBJECT_MOONSTONE:
		_log.print("A moonstone!\n");
		sg._moonstoneState[amount & 7] = 0xFF;
		break;

	case OBJECT_CARPET:
		_log.print("A magic carpet!\n");
		if (++sg._carpets == 100)
			sg._carpets = MAX_STACK;

		if (sg._mapId == CARPET_TOWN)
			NpcLogic::despawn(sg, CARPET_NPC);
		break;

	case OBJECT_BOX:
		_log.print("A sandalwood box!\n");
		sg._sandalwoodBox = 0xFF;
		sg._npcKilled[BOX_KILLED_BYTE] |= BOX_KILLED_BIT;
		break;

	case OBJECT_SHARD:
		sg._shards[amount & 3] = 0xFF;
		_log.print("The Shard of\n");
		_log.print((amount & 3) == 0 ? "Falsehood!\n" : (amount & 3) == 1 ? "Hatred!\n" : "Cowardice!\n");
		break;

	case OBJECT_CROWN: {
		sg._crown = 0xFF;
		_log.print("The Crown of Lord British!\n");

		const int npc = AttackLogic::findNpc(object);
		if (npc != -1) {
			sg.setNpcKilled(npc);
			NpcLogic::despawn(sg, npc);
		}
		break;
	}

	case OBJECT_SCEPTRE:
		sg._sceptre = 0xFF;
		_log.print("The Sceptre of Lord British!\n");
		break;

	case OBJECT_LB_AMULET:
		sg._amulet = 0xFF;
		_log.print("The Amulet of Lord British!\n");
		break;

	case OBJECT_FOOD:
		_log.print(Common::String::format("%d food!\n", amount));
		sg._food = sg._food + amount < 9999 ? sg._food + amount : 9999;
		break;

	case OBJECT_TORCH:
		_log.print(Common::String::format("%d torch%s!\n", amount, amount == 1 ? "" : "es"));
		raise(sg._torches, amount, MAX_STACK);
		break;

	case OBJECT_GEM:
		_log.print(Common::String::format("%d gem%s!\n", amount, amount == 1 ? "" : "s"));
		raise(sg._gems, amount, MAX_STACK);
		break;

	case OBJECT_KEY:
		if (amount > ODD_KEY_FLAG - 1) {
			amount &= 0x7F;
			_log.print(Common::String::format("%d odd key", amount));
			raise(sg._skullKeys, amount, MAX_STACK);
		} else {
			_log.print(Common::String::format("%d key", amount));
			raise(sg._keys, amount, MAX_STACK);
		}

		_log.print(amount == 1 ? "!\n" : "s!\n");
		break;

	case OBJECT_SCROLL:
		if (amount == CAPE_PLANS) {
			_log.print("The plans for the HMS Cape!\n");
			sg._capePlans = 0xFF;
		} else {
			_log.print("A scroll: ");
			_log.setRunes(true);
			_log.print(SCROLL_RUNES[amount & 7]);
			_log.setRunes(false);
			_log.print("!\n");
			raiseStack(sg._scrolls[amount & 7]);
		}
		break;

	case OBJECT_GOLD:
		_log.print(Common::String::format("%d gold!\n", amount));
		sg._gold = sg._gold + amount < 9999 ? sg._gold + amount : 9999;
		break;

	case OBJECT_POTION:
		_log.print(Common::String("A ") + POTION_COLORS[amount & 7] + " potion!\n");
		raiseStack(sg._potions[amount & 7]);
		break;

	case OBJECT_WEAPON:
	case OBJECT_SHIELD:
	case OBJECT_HELM:
	case OBJECT_RING:
	case OBJECT_ARMOUR:
	case OBJECT_AMULET:
		if (amount == ARROWS || amount == QUARRELS)
			raise(sg._equipment[amount], 5, MAX_STACK);
		else
			raiseStack(sg._equipment[amount]);

		_log.print(Common::String(Data::EQUIPMENT_NAMES[amount]) + "!\n");
		break;

	default:
		_log.print("Nothing to get!\n");
		return false;
	}

	return true;
}

} // namespace Ultima5
} // namespace Ultima
