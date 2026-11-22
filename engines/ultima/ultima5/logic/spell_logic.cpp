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

#include "ultima/ultima5/logic/spell_logic.h"
#include "ultima/ultima5/data/spell_data.h"
#include "ultima/ultima5/logic/dungeon_items.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/logic/inventory_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int KEY_LEFT = 1;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;

constexpr int SPELLS_PER_CIRCLE = 6;
constexpr int REAGENTS = 8;
constexpr int MAX_STACK = 99;
constexpr int PLACE_OUTDOORS = 8;
constexpr int PLACE_TOWN = 4;
constexpr int PLACE_DUNGEON = 2;
constexpr int PLACE_COMBAT = 1;
constexpr int FIRST_DUNGEON_MAP = 0x21;
constexpr int FIRST_COMBAT_MAP = 0x80;
constexpr int CASTLE_MAP = 0x12;
constexpr int STONEGATE_MAP = 0x1D;
constexpr int DOOM_MAP = 40;
constexpr int SHIP_TILES = 0x20;
constexpr int MOONSTONES = 8;
constexpr int LIGHT_TURNS = 100;
constexpr int LONG_LIGHT_TURNS = 0xFF;
constexpr int MAX_FOOD = 9999;
constexpr int BIGGEST_WINDOW = 0x100;
constexpr int WINDOW_TILES = 0x20;

// The spells by where they are in the list
enum {
	SPELL_LIGHT = 0,
	SPELL_GRAVITY = 1,
	SPELL_AWAKEN = 2,
	SPELL_CURE = 3,
	SPELL_HEAL = 4,
	SPELL_REMOVE = 5,
	SPELL_UNLOCK = 6,
	SPELL_WIND = 8,
	SPELL_POSITION = 9,
	SPELL_FOOD_GOLD = 11,
	SPELL_LONG_LIGHT = 12,
	SPELL_FIREBALL_WAVE = 13,
	SPELL_FIELD_FIRE = 14,
	SPELL_FIELD_POISON = 15,
	SPELL_FIELD_SLEEP = 16,
	SPELL_PORTAL = 17,
	SPELL_DISPEL_FIELD = 18,
	SPELL_PROTECTION = 19,
	SPELL_FIELD_ENERGY = 20,
	SPELL_CLIMB_UP = 21,
	SPELL_CLIMB_DOWN = 22,
	SPELL_LOCK_DOOR = 25,
	SPELL_OPEN_DOOR = 26,
	SPELL_FULL_HEAL = 27,
	SPELL_QUICKNESS = 29,
	SPELL_ENERGY_BOLT = 37,
	SPELL_REPEL = 31,
	SPELL_NEGATE_MAGIC = 32,
	SPELL_WIZARD_EYE = 33,
	SPELL_VIEW = 39,
	SPELL_RESURRECT = 42,
	SPELL_MOON_TRAVEL = 46,
	SPELL_NEGATE_TIME = 47
};

static const byte FIELD_CELLS[4] = { 0x82, 0x81, 0x80, 0x83 };

// What a field spell is as the fight numbers the things that are shot
static const byte FIELD_FIGHT_ITEMS[4] = { 0x35, 0x33, 0x34, 0x36 };
constexpr int FIRST_SPELL_ITEM = 0x30;
constexpr int CREATURE_SPELL_BASE = 0x100;

static Data::Savegame &save() {
	return _G(savegame);
}

// The fight that is cast in, if it is
static CombatLogic *s_combat = nullptr;

SpellLogic::SpellLogic(Views::MessageLog &log, CombatLogic *combat) : _log(log), _combat(combat) {
	s_combat = combat;
}

SpellLogic::~SpellLogic() {
	s_combat = nullptr;
}

static int standX() {
	return s_combat ? s_combat->entity(s_combat->current())._x : save()._mapX;
}

static int standY() {
	return s_combat ? s_combat->entity(s_combat->current())._y : save()._mapY;
}

static byte tileAtPos(int x, int y) {
	if (s_combat)
		return x >= 0 && y >= 0 && x < COMBAT_SIZE && y < COMBAT_SIZE ? s_combat->tile(x, y) : 0;

	return tileAtPos(x, y);
}

static byte &dungeonCell(int x, int y) {
	Data::Savegame &sg = save();
	return sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + (y & 7) * DUNGEON_SIZE + (x & 7)];
}

static void setTileAt(int x, int y, byte tile) {
	if (s_combat) {
		if (x >= 0 && y >= 0 && x < COMBAT_SIZE && y < COMBAT_SIZE)
			s_combat->mapData()[y * COMBAT_MAP_STRIDE + x] = tile;
	} else if (save()._mapId == 0)
		_G(world).setTile(x, y, tile);
	else if (x >= 0 && y >= 0 && x < Data::LOCATION_SIZE && y < Data::LOCATION_SIZE)
		_G(location).setTile(x, y, tile);
}

static bool inDungeon() {
	return DungeonLogic::isDungeon(save()._mapId);
}

// Works out where a direction key points from the party, as the map does
static void aim(int key, int &x, int &y) {
	x = standX();
	y = standY();

	switch (key) {
	case KEY_UP:
		--y;
		break;
	case KEY_DOWN:
		++y;
		break;
	case KEY_RIGHT:
		++x;
		break;
	case KEY_LEFT:
		--x;
		break;
	default:
		break;
	}
}

// The spells that can be cast in a fight so far, which are those that need no one to be aimed at
static bool combatSpell(int spell) {
	switch (spell) {
	case 7:
	case 10:
	case 17:
	case 23:
	case 24:
	case 28:
	case 30:
	case 34:
	case 35:
	case 36:
	case 38:
	case 40:
	case 41:
	case 44:
	case 45:
	case SPELL_GRAVITY:
	case SPELL_FIREBALL_WAVE:
	case SPELL_ENERGY_BOLT:
	case SPELL_FIELD_FIRE:
	case SPELL_FIELD_POISON:
	case SPELL_FIELD_SLEEP:
	case SPELL_FIELD_ENERGY:
	case SPELL_LIGHT:
	case SPELL_AWAKEN:
	case SPELL_CURE:
	case SPELL_HEAL:
	case SPELL_REMOVE:
	case SPELL_UNLOCK:
	case SPELL_FOOD_GOLD:
	case SPELL_LONG_LIGHT:
	case SPELL_PROTECTION:
	case SPELL_LOCK_DOOR:
	case SPELL_OPEN_DOOR:
	case SPELL_FULL_HEAL:
	case SPELL_QUICKNESS:
	case SPELL_REPEL:
	case SPELL_NEGATE_MAGIC:
	case SPELL_RESURRECT:
	case SPELL_NEGATE_TIME:
		return true;
	default:
		return false;
	}
}

int SpellLogic::identify(const char *letters, int count) {
	char sorted[4];
	int i;

	if (count == 0)
		return SPELL_NONE;

	for (i = 0; i < count; ++i)
		sorted[i] = letters[i];

	for (bool swapped = true; swapped;) {
		swapped = false;

		for (i = 0; i < count - 1; ++i) {
			if (sorted[i + 1] < sorted[i]) {
				SWAP(sorted[i], sorted[i + 1]);
				swapped = true;
			}
		}
	}

	for (int spell = 0; spell < Data::SPELL_COUNT; ++spell) {
		const char *name = Data::SPELL_LETTERS[spell];
		bool matches = true;
		int at = 0;

		for (i = 0; i < count; ++i) {
			if (name[at] != sorted[i]) {
				matches = false;
				break;
			}

			// A letter typed more than once counts once
			while (i < count - 1 && sorted[i] == sorted[i + 1])
				++i;

			++at;
		}

		if (matches && name[at] != '\0')
			matches = false;

		if (matches)
			return spell;
	}

	return SPELL_UNKNOWN;
}

bool SpellLogic::hasReagents(int recipe, int amount) {
	const Data::Savegame &sg = save();

	for (int i = 0, bit = 0x80; i < REAGENTS; ++i, bit >>= 1) {
		if ((recipe & bit) != 0 && sg._reagents[i] < amount)
			return false;
	}

	return true;
}

void SpellLogic::mixSpell(int spell, int recipe, int amount) {
	Data::Savegame &sg = save();

	_log.print("Mixing...\n");

	for (int i = 0, bit = 0x80; i < REAGENTS; ++i, bit >>= 1) {
		if ((recipe & bit) != 0)
			sg._reagents[i] -= amount;
	}

	if (spell >= 0 && Data::SPELL_RECIPES[spell] == recipe) {
		_log.print("\nDone!\n");
		sg._mixtures[spell] = MIN(sg._mixtures[spell] + amount, MAX_STACK);
		return;
	}

	// A mixture that doesn't make the spell goes off in the face of whoever is first
	_log.print("\n");

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_GOOD || sg._party[i]._status == Data::STATUS_POISONED) {
			DungeonItems(_log).chestTrap(i);
			break;
		}
	}
}

void SpellLogic::promptDirection() {
	_log.print("Direction-");
}

void SpellLogic::promptMember() {
	_log.print("On who: ");
}

void SpellLogic::setIcon(int icon, int duration) {
	Data::Savegame &sg = save();
	sg._icon = icon;
	sg._protection = duration;
}

SpellLogic::Need SpellLogic::finish() {
	if (_result == 1) {
		_log.print("Success!\n");
	} else if (_result == 0) {
		_log.print("Failed!\n");
	}

	return NEED_NONE;
}

SpellLogic::Need SpellLogic::cast(int caster, int spell) {
	Data::Savegame &sg = save();
	const int level = spell / SPELLS_PER_CIRCLE + 1;
	bool here = true;

	_caster = caster;
	_spell = spell;
	_result = -1;
	_passes = true;

	if (_combat) {
		here = (Data::SPELL_PLACES[spell] & PLACE_COMBAT) != 0 && combatSpell(spell);
	} else if (sg._mapId == 0) {
		here = (Data::SPELL_PLACES[spell] & PLACE_OUTDOORS) != 0;
	} else if (sg._mapId >= FIRST_COMBAT_MAP) {
		here = (Data::SPELL_PLACES[spell] & PLACE_COMBAT) != 0;
	} else {
		if ((sg._mapId == CASTLE_MAP && sg._crown == 0) || sg._mapId == STONEGATE_MAP) {
			_log.print("Absorbed!\n");
			return NEED_NONE;
		}

		here = (Data::SPELL_PLACES[spell] & (sg._mapId < FIRST_DUNGEON_MAP ? PLACE_TOWN : PLACE_DUNGEON)) != 0;
	}

	if (!here) {
		_log.print("Not here!\n");
		return NEED_NONE;
	}

	if (sg._mixtures[spell] == 0) {
		_log.print("None mixed!\n");
		return NEED_NONE;
	}

	--sg._mixtures[spell];

	Data::Character &caster_ = sg._party[caster];

	if (caster_._mag < level) {
		_log.print("M.P. too low!\n");
		_result = 0;
		return finish();
	}

	caster_._mag -= level;

	if (caster_._level < level) {
		_result = 0;
		return finish();
	}

	switch (spell) {
	case SPELL_LIGHT:
		sg._unk0300 = LIGHT_TURNS;
		break;

	case SPELL_LONG_LIGHT:
		sg._unk0300 = LONG_LIGHT_TURNS;
		break;

	case SPELL_GRAVITY:
	case SPELL_FIREBALL_WAVE:
	case SPELL_ENERGY_BOLT:
		_aimItem = FIRST_SPELL_ITEM + (spell == SPELL_GRAVITY ? 0 : spell == SPELL_FIREBALL_WAVE ? 1 : 2);
		return NEED_AIM;

	case SPELL_AWAKEN:
	case SPELL_CURE:
	case SPELL_HEAL:
	case SPELL_FULL_HEAL:
	case SPELL_RESURRECT:
		promptMember();
		return NEED_MEMBER;

	case SPELL_PORTAL:
		// In a fight it takes the caster to a place picked by chance
		if (_combat) {
			_result = _combat->castInstant(spell, caster);
			break;
		}

		promptDirection();
		return NEED_DIRECTION;

	case SPELL_REMOVE:
	case SPELL_WIND:
	case SPELL_OPEN_DOOR:
	case SPELL_LOCK_DOOR:
		promptDirection();
		return NEED_DIRECTION;

	case SPELL_UNLOCK:
		if (inDungeon())
			return unlockSpell();

		promptDirection();
		return NEED_DIRECTION;

	case SPELL_POSITION:
		showPosition();
		break;

	case SPELL_FOOD_GOLD:
		sg._food = MIN(sg._food + _G(gameRandom).range(1, 3), MAX_FOOD);
		_result = 1;
		break;

	case SPELL_FIELD_FIRE:
		return field(0);
	case SPELL_FIELD_POISON:
		return field(1);
	case SPELL_FIELD_SLEEP:
		return field(2);
	case SPELL_FIELD_ENERGY:
		return field(3);

	case SPELL_DISPEL_FIELD:
		return destroyField();

	case SPELL_PROTECTION:
		setIcon(0x50, 0x14);
		break;

	case SPELL_QUICKNESS:
		setIcon(0x51, 0x1E);
		break;

	case SPELL_REPEL:
		setIcon(0x43, 0x14);
		break;

	case SPELL_NEGATE_MAGIC:
		setIcon(0x4E, 10);
		break;

	case SPELL_NEGATE_TIME:
		for (int i = 0; i < Data::ACTOR_COUNT; ++i) {
			if (sg.getActor(i)._tile == 0xFC) {
				_log.print("Magic absorbed!\n");
				_result = 0;
				return finish();
			}
		}

		setIcon(0x54, 10);
		break;

	case 28:
	case 40:
	case 44:
	case 45:
		promptDirection();
		return NEED_DIRECTION;

	case 34:
	case 35:
	case 38:
		_aimItem = CREATURE_SPELL_BASE + spell;
		return NEED_AIM;

	case 7:
	case 10:
	case 23:
	case 24:
	case 30:
	case 36:
	case 41:
		_result = _combat ? _combat->castInstant(spell, caster) : 0;
		break;

	case SPELL_VIEW:
		return NEED_VIEW;

	case SPELL_WIZARD_EYE:
		return NEED_EYE;

	case SPELL_MOON_TRAVEL:
		if ((sg._partyTile & 0xF0) == SHIP_TILES) {
			_result = 0;
			break;
		}

		_log.print("To phase: ");
		return NEED_KEY;

	case SPELL_CLIMB_UP:
	case SPELL_CLIMB_DOWN: {
		if (sg._mapId == DOOM_MAP) {
			_result = 0;
			break;
		}

		DungeonLogic dungeon(_log);
		if (dungeon.climb(spell == SPELL_CLIMB_UP ? -1 : 1))
			dungeon.leave();
		break;
	}

	default:
		break;
	}

	return finish();
}

// Fills the way ahead with a field, if it's open ground
SpellLogic::Need SpellLogic::field(int kind) {
	if (_combat) {
		_aimItem = FIELD_FIGHT_ITEMS[kind];
		return NEED_AIM;
	}

	if (!inDungeon()) {
		_result = 0;
		return finish();
	}

	int x, y;
	DungeonItems::target(KEY_UP, x, y);
	byte &cell = dungeonCell(x, y);

	if ((cell & 0xF7) != 0) {
		_result = 0;
	} else {
		cell = (cell & 8) | FIELD_CELLS[kind];
		_result = -1;
	}

	return finish();
}

// Opens the chest that is here, or ahead, and takes the trap away
SpellLogic::Need SpellLogic::unlockSpell() {
	int x, y;
	byte *cell = &dungeonCell(save()._mapX, save()._mapY);

	if ((*cell & 0xF0) != 0x40) {
		DungeonItems::target(KEY_UP, x, y);
		cell = &dungeonCell(x, y);
	}

	if ((*cell & 0xF0) == 0x40) {
		if ((*cell & 1) != 0)
			_log.print("Disarmed!\n");

		*cell = (*cell & 8) | 0x70;
		_log.print("Chest opened!\n");
		_result = -1;
	} else {
		_result = 0;
	}

	return finish();
}

// Takes away the field that is here, or ahead
SpellLogic::Need SpellLogic::destroyField() {
	int x, y;
	byte *cell = &dungeonCell(save()._mapX, save()._mapY);

	if ((*cell & 0xF0) != 0x80) {
		DungeonItems::target(KEY_UP, x, y);
		cell = &dungeonCell(x, y);
	}

	if ((*cell & 0xF0) == 0x80) {
		*cell &= 8;
		_log.print("Field destroyed!\n");
		_result = -1;
	} else {
		_result = 0;
	}

	return finish();
}

// Writes where the party is, in the letters of the runes
void SpellLogic::showPosition() {
	const Data::Savegame &sg = save();

	_log.setRunes(true);
	_log.printChar('\n');
	_log.printChar(((sg._mapY & 0xF0) >> 4) + 'A');
	_log.printChar('\'');
	_log.printChar((sg._mapY & 0xF) + 'A');
	_log.print("\", ");
	_log.printChar(((sg._mapX & 0xF0) >> 4) + 'A');
	_log.printChar('\'');
	_log.printChar((sg._mapX & 0xF) + 'A');
	_log.printChar('"');
	_log.setRunes(false);
	_log.printChar('\n');
}

SpellLogic::Need SpellLogic::windSpell(int key) {
	Data::Savegame &sg = save();

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
		return finish();
	}

	sg._windTicks = 0;
	return finish();
}

SpellLogic::Need SpellLogic::directionChosen(int key) {
	Data::Savegame &sg = save();
	int x, y;

	if (key == 0) {
		_log.print("Pass\n");
		return finish();
	}

	switch (key) {
	case KEY_UP:
		_log.print("North\n");
		break;
	case KEY_RIGHT:
		_log.print("East\n");
		break;
	case KEY_DOWN:
		_log.print("South\n");
		break;
	default:
		_log.print("West\n");
		break;
	}

	aim(key, x, y);

	switch (_spell) {
	case SPELL_WIND:
		return windSpell(key);

	case 28:
	case 40:
	case 44:
	case 45:
		if (_combat)
			_combat->castWave(_spell == 28 ? 1 : _spell == 40 ? 2 : _spell == 45 ? 3 : 4, key, _caster);
		return finish();

	case SPELL_REMOVE: {
		static const byte REMOVABLE[13] = { 0x5B, 0x90, 0x91, 0x92, 0x93, 0x9D, 0xA5, 0xA6, 0xA8, 0xA9, 0xAD, 0xAE, 0xAF };
		const byte tile = tileAtPos(x, y);

		_result = 0;
		for (int i = 0; i < 13; ++i) {
			if (tile == REMOVABLE[i]) {
				setTileAt(x, y, 0x44);
				_log.print("POOF!\n");
				_result = -1;
				break;
			}
		}
		break;
	}

	case SPELL_UNLOCK: {
		const byte tile = tileAtPos(x, y);
		_result = 0;

		if (tile == 0xB9 || tile == 0xBB) {
			setTileAt(x, y, tile - 1);
			_result = 1;
			break;
		}

		for (int i = 0; i < Data::ACTOR_COUNT; ++i) {
			Data::ActorRecord a = s_combat ? s_combat->actor(i) : sg.getActor(i);

			if (a._tile == 1 && a._x == x && a._y == y && (s_combat || a._z == sg._mapLevel)) {
				a._hull &= 0x7F;

				if (s_combat)
					s_combat->actorRef(i) = a;
				else
					sg.setActor(i, a);
				_result = 1;
				break;
			}
		}
		break;
	}

	case SPELL_LOCK_DOOR: {
		const byte tile = tileAtPos(x, y);
		_result = 0;

		if (tile == 0xB8 || tile == 0xB9) {
			setTileAt(x, y, 0x97);
			_result = 1;
		} else if (tile == 0xBA || tile == 0xBB) {
			setTileAt(x, y, 0x98);
			_result = 1;
		}
		break;
	}

	case SPELL_OPEN_DOOR: {
		const byte tile = tileAtPos(x, y);
		_result = 0;

		if (tile == 0x97) {
			setTileAt(x, y, 0xB8);
			_result = 1;
		} else if (tile == 0x98) {
			setTileAt(x, y, 0xBA);
			_result = 1;
		}
		break;
	}

	case SPELL_PORTAL: {
		const int dx = x - sg._mapX;
		const int dy = y - sg._mapY;
		const int right = MIN(sg._chunkX + WINDOW_TILES, BIGGEST_WINDOW);
		const int bottom = MIN(sg._chunkY + WINDOW_TILES, BIGGEST_WINDOW);
		bool moved = false;
		int px = (byte)x;

		for (int py = (byte)y; sg._chunkX <= px && px < right && sg._chunkY <= py && py < bottom; py += dy) {
			if (g_engine->getMapTile(px, py) == Data::TILE_GRASS) {
				moved = true;
				sg._mapX = px;
				sg._mapY = py;
			}

			px += dx;
		}

		if (moved) {
			WorldLogic::centerWindow(sg);
			_G(visibility).invalidate();
		}

		_result = moved ? -1 : 0;
		break;
	}

	default:
		break;
	}

	return finish();
}

// Goes to where one of the moonstones is buried, if it is outdoors
SpellLogic::Need SpellLogic::keyChosen(int key) {
	Data::Savegame &sg = save();

	if (key >= ' ')
		_log.printChar(key);
	_log.printChar('\n');

	_result = 0;

	if (key >= '1' && key < '1' + MOONSTONES) {
		const int stone = key - '1';

		if (WorldLogic::jumpToMoonstone(sg, stone)) {
			_result = -1;
			_passes = false;
		}
	}

	return finish();
}

SpellLogic::Need SpellLogic::memberChosen(int member) {
	Data::Savegame &sg = save();

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

	Data::Character &c = sg._party[member];

	switch (_spell) {
	case SPELL_AWAKEN:
		if (c._status == Data::STATUS_SLEEP) {
			c._status = Data::STATUS_GOOD;
			_result = -1;

			if (_combat)
				_combat->wakeMember(member);
		} else {
			_result = 0;
		}
		break;

	case SPELL_CURE:
		if (c._status == Data::STATUS_POISONED) {
			c._status = Data::STATUS_GOOD;
			_result = 1;
		} else {
			_result = 0;
		}
		break;

	case SPELL_HEAL:
		if (c._status == Data::STATUS_DEAD) {
			_result = 0;
		} else {
			int amount = _G(gameRandom).range(0, 0x3C) / 2;
			c._hp = MIN<int>(c._hp + (amount == 0 ? 1 : amount), c._maxHp);
			_result = 1;
		}
		break;

	case SPELL_FULL_HEAL:
		if (c._status == Data::STATUS_DEAD) {
			_result = 0;
		} else {
			c._hp = c._maxHp;
			_result = 1;
		}
		break;

	case SPELL_RESURRECT:
		if (c._status == Data::STATUS_DEAD) {
			InventoryLogic::resurrect(sg, member);
			_result = 1;
		} else {
			_result = 0;
		}
		break;

	default:
		break;
	}

	return finish();
}

} // namespace Ultima5
} // namespace Ultima
