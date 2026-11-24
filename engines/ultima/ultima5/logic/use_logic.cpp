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

#include "common/util.h"
#include "ultima/ultima5/logic/use_logic.h"
#include "ultima/ultima5/logic/inventory_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

// How the arrow keys are numbered
constexpr int KEY_LEFT = 1;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;

// Kinds of maps
constexpr int FIRST_DUNGEON = 0x21;
constexpr int LAST_DUNGEON = 0x28;
constexpr int FIRST_BATTLE = 0x80;
constexpr int NEGATE_TIME_MAP_A = 0x1D;
constexpr int NEGATE_TIME_MAP_B = 0x28;

// What the items are in the list
constexpr int SCROLLS = 8;
constexpr int POTIONS_END = 0x10;
constexpr int ITEM_CARPET = 0x10;
constexpr int ITEM_SKULL_KEY = 0x11;
constexpr int ITEM_AMULET = 0x12;
constexpr int ITEM_CROWN = 0x13;
constexpr int ITEM_SCEPTRE = 0x14;
constexpr int ITEM_FIRST_MOONSTONE = 0x15;
constexpr int ITEM_LAST_MOONSTONE = 0x1C;
constexpr int ITEM_FIRST_SHARD = 0x1D;
constexpr int ITEM_LAST_SHARD = 0x1F;
constexpr int ITEM_SPYGLASS = 0x20;
constexpr int ITEM_PLANS = 0x21;
constexpr int ITEM_SEXTANT = 0x22;
constexpr int ITEM_WATCH = 0x23;
constexpr int ITEM_BADGE = 0x24;
constexpr int ITEM_BOX = 0x25;

// The icons shown for what's been worn and the spells that last
constexpr int ICON_NONE = 0;
constexpr int ICON_AMULET = 0xE;
constexpr int ICON_CROWN = 0x1C;
constexpr int ICON_BADGE = 0x1D;
constexpr int ICON_PROTECTION = 0x50;
constexpr int ICON_NEGATE_MAGIC = 0x4E;
constexpr int ICON_NEGATE_TIME = 0x54;
constexpr int DURATION_PROTECTION = 100;
constexpr int DURATION_NEGATE_MAGIC = 0x14;
constexpr int DURATION_NEGATE_TIME = 0x14;
constexpr int DURATION_FOREVER = 0xFF;

constexpr byte LIGHT_SPELL_TURNS = 0xF0;
constexpr int PARTY_ON_FOOT = 0x1C;
constexpr int PARTY_CARPET = 0x14;
constexpr int PARTY_SHIP_MASK = 0xF8;
constexpr int PARTY_SHIP = 0x20;
constexpr byte TILE_CARPET_BLOCKED = 0x0C;
constexpr byte TILE_CROPS_PICKED = 0x2C;
constexpr byte TILE_CROPS = 0x2D;
constexpr byte TILE_WATER_THREE = 3;
constexpr byte TILE_BURY_LIMIT = 0x0B;
constexpr byte TILE_LOCKED_DOOR_A = 0x97;
constexpr byte TILE_LOCKED_DOOR_B = 0x98;
constexpr byte TILE_OPEN_DOOR_A = 0xB8;
constexpr byte TILE_OPEN_DOOR_B = 0xBA;
constexpr byte TILE_FIELD_KIND = 0x70;
constexpr byte TILE_GRASS = 5;
constexpr int NIGHT_START = 0x13;
constexpr int DAY_START = 6;
constexpr int STAR_DAY_END = 0x12;
constexpr int POTION_ODDS = 0xF;
constexpr int HEAL_MAX = 0x3C;
constexpr int LAST_GEM_PLACE = 3;

static const byte FLAME_X[3] = { 0x0F, 0x0F, 0x0F };
static const byte FLAME_Y[3] = { 0x09, 0x03, 0x10 };
static const byte FLAME_MAP[3] = { 0x1E, 0x1F, 0x20 };
static const byte FLAME_LEVEL[3] = { 0x02, 0x01, 0xFF };

static void setTileAt(int x, int y, byte tile) {
	if (_G(savegame)._mapId != 0)
		_G(location).setTile(x, y, tile);
}

void UseLogic::setIcon(int icon, int duration) {
	Data::Savegame &sg = _G(savegame);
	sg._icon = icon;
	sg._protection = duration;
}

// A thing that's worn can be taken off by using it again
bool UseLogic::removeWorn(int icon) {
	Data::Savegame &sg = _G(savegame);

	if (icon == sg._icon) {
		_log.print("Removed!\n");
		sg._icon = sg._protection = 0;
		return false;
	}

	return true;
}

void UseLogic::promptDirection() {
	_log.print("Direction-");
}

void UseLogic::promptMember() {
	_log.print("On who: ");
}

UseLogic::Need UseLogic::finish() {
	if (_result == 0) {
		_log.print("Failed!\n");
		g_engine->sound().alert();
	}

	return NEED_NONE;
}

UseLogic::Need UseLogic::use(int item) {
	Data::Savegame &sg = _G(savegame);
	_result = 1;

	Need need;
	if (item < SCROLLS) {
		need = scroll(item);
	} else if (item < POTIONS_END) {
		need = potion(item - SCROLLS);
	} else if (item >= ITEM_FIRST_MOONSTONE && item <= ITEM_LAST_MOONSTONE) {
		moonstone(item - ITEM_FIRST_MOONSTONE);
		need = NEED_NONE;
	} else {
		need = other(item);
	}

	(void)sg;
	return need == NEED_NONE ? finish() : need;
}

UseLogic::Need UseLogic::directionChosen(int key) {
	Data::Savegame &sg = _G(savegame);
	int x = sg._mapX;
	int y = sg._mapY;

	switch (key) {
	case KEY_UP:
		_log.print("North\n");
		--y;
		break;
	case KEY_RIGHT:
		_log.print("East\n");
		++x;
		break;
	case KEY_DOWN:
		_log.print("South\n");
		++y;
		break;
	case KEY_LEFT:
		_log.print("West\n");
		--x;
		break;
	default:
		_log.print("Pass\n");
		key = 0;
		break;
	}

	if (_stage == STAGE_WIND) {
		if (sg._mapId < FIRST_DUNGEON) {
			switch (key) {
			case KEY_UP:
				sg._windDirection = 1;
				break;
			case KEY_DOWN:
				sg._windDirection = 2;
				break;
			case KEY_RIGHT:
				sg._windDirection = 3;
				break;
			case KEY_LEFT:
				sg._windDirection = 4;
				break;
			default:
				sg._windDirection = 0;
				break;
			}

			sg._windTicks = 0;
		} else {
			_result = 0;
		}
	} else {
		// The skull key opens the locked doors that lead to the places the party is meant to find
		if (key == 0) {
			_result = -1;
		} else {
			const byte tile = g_engine->getMapTile(x, y);
			_result = 0;

			if (tile == TILE_LOCKED_DOOR_A) {
				setTileAt(x, y, TILE_OPEN_DOOR_A);
				_result = 1;
			} else if (tile == TILE_LOCKED_DOOR_B) {
				setTileAt(x, y, TILE_OPEN_DOOR_B);
				_result = 1;
			}
		}
	}

	return finish();
}

UseLogic::Need UseLogic::scroll(int which) {
	Data::Savegame &sg = _G(savegame);

	--sg._scrolls[which];
	_log.print("Scroll\n\n");

	switch (which) {
	case 0:
		sg._unk0300 = LIGHT_SPELL_TURNS;
		_log.print("Light!\n");
		TimeLogic::advance(sg, 0);
		break;

	case 1:
		_log.print("Wind change!\n");
		_stage = STAGE_WIND;
		promptDirection();
		return NEED_DIRECTION;

	case 2:
		_log.print("Protection!\n");
		setIcon(ICON_PROTECTION, DURATION_PROTECTION);
		break;

	case 3:
		_log.print("Negate magic!\n");
		setIcon(ICON_NEGATE_MAGIC, DURATION_NEGATE_MAGIC);
		break;

	case 4:
		_log.print("View!\n");
		_result = -1;
		return NEED_VIEW;

	case 5:
		_log.print("Summon Daemon!\n");
		_log.print("Not here!\n");
		break;

	case 6:
		_log.print("Resurrection!\n");
		_stage = STAGE_RESURRECTION;
		promptMember();
		return NEED_MEMBER;

	case 7:
		if (sg._mapId == NEGATE_TIME_MAP_A || sg._mapId == NEGATE_TIME_MAP_B) {
			_log.print("No effect!\n");
			break;
		}

		_log.print("Negate time!\n");
		setIcon(ICON_NEGATE_TIME, DURATION_NEGATE_TIME);
		break;

	default:
		break;
	}

	return NEED_NONE;
}

UseLogic::Need UseLogic::potion(int which) {
	Data::Savegame &sg = _G(savegame);

	--sg._potions[which];
	_log.print("Potion\n");

	_potion = which;
	_stage = STAGE_POTION;
	promptMember();
	return NEED_MEMBER;
}

void UseLogic::drinkPotion(int which, int who) {
	Data::Savegame &sg = _G(savegame);
	Data::Character &c = sg._party[who];
	GameRandom &random = _G(gameRandom);

	// Now and then it isn't what it's meant to be
	const int chance = random.range(0, POTION_ODDS);
	if (chance == 0)
		which = 4;
	else if (chance == 1)
		which = random.range(0, 7);

	switch (which) {
	case 0:
		if (c._status == Data::STATUS_SLEEP)
			c._status = Data::STATUS_GOOD;
		else
			_result = 0;
		break;

	case 1:
		if (c._status == Data::STATUS_DEAD) {
			_result = 0;
		} else {
			int amount = random.range(0, HEAL_MAX) / 2;
			if (amount == 0)
				++amount;

			c._hp = c._hp + amount < c._maxHp ? c._hp + amount : c._maxHp;
			_log.print("Healed!\n");
		}
		break;

	case 2:
		if (c._status == Data::STATUS_POISONED) {
			c._status = Data::STATUS_GOOD;
			_log.print("Poison cured!\n");
		} else {
			_result = 0;
		}
		break;

	case 3:
		if (c._status == Data::STATUS_GOOD) {
			c._status = Data::STATUS_POISONED;
			_log.print("POISONED!\n");
		} else {
			_result = 0;
		}
		break;

	case 4:
		if (c._status == Data::STATUS_GOOD) {
			c._status = Data::STATUS_SLEEP;
			_log.print("Slept!\n");
		} else {
			_result = 0;
		}
		break;

	case 5:
	case 6:
		_log.print("\nNo noticeable effect now!\n");
		break;

	default:
		// The potion of x-ray vision shows what's around, which only changes the picture
		if (sg._mapId >= FIRST_DUNGEON)
			_log.print("\nNo noticeable effect now!\n");
		break;
	}
}

UseLogic::Need UseLogic::memberChosen(int member) {
	Data::Savegame &sg = _G(savegame);

	if (member < 0) {
		_log.print("None!");
		_result = -1;
	} else {
		_log.print(sg._party[member]._name);
	}

	if (_log.column() != 0)
		_log.printChar('\n');

	if (member < 0)
		return finish();

	if (_stage == STAGE_POTION) {
		drinkPotion(_potion, member);
	} else if (sg._party[member]._status != Data::STATUS_DEAD) {
		_log.print("Not dead!\n");
		_result = 0;
	} else {
		InventoryLogic::resurrect(sg, member);
	}

	return finish();
}

void UseLogic::moonstone(int which) {
	Data::Savegame &sg = _G(savegame);
	const byte tile = g_engine->getMapTile(sg._mapX, sg._mapY);

	_log.print("Moonstone ");

	if (sg._mapId < FIRST_DUNGEON && (tile == TILE_CROPS_PICKED || tile == TILE_CROPS ||
			(tile > TILE_WATER_THREE && tile < TILE_BURY_LIMIT))) {
		_log.print("buried!\n");
		sg._moonstoneX[which] = sg._mapX;
		sg._moonstoneY[which] = sg._mapY;
		sg._moonstoneState[which] = sg._mapId;
		sg._moonstoneZ[which] = sg._mapLevel;
	} else {
		_log.print("cannot be buried here!\n");
	}
}

void UseLogic::worn(int icon, const char *intro, const char *line) {
	_log.print(intro);

	if (!removeWorn(icon))
		return;

	_log.print(line);
	_log.print(" of Lord British...\n");
	setIcon(icon, DURATION_FOREVER);
}

void UseLogic::carpet() {
	Data::Savegame &sg = _G(savegame);

	_log.print("Carpet\n\n");

	if (sg._mapId < FIRST_DUNGEON && g_engine->getMapTile(sg._mapX, sg._mapY) != TILE_CARPET_BLOCKED) {
		if (sg._partyTile == PARTY_ON_FOOT) {
			_log.print("Boarded!\n");
			sg._partyTile = _G(gameRandom).range(0, 1) + PARTY_CARPET;
			--sg._carpets;
		} else if ((sg._partyTile & PARTY_SHIP_MASK) == PARTY_SHIP) {
			_log.print("X-it ship first!\n");
		} else {
			_log.print("Only on foot!\n");
		}
	} else {
		_log.print("Not here!\n");
	}
}

void UseLogic::sceptre() {
	Data::Savegame &sg = _G(savegame);

	_log.print("Sceptre\n\n");
	_log.print("Wielding the Sceptre");
	_log.print(" of Lord British...\n");
	g_engine->sound().pulse(0x1450, 1, 50000, 5000, 1);

	int cleared = 0;
	if (sg._mapId < FIRST_DUNGEON || sg._mapId > LAST_DUNGEON) {
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				const int x = sg._mapX + dx;
				const int y = sg._mapY + dy;

				if ((g_engine->getMapTile(x, y) & 0xF0) == TILE_FIELD_KIND) {
					setTileAt(x, y, TILE_GRASS);
					g_engine->sound().noise(10, 3000, 2000);
					++cleared;
				}
			}
		}
	}

	if (cleared == 0)
		_log.print("No effect!\n");
}

void UseLogic::gemShard(int which) {
	const Data::Savegame &sg = _G(savegame);

	_log.print("Gem Shard\n\nThou dost hold above thee the evil Shard of ");

	switch (which) {
	case 0:
		_log.print("Falsehood...");
		break;
	case 1:
		_log.print("Hatred...");
		break;
	default:
		_log.print("Cowardice...");
		break;
	}

	g_engine->sound().flare(0xA50, 200);

	if (FLAME_X[which] != sg._mapX || FLAME_Y[which] != sg._mapY || FLAME_MAP[which] != sg._mapId ||
			FLAME_LEVEL[which] != sg._mapLevel) {
		_log.print("\n\nNo effect!\n");
		g_engine->sound().alert();
		return;
	}

	// Casting it into the flame, and what comes of it, needs the shadowlords to be there
	_result = 0;
}

void UseLogic::sextant() {
	const Data::Savegame &sg = _G(savegame);

	_log.print("Sextant\n\n");

	if (sg._mapLevel > 0x7F || sg._mapId != 0) {
		_log.print("Only outdoors!\n");
	} else if (sg._hour > DAY_START - 1 && sg._hour < NIGHT_START) {
		_log.print("Only at night!\n");
	} else {
		_log.print("Position:");

		// The place is told in the letters of the runes
		_log.setRunes(true);
		_log.printChar('\n');
		_log.printChar(((sg._mapY & 0xF0) >> 4) + 0x41);
		_log.printChar('\'');
		_log.printChar((sg._mapY & 0xF) + 0x41);
		_log.print("\", ");
		_log.printChar(((sg._mapX & 0xF0) >> 4) + 0x41);
		_log.printChar('\'');
		_log.printChar((sg._mapX & 0xF) + 0x41);
		_log.printChar('"');
		_log.setRunes(false);
		_log.printChar('\n');
	}
}

void UseLogic::watch() {
	const Data::Savegame &sg = _G(savegame);

	_log.print("Watch\n\nThe pocket watch reads ");

	int hour = sg._hour % 12;
	if (hour == 0)
		hour = 12;

	_log.print(Common::String::format("%d", hour));
	_log.printChar(':');
	_log.print(Common::String::format("%02d", sg._minute));
	_log.print(sg._hour > 11 ? " PM.\n" : " AM.\n");
}

UseLogic::Need UseLogic::other(int item) {
	Data::Savegame &sg = _G(savegame);

	switch (item) {
	case ITEM_CARPET:
		carpet();
		break;

	case ITEM_SKULL_KEY:
		--sg._skullKeys;
		_log.print("Skull Key\n");

		if (sg._mapId < FIRST_DUNGEON || sg._mapId > FIRST_BATTLE - 1) {
			_stage = STAGE_SKULL_KEY;
			promptDirection();
			return NEED_DIRECTION;
		}

		_log.print("Not here!\n");
		break;

	case ITEM_AMULET:
		worn(ICON_AMULET, "Amulet\n\n", "Wearing the Amulet");
		break;

	case ITEM_CROWN:
		worn(ICON_CROWN, "Crown\n\n", "Thou dost don the Crown");
		break;

	case ITEM_SCEPTRE:
		sceptre();
		break;

	case ITEM_FIRST_SHARD:
	case ITEM_FIRST_SHARD + 1:
	case ITEM_LAST_SHARD:
		gemShard(item - ITEM_FIRST_SHARD);
		break;

	case ITEM_SPYGLASS:
		_log.print("Spyglass\n\n");

		if (sg._mapId < FIRST_DUNGEON && sg._mapLevel < 0x80) {
			if (sg._hour < DAY_START || STAR_DAY_END < sg._hour)
				_log.print("Looking...\n");
			else
				_log.print("No stars!\n");
		} else {
			_log.print("Not here!\n");
		}
		break;

	case ITEM_PLANS:
		_log.print("Plans\n\n");

		if ((sg._partyTile & PARTY_SHIP_MASK) == PARTY_SHIP) {
			sg._capePlans |= 0x80;
			_log.print("Ship rigged for double speed!\n");
		} else {
			_log.print("Only usable on shipboard!\n");
		}
		break;

	case ITEM_SEXTANT:
		sextant();
		break;

	case ITEM_WATCH:
		watch();
		break;

	case ITEM_BADGE:
		_log.print("Badge\n\n");

		if (removeWorn(ICON_BADGE)) {
			_log.print("Badge worn!\n");
			setIcon(ICON_BADGE, DURATION_FOREVER);
		}
		break;

	case ITEM_BOX:
		_log.print("Box\n\nHow?\n");
		break;

	default:
		break;
	}

	return NEED_NONE;
}

} // namespace Ultima5
} // namespace Ultima
