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

#include "ultima/ultima5/logic/object_items.h"
#include "ultima/ultima5/data/object_data.h"
#include "ultima/ultima5/logic/attack_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

// What can be lying about, as the objects are numbered
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
constexpr int OBJECT_BOX = 0x0E;
constexpr int OBJECT_TORCH = 0x0D;
constexpr int OBJECT_FOOD = 0x0F;
constexpr int OBJECT_FIRST_CREATURE = 0x10;
constexpr int OBJECT_MOONSTONE = 0x19;
constexpr int OBJECT_CARPET = 0x1B;
constexpr int OBJECT_SLEEPER = 0x1E;
constexpr int OBJECT_BODY = 0x1F;
constexpr int OBJECT_SHARD = 0xB4;

// The tiles of the maps that it matters what they are
constexpr byte TILE_STUMP = 0x2B;
constexpr byte TILE_CROPS_PICKED = 0x2C;
constexpr byte TILE_CROPS = 0x2D;
constexpr byte TILE_FLOOR = 0x44;
constexpr byte TILE_HIDDEN_DOOR = 0x4E;
constexpr byte TILE_WALL = 0x4F;
constexpr byte TILE_SHELF = 0x5A;
constexpr byte TILE_BOOKSHELF = 0x5C;
constexpr byte TILE_CELL_A = 0x84;
constexpr byte TILE_CELL_B = 0x85;
constexpr byte TILE_PLATE = 0x95;
constexpr byte TILE_LOCKED_A = 0x97;
constexpr byte TILE_LOCKED_B = 0x98;
constexpr byte TILE_HEAVY = 0x99;
constexpr byte TILE_TABLE_A = 0x9A;
constexpr byte TILE_TABLE_B = 0x9B;
constexpr byte TILE_TABLE_C = 0x9C;
constexpr byte TILE_WELL = 0xA1;
constexpr byte TILE_DESK = 0xA5;
constexpr byte TILE_BARREL = 0xA6;
constexpr byte TILE_VANITY = 0xA8;
constexpr byte TILE_BED = 0xAB;
constexpr byte TILE_DRESSER = 0xAD;
constexpr byte TILE_TRUNK = 0xAF;
constexpr byte TILE_SCONCE_A = 0xB0;
constexpr byte TILE_SCONCE_B = 0xB1;
constexpr byte TILE_BRAZIER = 0xB2;
constexpr byte TILE_DOOR_OPEN_A = 0xB8;
constexpr byte TILE_DOOR_LOCKED_A = 0xB9;
constexpr byte TILE_DOOR_OPEN_B = 0xBA;
constexpr byte TILE_DOOR_LOCKED_B = 0xBB;
constexpr byte TILE_FIREPLACE = 0xBC;
constexpr byte TILE_MOONGATE = 0xDC;

constexpr byte TILE_STAIR_DOWN = 0x86;
constexpr byte TILE_LADDER_UP = 0xC8;
constexpr byte TILE_LADDER_DOWN = 0xC9;
constexpr byte TILE_CLIMB_A = 0x4C;
constexpr byte TILE_CLIMB_B = 0xCA;
constexpr byte TILE_CLIMB_C = 0xCB;
constexpr byte TILE_MOUNTAIN = 0x0C;
constexpr byte TILE_IMPASSABLE = 0x0D;
constexpr byte TILE_CHAIR = 0x90;
constexpr byte TILE_CANNON = 0xB4;
constexpr byte TILE_CANNON_FLOOR = 0x45;
constexpr byte PARTY_ON_FOOT = 0x1C;
constexpr byte PARTY_RIDING_HORSE = 0x12;
constexpr int FALL_DAMAGE = 5;
constexpr int WINDOW_MARGIN = 5;
constexpr int WINDOW_LAST = 0x1A;
constexpr int CHUNK_STEP = 16;
constexpr int LOCATION_TILES = 32;
constexpr int FIRST_COMBAT_MAP = 0x80;
constexpr int LAST_LOCATION_MAP = 0x20;
constexpr int DOOR_OPEN_TURNS = 4;
constexpr int SCONCE_LIGHT = 100;
constexpr int KARMA_LIMIT = 99;
constexpr int FOOD_LIMIT = 9999;
constexpr int FIRST_FREE_SLOTS = 0x18;
constexpr int SKILL_RANGE = 0x1E;
constexpr int DEX_RANGE = 0x1D;
constexpr int SPAWN_ODDS = 0x1E;
constexpr int OPEN_KARMA_COST = 2;
constexpr int PRISONER_KARMA_GAIN = 2;
constexpr int PRISONER_SCHEDULE = 5;
constexpr int HERB_HOUR = 0;
constexpr int HERB_PATCHES = 3;
constexpr int HERB_MIN = 2;
constexpr int HERB_MAX = 0xF;
constexpr int MAX_STACK = 99;
constexpr int LOCKED_FLAG = 0x80;
constexpr int KEY_ITEM_DAY_ENTRY = 0xE;
constexpr int KEY_ITEM_ENTRY = 0xD;
constexpr int KEY_ITEM_LAST_ENTRY = 0xF;
constexpr int ANKH_EQUIPMENT = 0x27;

static Data::Savegame &save() {
	return _G(savegame);
}

// Whether an object is on the square, on the level the party is on
static bool atSquare(const Data::ActorRecord &a, int x, int y) {
	return a._x == x && a._y == y && a._z == save()._mapLevel;
}

// The first of the objects on a square, going up from the first after the party's, and its kind
static int firstAt(int x, int y, byte &tile) {
	const Data::Savegame &sg = save();

	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord a = sg.getActor(i);

		if (atSquare(a, x, y)) {
			tile = a._tile;
			return i;
		}
	}

	tile = 0;
	return 0;
}

// The first chest of a square, if it has one
static int chestAt(int x, int y) {
	const Data::Savegame &sg = save();

	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord a = sg.getActor(i);

		if (atSquare(a, x, y) && a._tile == OBJECT_CHEST)
			return i;
	}

	return 0;
}

static void raise(byte &value, int amount, int limit) {
	value = value + amount < limit ? value + amount : limit;
}

static void lowerKarma(int amount) {
	Data::Savegame &sg = save();
	sg._karma = sg._karma > amount ? sg._karma - amount : 0;
}

byte ObjectItems::tileAt(int x, int y) {
	return g_engine->getMapTile(x, y);
}

void ObjectItems::setTileAt(int x, int y, byte tile) {
	if (save()._mapId == 0)
		_G(world).setTile(x, y, tile);
	else if (x >= 0 && y >= 0 && x < LOCATION_TILES && y < LOCATION_TILES)
		_G(location).setTile(x, y, tile);
}

// The last of the objects that isn't being used, or the first if there is none
int ObjectItems::freeSlot() {
	const Data::Savegame &sg = save();

	for (int i = Data::ACTOR_COUNT - 1; i >= 0; --i) {
		if (sg.getActor(i)._tile == 0)
			return i;
	}

	return 0;
}

// Looks for one of the objects, of kinds from one to another, that can be used again
int ObjectItems::scanSlots(int lowest, int highest, bool offscreen) {
	const Data::Savegame &sg = save();

	for (int i = 1; i < FIRST_FREE_SLOTS; ++i) {
		const Data::ActorRecord a = sg.getActor(i);

		if (a._tile < lowest || a._tile > highest || a._tile == 0xB5)
			continue;

		if (!offscreen)
			return i;

		const byte across = a._x - sg._mapX + 5;
		const byte down = a._y - sg._mapY + 5;
		if (across > 10 || down > 10)
			return i;
	}

	return 0;
}

// Finds an object to use for what turns up, taking from what is least wanted
int ObjectItems::pickSlot() {
	static const byte LOWEST[10] = { 0, 1, 0x80, 0x10, 0x30, 1, 0x80, 0x10, 0x30, 0 };
	static const byte HIGHEST[10] = { 0, 0xF, 0xFF, 0x11, 0x7F, 0xF, 0xFF, 0x11, 0x7F, 0xFF };

	for (int i = 0; i < 10; ++i) {
		const int slot = scanSlots(LOWEST[i], HIGHEST[i], i != 0 && i < 5);

		if (slot != 0)
			return slot;
	}

	return 0;
}

void ObjectItems::place(int slot, int tile, int x, int y, int z, int amount) {
	Data::Savegame &sg = save();
	Data::ActorRecord a = sg.getActor(slot);

	a._tile = a._animTile = tile;
	a._x = x;
	a._y = y;
	a._z = z;
	a._hull = amount;
	sg.setActor(slot, a);
}

void ObjectItems::clear(int slot) {
	Data::Savegame &sg = save();
	Data::ActorRecord a = sg.getActor(slot);

	a._tile = a._animTile = a._x = a._y = a._z = a._hull = 0;
	sg.setActor(slot, a);
}

void ObjectItems::closeDoor() {
	const Data::Savegame &sg = save();

	if (sg._doorOpen != 0 && sg._mapId != 0 && sg._mapId <= LAST_LOCATION_MAP)
		setTileAt(sg._doorX, sg._doorY, sg._doorOpen);
}

bool ObjectItems::ready() {
	if (_kind == KIND_JIMMY && save()._keys == 0) {
		_log.print("No Keys!\n");
		return false;
	}

	if (_kind == KIND_KLIMB)
		return climbReady();

	return true;
}

void ObjectItems::cancel() {
	// Nothing is climbed in a town if no way is given
	if (_kind == KIND_KLIMB && save()._mapId != 0)
		_passes = false;
}

// Gets ready to climb. Ladders and stairs are used where the party stands
bool ObjectItems::climbReady() {
	Data::Savegame &sg = save();

	if (sg._mapId == 0) {
		if (sg._grapple == 0) {
			_log.print("With what?\n");
			return false;
		}

		if (sg._partyTile != PARTY_ON_FOOT) {
			_log.print("On foot!\n");
			return false;
		}

		return true;
	}

	if ((sg._partyTile & 0xFE) == PARTY_RIDING_HORSE) {
		_log.print("-On foot!\n");
		_passes = false;
		return false;
	}

	const byte here = tileAt(sg._mapX, sg._mapY);
	int change = 0;

	if (here == TILE_LADDER_UP)
		change = 1;
	else if (here == TILE_STAIR_DOWN || here == TILE_LADDER_DOWN)
		change = -1;

	if (change == 0)
		return true;

	sg._mapLevel += change;
	_log.print(change > 0 ? "Up!\n" : "Down!\n");
	sg._doorOpen = 0;
	NpcLogic::place(sg);
	return false;
}

void ObjectItems::climb() {
	Data::Savegame &sg = save();
	const byte tile = tileAt(_x, _y);

	if (sg._mapId != 0) {
		if (tile != TILE_CLIMB_A && tile != TILE_CLIMB_B && tile != TILE_CLIMB_C) {
			_log.print("What?\n");
			_passes = false;
			return;
		}

		sg._mapX = _x;
		sg._mapY = _y;
		return;
	}

	if (tile == TILE_IMPASSABLE) {
		_log.print("Impassable!\n");
		return;
	}

	if (tile != TILE_MOUNTAIN) {
		_log.print("Not climbable!\n");
		return;
	}

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status != Data::STATUS_DEAD && sg._party[i]._dex < _G(gameRandom).range(1, SKILL_RANGE)) {
			_log.print("Fell!\n");
			PartyLogic::hurtMember(i, _G(gameRandom).range(1, FALL_DAMAGE));
		}
	}

	sg._mapX = _x;
	sg._mapY = _y;

	// The window of the map moves on when the party gets near its edge
	const int across = (sg._mapX - sg._chunkX) & 0x1F;
	const int down = (sg._mapY - sg._chunkY) & 0x1F;
	if (across < WINDOW_MARGIN || across > WINDOW_LAST || down < WINDOW_MARGIN || down > WINDOW_LAST) {
		sg._chunkX = (_dx * CHUNK_STEP + sg._chunkX) & 0xF0;
		sg._chunkY = (_dy * CHUNK_STEP + sg._chunkY) & 0xF0;
		_G(visibility).invalidate();
	}
}

// What can be pushed about
static bool pushable(byte tile) {
	switch (tile) {
	case 0x5B:
	case 0x90:
	case 0x91:
	case 0x92:
	case 0x93:
	case TILE_DESK:
	case TILE_BARREL:
	case TILE_VANITY:
	case 0xA9:
	case TILE_DRESSER:
	case 0xAE:
	case TILE_TRUNK:
	case 0xB4:
	case 0xB5:
	case 0xB6:
	case 0xB7:
		return true;
	default:
		return false;
	}
}

// Chairs and cannons turn to face the way they were moved
static byte faced(int dx, int dy, byte tile, bool pulled) {
	if (dx == 1 && dy == 0)
		tile += 1;
	if (dx == 0 && dy == 1)
		tile += 2;
	if (dx == -1 && dy == 0)
		tile += 3;

	return pulled ? tile ^ 2 : tile;
}

void ObjectItems::push() {
	Data::Savegame &sg = save();
	const byte tile = tileAt(_x, _y);
	byte there;

	firstAt(_x, _y, there);

	if (there != 0 || !pushable(tile)) {
		_log.print("Won't budge!\n");
		return;
	}

	const byte floor = (tile & 0xFC) == TILE_CANNON ? TILE_CANNON_FLOOR : TILE_FLOOR;
	const int beyondX = _x + _dx;
	const int beyondY = _y + _dy;
	const byte beyond = tileAt(beyondX, beyondY);
	const byte standing = tileAt(sg._mapX, sg._mapY);
	const byte base = tile & 0xFC;
	const bool turns = base == TILE_CHAIR || base == TILE_CANNON;

	firstAt(beyondX, beyondY, there);

	if (there == 0 && beyond == floor) {
		_log.print("Pushed!\n");
		setTileAt(beyondX, beyondY, tile);
		setTileAt(_x, _y, beyond);

		if (turns)
			setTileAt(beyondX, beyondY, faced(_dx, _dy, base, false));
	} else if (standing == floor) {
		_log.print("Pulled!\n");
		setTileAt(sg._mapX, sg._mapY, tile);
		setTileAt(_x, _y, standing);

		if (turns)
			setTileAt(sg._mapX, sg._mapY, faced(_dx, _dy, base, true));
	} else {
		_log.print("Won't budge\n");
		return;
	}

	sg._mapX += _dx;
	sg._mapY += _dy;
}

ObjectItems::Need ObjectItems::begin(int dx, int dy) {
	const Data::Savegame &sg = save();

	_dx = dx;
	_dy = dy;
	_x = sg._mapX + dx;
	_y = sg._mapY + dy;

	switch (_kind) {
	case KIND_GET:
		get();
		return NEED_NONE;

	case KIND_OPEN:
		return open();

	case KIND_JIMMY:
		return jimmy();

	case KIND_KLIMB:
		climb();
		return NEED_NONE;

	case KIND_PUSH:
		push();
		return NEED_NONE;

	default:
		return NEED_WHO;
	}
}

void ObjectItems::finish(int who) {
	switch (_kind) {
	case KIND_OPEN:
		openChest(who);
		break;

	case KIND_JIMMY:
		if (_lock == LOCK_DOOR)
			jimmyDoor(who);
		else if (_lock == LOCK_PRISONER)
			jimmyPrisoner(who);
		else
			jimmyChest(who);
		break;

	case KIND_SEARCH:
		search(who);
		break;

	default:
		break;
	}
}

// Says what is found
void ObjectItems::describe(int tile) {
	switch (tile) {
	case OBJECT_CHEST:
		_log.print("a chest!\n");
		break;
	case OBJECT_GOLD:
		_log.print("a sack of gold!\n");
		break;
	case OBJECT_POTION:
		_log.print("a potion!\n");
		break;
	case OBJECT_SCROLL:
		_log.print("a scroll!\n");
		break;
	case OBJECT_WEAPON:
		_log.print("a weapon!\n");
		break;
	case OBJECT_SHIELD:
		_log.print("a shield!\n");
		break;
	case OBJECT_KEY:
		_log.print("a ring of keys!\n");
		break;
	case OBJECT_GEM:
		_log.print("a gem!\n");
		break;
	case OBJECT_HELM:
		_log.print("a helm!\n");
		break;
	case OBJECT_RING:
		_log.print("a ring!\n");
		break;
	case OBJECT_ARMOUR:
		_log.print("some armour!\n");
		break;
	case OBJECT_AMULET:
		_log.print("an amulet!\n");
		break;
	case OBJECT_TORCH:
		_log.print("some torches!\n");
		break;
	case OBJECT_FOOD:
		_log.print("some food!\n");
		break;
	case OBJECT_MOONSTONE:
		_log.print("a strange rock!\n");
		break;
	case OBJECT_SLEEPER:
		_log.print("a rotting body!\n");
		break;
	case OBJECT_BODY:
		_log.print("a moldy corpse!\n");
		break;
	default:
		_log.print("Nothing of note.\n");
		break;
	}
}

void ObjectItems::get() {
	Data::Savegame &sg = save();
	int object = 0;
	Data::ActorRecord found;

	_log.print("\n");

	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord a = sg.getActor(i);

		if (atSquare(a, _x, _y) && (a._tile < OBJECT_FIRST_CREATURE || a._tile == OBJECT_MOONSTONE ||
				a._tile == OBJECT_CARPET || (a._tile & 0xFC) == OBJECT_SHARD)) {
			object = i;
			found = a;
			break;
		}
	}

	if (object != 0) {
		_items.award(found._tile, found._hull, object);
		return;
	}

	const byte tile = tileAt(_x, _y);

	switch (tile) {
	case TILE_SCONCE_A:
	case TILE_SCONCE_B:
		setTileAt(_x, _y, TILE_FLOOR);
		sg._unk0300 = SCONCE_LIGHT;
		_log.print("Borrowed!\n");
		break;

	case TILE_CROPS:
		setTileAt(_x, _y, TILE_CROPS_PICKED);
		_log.print("Crops picked!\n");
		sg._food = MIN(sg._food + 1, FOOD_LIMIT);

		if (sg._karma != 0)
			--sg._karma;
		break;

	case TILE_TABLE_A:
	case TILE_TABLE_B:
		if (_dy != (tile == TILE_TABLE_A ? 1 : -1)) {
			_log.print("Can't reach plate!\n");
			break;
		}

		setTileAt(_x, _y, TILE_PLATE);
		_log.print("Mmmmm...!\n");
		sg._food = MIN(sg._food + 1, FOOD_LIMIT);

		if (sg._karma != 0)
			--sg._karma;
		break;

	case TILE_TABLE_C:
		if (_dx == 1 || _dx == -1) {
			_log.print("Can't reach plate!\n");
			break;
		}

		if (_dy == 1)
			setTileAt(_x, _y, TILE_TABLE_B);
		if (_dy == -1)
			setTileAt(_x, _y, TILE_TABLE_A);

		_log.print("Mmmmm...!\n");
		sg._food = MIN(sg._food + 1, FOOD_LIMIT);

		if (sg._karma != 0)
			--sg._karma;
		break;

	default:
		_log.print("Nothing to get!\n");
		break;
	}
}

ObjectItems::Need ObjectItems::open() {
	Data::Savegame &sg = save();
	const byte tile = tileAt(_x, _y);

	switch (tile) {
	case TILE_TRUNK:
		_log.print("It's open!\n");
		return NEED_NONE;

	case TILE_HEAVY:
		_log.print("Too heavy!\n");
		return NEED_NONE;

	case TILE_LOCKED_A:
	case TILE_LOCKED_B:
	case TILE_DOOR_LOCKED_A:
	case TILE_DOOR_LOCKED_B:
		_log.print("Locked!\n");
		return NEED_NONE;

	case TILE_DOOR_OPEN_A:
	case TILE_DOOR_OPEN_B:
		sg._doorOpen = tile;
		sg._doorTurns = DOOR_OPEN_TURNS;
		sg._doorX = _x;
		sg._doorY = _y;
		setTileAt(_x, _y, TILE_FLOOR);
		_log.print("Opened!\n");
		return NEED_NONE;

	default:
		break;
	}

	// A chest, if there is one, but a box is not to be opened
	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord a = sg.getActor(i);

		if (!atSquare(a, _x, _y))
			continue;

		if (a._tile == OBJECT_CHEST) {
			_object = i;
			return NEED_WHO;
		}

		if (a._tile == OBJECT_BOX) {
			_log.print("Can't!\n");
			return NEED_NONE;
		}
	}

	_log.print("Nothing to open!\n");
	return NEED_NONE;
}

void ObjectItems::openChest(int who) {
	Data::Savegame &sg = save();
	int power = sg.getActor(_object)._hull;

	clear(_object);

	if (sg._mapId >= 1 && sg._mapId <= LAST_LOCATION_MAP)
		lowerKarma(OPEN_KARMA_COST);

	if (power >= LOCKED_FLAG) {
		power &= 0x7F;
		_log.print("Trapped!\n");
		_items.chestTrap(who);
	}

	spawnLoot(power);
}

ObjectItems::Need ObjectItems::jimmy() {
	const byte tile = tileAt(_x, _y);

	switch (tile) {
	case TILE_DOOR_LOCKED_A:
	case TILE_DOOR_LOCKED_B:
		_tile = tile;
		_lock = LOCK_DOOR;
		return NEED_WHO;

	case TILE_LOCKED_A:
	case TILE_LOCKED_B:
		_log.print("Key broke!\n");
		--save()._keys;
		return NEED_NONE;

	case TILE_CELL_A:
	case TILE_CELL_B: {
		byte inside;
		_object = firstAt(_x, _y, inside);

		if (inside == 0) {
			_log.print("No one is there!\n");
			return NEED_NONE;
		}

		_lock = LOCK_PRISONER;
		return NEED_WHO;
	}

	default:
		break;
	}

	_object = chestAt(_x, _y);
	if (_object == 0) {
		_log.print("No lock!\n");
		return NEED_NONE;
	}

	_lock = LOCK_CHEST;
	return NEED_WHO;
}

void ObjectItems::jimmyDoor(int who) {
	Data::Savegame &sg = save();

	if (sg._party[who]._dex <= _G(gameRandom).range(0, DEX_RANGE)) {
		_log.print("Key broke!\n");
		--sg._keys;
		return;
	}

	setTileAt(_x, _y, _tile - 1);
	_log.print("Unlocked!\n");
}

void ObjectItems::jimmyPrisoner(int who) {
	Data::Savegame &sg = save();

	if (sg._party[who]._dex <= _G(gameRandom).range(0, DEX_RANGE)) {
		_log.print("Key broke!\n");
		--sg._keys;
		return;
	}

	const int npc = AttackLogic::findNpc(_object);
	if (npc == -1) {
		_log.print("Couldn't find this npc\n\n");
		return;
	}

	Data::NpcState state = sg.getNpcState(npc);
	state._dialog = 0;
	sg.setNpcState(npc, state);

	if (!sg.isNpcKilled(npc)) {
		Data::NpcSchedule schedule = sg.getSchedule(npc);
		for (int i = 0; i < Data::NPC_SCHEDULE_SLOTS; ++i)
			schedule._type[i] = PRISONER_SCHEDULE;
		sg.setSchedule(npc, schedule);

		_log.print("\n\"I thank thee!\"\n");
		raise(sg._karma, PRISONER_KARMA_GAIN, KARMA_LIMIT);
	}

	sg.setNpcKilled(npc);
}

void ObjectItems::jimmyChest(int who) {
	Data::Savegame &sg = save();
	Data::ActorRecord chest = sg.getActor(_object);

	if (chest._hull >= LOCKED_FLAG) {
		const byte skill = ((chest._hull & 0x7F) - sg._party[who]._dex + SKILL_RANGE) >> 1;

		if (skill < _G(gameRandom).range(1, SKILL_RANGE)) {
			_log.print("Success!\n");
			chest._hull &= 0x7F;
			sg.setActor(_object, chest);
			return;
		}
	}

	_log.print("Key broke!\n");
	--sg._keys;
}

void ObjectItems::search(int who) {
	Data::Savegame &sg = save();

	// Chests and bodies come first
	const int chest = chestAt(_x, _y);
	if (chest != 0) {
		_log.print("\nThou dost find\n");
		trapSense(chest, who);
		return;
	}

	for (int i = Data::ACTOR_COUNT - 1; i >= 0; --i) {
		const Data::ActorRecord a = sg.getActor(i);

		if (!atSquare(a, _x, _y))
			continue;

		if (a._tile == OBJECT_BODY) {
			_log.print("\nThou dost find\n");
			searchBody(i, who);
			return;
		}
		break;
	}

	const byte tile = tileAt(_x, _y);

	switch (tile) {
	case TILE_STUMP:
		_log.print("\nIn the stump\nt");
		break;
	case TILE_SHELF:
		_log.print("\nOn the shelf\nt");
		break;
	case TILE_BOOKSHELF:
	case TILE_BOOKSHELF + 1:
		_log.print("\nIn the bookshelf\nt");
		break;
	case TILE_WELL:
		_log.print("\nNear the well\nt");
		break;
	case TILE_DESK:
		_log.print("\nIn the desk\nt");
		break;
	case TILE_BARREL:
		_log.print("\nIn the barrel\nt");
		break;
	case TILE_VANITY:
		_log.print("\nIn the vanity\nt");
		break;
	case TILE_BED:
	case TILE_BED + 1:
		_log.print("\nUnder the bed\nt");
		break;
	case TILE_DRESSER:
		_log.print("\nIn the dresser\nt");
		break;
	case TILE_TRUNK:
		_log.print("\nIn the trunk\nt");
		break;
	case TILE_FIREPLACE:
		_log.print("\nIn the fireplace\nt");
		break;
	case TILE_BRAZIER:
		_log.print("\nIn the brazier\nt");
		break;
	case TILE_WALL:
		_log.print("\nIn the wall\nt");
		break;
	default:
		_log.print("\nT");
		break;
	}

	_log.print("hou dost find\n");

	if (tile == TILE_HIDDEN_DOOR) {
		_log.print("a hidden door!\n");
		setTileAt(_x, _y, sg._mapLevel < FIRST_COMBAT_MAP ? TILE_DOOR_LOCKED_A : TILE_DOOR_OPEN_A);
		return;
	}

	if (tile != TILE_MOONGATE && searchMoonstone(_x, _y))
		return;

	if (!searchHerbs(_x, _y))
		searchHidden(_x, _y);
}

// What a look at a chest says of its trap
void ObjectItems::trapSense(int object, int who) {
	const Data::Savegame &sg = save();
	const int trap = sg.getActor(object)._hull;
	const int intel = sg._party[who]._int;
	const int worth = (trap & LOCKED_FLAG) == 0 ? (SKILL_RANGE - intel) >> 1 : ((trap & 0x7F) - intel + SKILL_RANGE) >> 1;
	const bool sensed = worth <= _G(gameRandom).range(1, SKILL_RANGE);

	if ((sensed && (trap & LOCKED_FLAG) == 0) || (!sensed && (trap & LOCKED_FLAG) != 0)) {
		_log.print("no trap!\n");
		return;
	}

	const int level = trap & 0x7F;

	if (sensed && level < 10)
		_log.print("a simple trap!\n");
	else if (sensed && level > 0x14)
		_log.print("a complex trap!\n");
	else
		_log.print("a trap!\n");
}

// What is on a body
void ObjectItems::searchBody(int object, int who) {
	Data::Savegame &sg = save();

	if (_G(gameRandom).range(0, 7) != 0) {
		clear(object);

		if (_G(gameRandom).range(0, 0x1F) == 0x13) {
			_log.print("Plague!\n");
			sg._party[who]._status = Data::STATUS_POISONED;
			return;
		}

		switch (_G(gameRandom).range(0, _G(gameRandom).range(0, 3))) {
		case 0:
			_log.print("nothing!\n");
			break;
		case 1:
			_log.print("worms!\n");
			break;
		case 2:
			_log.print("guts!\n");
			break;
		default:
			_log.print("a bloody pulp!\n");
			break;
		}
		return;
	}

	Data::ActorRecord a = sg.getActor(object);

	if (_G(gameRandom).range(0, 3) == 0) {
		_log.print("food!\n");
		a._tile = a._animTile = OBJECT_FOOD;
	} else {
		_log.print("gold!\n");
		a._tile = a._animTile = OBJECT_GOLD;
	}

	a._hull = _G(gameRandom).range(1, 3);
	sg.setActor(object, a);
}

// A moonstone buried here comes up, unless it has
bool ObjectItems::searchMoonstone(int x, int y) {
	Data::Savegame &sg = save();

	for (int i = 7; i >= 0; --i) {
		if (sg._moonstoneX[i] != x || sg._moonstoneY[i] != y || sg._moonstoneZ[i] != sg._mapLevel ||
				sg._moonstoneState[i] != sg._mapId)
			continue;

		byte tile;
		const int top = firstAt(x, y, tile);

		if (tile == OBJECT_MOONSTONE && sg.getActor(top)._hull == i)
			continue;

		place(pickSlot(), OBJECT_MOONSTONE, x, y, sg._mapLevel, i);
		_log.print("a strange rock!\n");
		return true;
	}

	return false;
}

// Reagents grow in three places and are picked at midnight, once a day
bool ObjectItems::searchHerbs(int x, int y) {
	Data::Savegame &sg = save();
	static const char *const NAMES[HERB_PATCHES] = { "mandrake root!", "mandrake root!", "nightshade!" };

	for (int i = 0; i < HERB_PATCHES; ++i) {
		if (Data::HERB_X[i] != x || Data::HERB_Y[i] != y || sg._hour != HERB_HOUR || sg._harvestDays[i] == sg._day)
			continue;

		sg._harvestDays[i] = sg._day;

		const int sprigs = _G(gameRandom).range(HERB_MIN, HERB_MAX);
		byte &reagent = sg._reagents[Data::HERB_REAGENT[i]];
		reagent = MIN(reagent + sprigs, MAX_STACK);

		_log.print(Common::String::format("%d sprigs of\n%s\n", sprigs, NAMES[i]));
		return true;
	}

	return false;
}

// Things that are hidden in places, that turn up only once
void ObjectItems::searchHidden(int x, int y) {
	Data::Savegame &sg = save();
	bool found = false;
	int i;

	for (i = 0; i < Data::HIDDEN_COUNT; ++i) {
		if (Data::HIDDEN_MAP[i] != sg._mapId || Data::HIDDEN_LEVEL[i] != sg._mapLevel ||
				Data::HIDDEN_X[i] != x || Data::HIDDEN_Y[i] != y)
			continue;

		byte there;
		firstAt(x, y, there);

		if ((i == KEY_ITEM_ENTRY && sg._keys == 0 && there == 0) ||
				(i == KEY_ITEM_DAY_ENTRY && sg._day != sg._skullKeysDay) ||
				(i == KEY_ITEM_LAST_ENTRY && sg._equipment[ANKH_EQUIPMENT] == 0 && there == 0)) {
			found = true;
			break;
		}

		const byte bit = 1 << (i & 7);
		if ((sg._unk02B6[i >> 3] & bit) == 0 && (i < KEY_ITEM_ENTRY || i > KEY_ITEM_LAST_ENTRY)) {
			sg._unk02B6[i >> 3] |= bit;
			found = true;
			break;
		}
	}

	if (i == KEY_ITEM_DAY_ENTRY)
		sg._skullKeysDay = sg._day;

	if (!found) {
		_log.print("nothing of note.\n");
		return;
	}

	place(pickSlot(), Data::HIDDEN_KIND[i], Data::HIDDEN_X[i], Data::HIDDEN_Y[i], Data::HIDDEN_LEVEL[i],
		Data::HIDDEN_AMOUNT[i]);
	describe(Data::HIDDEN_KIND[i]);
}

// Makes one thing that was in a chest
void ObjectItems::spawn(int kind, int amount, int power, bool &found) {
	const Data::Savegame &sg = save();

	if (kind == OBJECT_POTION || kind == OBJECT_SCROLL)
		--amount;

	if (kind == OBJECT_CHEST)
		amount = _G(gameRandom).range(1, power);
	else if (kind == OBJECT_GOLD)
		amount = _G(gameRandom).range(1, power * 3);

	const int slot = freeSlot();
	if (slot == 0)
		return;

	place(slot, kind, _x, _y, sg._mapLevel, amount);

	Data::ActorRecord a = save().getActor(slot);
	a._skiffs = 0;
	save().setActor(slot, a);

	if (!found) {
		_log.print("Found:\n");
		found = true;
	}

	describe(kind);
}

// Makes what is in a chest of a power, in two goes
void ObjectItems::spawnLoot(int power) {
	bool found = false;

	for (int i = 7; i >= 0; --i) {
		if (Data::CHEST_ODDS[i] > power || Data::CHEST_ODDS[i] > _G(gameRandom).range(1, SPAWN_ODDS))
			continue;

		const int amount = Data::CHEST_SIZE[i] == 1 ? 1 : _G(gameRandom).range(1, Data::CHEST_SIZE[i]);
		spawn(Data::CHEST_KIND[i], amount, power, found);
	}

	for (int left = power / 2; left-- >= 0;) {
		const int pick = _G(gameRandom).range(0, 0x2F);

		if (Data::CHEST_ITEM_ODDS[pick] > power || Data::CHEST_ITEM_ODDS[pick] > _G(gameRandom).range(1, SPAWN_ODDS))
			continue;

		spawn(Data::CHEST_ITEM_KIND[pick], pick, power, found);
	}

	if (!found)
		_log.print("Chest empty!\n");
}

} // namespace Ultima5
} // namespace Ultima
