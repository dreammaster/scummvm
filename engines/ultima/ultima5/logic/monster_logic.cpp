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

#include "ultima/ultima5/logic/monster_logic.h"
#include "ultima/ultima5/logic/combat_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/transport_logic.h"
#include "ultima/ultima5/logic/visibility_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int ICON_TIME_STOP = 'T';
constexpr int ICON_QUICK = 'Q';
constexpr int SPAWN_TRIES = 0x80;
constexpr int SPAWN_CLEAR_DISTANCE = 6;
constexpr int PIRATE_HULL = 100;
constexpr int SHADOWLORD_CHASE = 0x14;
constexpr byte LEVEL_UNDERGROUND = 0x80;
constexpr byte LEVEL_DEEP = 0xFF;

constexpr byte TILE_HORSE = 0x12;
constexpr byte TILE_CARPET = 0x14;
constexpr byte TILE_SKIFF = 0x28;
constexpr byte TILE_PIRATE = 0x2C;
constexpr byte TILE_PIRATE_FRONT = 0x2D;
constexpr byte TILE_PIRATE_BACK = 0x2E;
constexpr byte TILE_PIRATE_SIDE = 0x2F;
constexpr byte TILE_BREATHER_A = 0x88;
constexpr byte TILE_BREATHER_B = 0xDC;
constexpr byte TILE_SHADOWLORD = 0xFC;
constexpr byte TILE_WHIRLPOOL = 0xEC;
constexpr byte TILE_SAND_TRAP = 0xE0;
constexpr byte TILE_CASTER_LORD = 0xF8;
constexpr byte TILE_MOONGATE = 0xDC;
constexpr byte TILE_DAEMON = 0xD8;
constexpr byte TILE_FRIGATE_FIRST = 0x20;

// The monsters turned up on each kind of ground, with the chance of each
static const byte LAND_MONSTERS[] = { 0xC0, 0xC8, 0x90, 0x98, 0xBC, 0xC4, 0xD0, 0xE4, 0xCC, 0xD4, 0xDC, 0xD8 };
static const byte LAND_WEIGHTS[] = { 0x3C, 0x32, 0x28, 0x1E, 0x14, 0x0F, 0x0F, 0x0A, 0x0A, 0x03, 0x02, 0x01 };
static const byte UNDER_MONSTERS[] = { 0x94, 0x90, 0x98, 0xF0, 0xF4, 0xD8, 0xDC, 0x00 };
static const byte UNDER_WEIGHTS[] = { 0x40, 0x38, 0x38, 0x20, 0x20, 0x08, 0x08, 0x00 };
static const byte SEA_MONSTERS[] = { 0x8C, 0x84, 0x88, 0x80, 0x2C, 0x00 };
static const byte SEA_WEIGHTS[] = { 0x48, 0x48, 0x28, 0x26, 0x22, 0x00 };
static const byte UNDER_SEA_MONSTERS[] = { 0x84, 0x88 };
static const byte UNDER_SEA_WEIGHTS[] = { 0x80, 0x80 };

// How many turns a pirate ship waits for each way it can face with the wind
static const uint16 PIRATE_SPEED[4][4] = {
	{ 2, 3, 4, 4 }, { 4, 4, 2, 3 }, { 3, 2, 4, 4 }, { 4, 4, 3, 2 }
};

// The places around the party from which the Shadowlord goes after it
static const byte SHADOWLORD_RANGE[0x3C] = {
	0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

byte MonsterLogic::_lastX = 0;
byte MonsterLogic::_lastY = 0;
bool MonsterLogic::_quickToggle = false;
bool MonsterLogic::_horseToggle = false;

static int rnd(int low, int high) {
	return _G(gameRandom).range(low, high);
}

static Data::Savegame &save() {
	return _G(savegame);
}

// Gives the tiles of the map of Britannia to the working out of what can be seen
class WorldTiles : public TileSource {
public:
	byte tileAt(int x, int y) const override {
		return g_engine->getMapTile(x, y);
	}
};

bool MonsterLogic::isMonster(int tile) {
	if (tile >= 0x2C && tile <= 0x2F)
		return true;

	if (tile < 0x80)
		return false;

	return !((tile >= 0xB4 && tile <= 0xB7) || (tile >= 0xE8 && tile <= 0xEB));
}

int MonsterLogic::tileAt(int x, int y) {
	return g_engine->getMapTile(x, y);
}

// The tile of the last object at a place on this level, which can be none at all
int MonsterLogic::objectAt(int x, int y) {
	const Data::Savegame &sg = save();

	for (int i = Data::ACTOR_COUNT - 1; i >= 0; --i) {
		const Data::ActorRecord a = sg.getActor(i);
		if (a._x == x && a._y == y && a._z == sg._mapLevel)
			return a._tile;
	}

	return 0;
}

bool MonsterLogic::canGo(int object, int x, int y) {
	const Data::ActorRecord a = save().getActor(object);

	if (!WorldLogic::isWalkable(a._tile, tileAt(x, y)))
		return false;

	return objectAt(x, y) == 0;
}

// How likely it is that a monster turns up, from the ground and the hour
int MonsterLogic::spawnChance() {
	const Data::Savegame &sg = save();

	if (sg._mapLevel > 0x7F)
		return 3;

	const int ground = tileAt(sg._mapX, sg._mapY);
	int chance;

	if (ground >= 0x20 && ground <= 0x26)
		chance = 0;
	else if (ground == 4 || (ground >= 9 && ground <= 0xF))
		chance = 2;
	else
		chance = 1;

	if (sg._hour >= 0x20 || sg._hour < 5)
		return chance + 3;

	return chance;
}

int MonsterLogic::pickWeighted(const byte *weights) {
	int roll = rnd(0, 0xFF);
	int i;

	for (i = 0; weights[i] <= roll; ++i)
		roll -= weights[i];

	return i;
}

// Which monster, if any, turns up on a kind of ground
byte MonsterLogic::monsterFor(int ground) {
	const Data::Savegame &sg = save();

	if (ground < 4 || (ground >= 0x60 && ground <= 0x6F) || (ground >= 0xD4 && ground <= 0xD7) || (ground >= 0xE4 && ground <= 0xE7)) {
		if (rnd(0, 0x40) < 0x10) {
			if (sg._mapLevel < LEVEL_UNDERGROUND) {
				if (ground == 1 && rnd(0, 7) == 7)
					return TILE_WHIRLPOOL;

				return SEA_MONSTERS[pickWeighted(SEA_WEIGHTS)];
			}

			return UNDER_SEA_MONSTERS[pickWeighted(UNDER_SEA_WEIGHTS)];
		}
	} else if (ground == 7) {
		if (rnd(0, 3) == 0)
			return TILE_SAND_TRAP;
	} else {
		if (ground == 4 && sg._mapLevel == LEVEL_DEEP)
			return TILE_CASTER_LORD;

		if (ground == 0xC || ground == 0xD)
			return 0;

		if (ground < 0x10 || (ground & 0xFC) == 0x30) {
			if (sg._mapLevel < LEVEL_UNDERGROUND)
				return LAND_MONSTERS[pickWeighted(LAND_WEIGHTS)];

			return UNDER_MONSTERS[pickWeighted(UNDER_WEIGHTS)];
		}
	}

	return 0;
}

// Picks a place in the part of the map that's loaded, not close to the party
void MonsterLogic::pickSpot(int &x, int &y) {
	const Data::Savegame &sg = save();

	do {
		x = (byte)(rnd(0, 0x1F) + sg._chunkX);
		y = (byte)(rnd(0, 0x1F) + sg._chunkY);
	} while (abs(x - sg._mapX) <= SPAWN_CLEAR_DISTANCE || abs(y - sg._mapY) <= SPAWN_CLEAR_DISTANCE ||
			abs(x - sg._mapX) >= 0xFA || abs(y - sg._mapY) >= 0xFA);
}

void MonsterLogic::spawn() {
	Data::Savegame &sg = save();
	int tries;
	int x = 0;
	int y = 0;
	byte monster = 0;

	for (tries = 0; tries < SPAWN_TRIES; ++tries) {
		pickSpot(x, y);

		monster = monsterFor(tileAt(x, y));
		if (monster != 0) {
			if (monster != TILE_PIRATE || (tileAt(x, y) & 0xF0) != 0x60)
				break;
		}
	}

	if (tries != SPAWN_TRIES) {
		const int object = NpcLogic::findFreeObject(sg);

		if (object != 0) {
			// What was there before is left as it was, apart from where it is
			Data::ActorRecord a = sg.getActor(object);
			a._tile = a._animTile = monster;
			a._x = x;
			a._y = y;
			a._z = sg._mapLevel;
			a._hull = monster == TILE_PIRATE ? PIRATE_HULL : 0;

			sg.setActor(object, a);
		}
	}
}

void MonsterLogic::remove(int object) {
	Data::ActorRecord a = save().getActor(object);
	a._tile = a._animTile = a._x = a._y = a._z = a._hull = 0;
	save().setActor(object, a);
}

// A monster takes a step, unless the ground slows it down
void MonsterLogic::step(int object, int dx, int dy) {
	Data::Savegame &sg = save();
	Data::ActorRecord a = sg.getActor(object);
	const int tile = a._tile;
	const int x = a._x + dx;
	const int y = a._y + dy;
	const int ground = tileAt(x, y);

	if ((tile & 0xFC) == TILE_PIRATE) {
		int facing = 0;

		if (dx == 0 && dy == -1)
			facing = 0;
		else if (dx == 1 && dy == 0)
			facing = 1;
		else if (dx == 0 && dy == 1)
			facing = 2;
		else if (dx == -1 && dy == 0)
			facing = 3;

		a._tile = a._animTile = facing + TILE_PIRATE;
	} else if (tile != TILE_BREATHER_B && tile != 0x94 && tile != TILE_DAEMON && tile != 0xF0) {
		switch (ground) {
		case 4:
		case 6:
		case 7:
		case 8:
		case 0x1E:
		case 0x1F:
			if (rnd(0, 1) == 0)
				return;
			break;

		case 9:
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
			if (rnd(0, 2) != 2)
				return;
			break;
		}
	}

	_lastX = a._x;
	_lastY = a._y;

	a._x = x;
	a._y = y;

	if (tileAt(x, y) == TILE_MOONGATE)
		a._tile = a._animTile = 0;

	sg.setActor(object, a);
}

// A monster goes a random way
void MonsterLogic::wander(int object) {
	const Data::ActorRecord a = save().getActor(object);
	const int x = a._x;
	const int y = a._y;

	for (int tries = 0; tries < 3; ++tries) {
		switch (rnd(0, 3)) {
		case 0:
			if (canGo(object, x, y - 1))
				step(object, 0, -1);
			return;

		case 1:
			if (canGo(object, x + 1, y))
				step(object, 1, 0);
			return;

		case 2:
			if (canGo(object, x, y + 1))
				step(object, 0, 1);
			return;

		default:
			if (canGo(object, x - 1, y))
				step(object, -1, 0);
			return;
		}
	}
}

// A monster goes after the party
void MonsterLogic::chase(int object) {
	const Data::Savegame &sg = save();
	const Data::ActorRecord a = sg.getActor(object);
	const int x = a._x;
	const int y = a._y;

	int dx = (byte)(a._x - sg._mapX);
	if (dx > 0x7F)
		dx -= 0x100;

	int dy = (byte)(a._y - sg._mapY);
	if (dy > 0x7F)
		dy -= 0x100;

	const int stepX = dx == 0 ? 0 : (dx > 0 ? -1 : 1);
	const int stepY = dy == 0 ? 0 : (dy > 0 ? -1 : 1);

	// Not back to where it just was
	const bool xFirst = rnd(0, 1) == 1;

	for (int pass = 0; pass < 2; ++pass) {
		const bool doX = (pass == 0) == xFirst;

		if (doX) {
			if (stepX != 0 && canGo(object, x + stepX, y) && !(x + stepX == _lastX && y == _lastY)) {
				step(object, stepX, 0);
				return;
			}
		} else {
			if (stepY != 0 && canGo(object, x, y + stepY) && !(x == _lastX && y + stepY == _lastY)) {
				step(object, 0, stepY);
				return;
			}
		}
	}

	wander(object);
}

// One monster's move
void MonsterLogic::move(int object) {
	Data::Savegame &sg = save();
	Data::ActorRecord a = sg.getActor(object);
	const int tile = a._tile;

	if ((tile & 0xFC) == TILE_WHIRLPOOL) {
		a._hull ^= 1;
		sg.setActor(object, a);

		if (a._hull == 0)
			return;

		if (rnd(0, 1) == 0) {
			wander(object);
			return;
		}
	} else if (tile == TILE_SHADOWLORD) {
		int dx = abs(a._x - sg._mapX);
		if (dx > 0x7F)
			dx = 0x100 - dx;

		int dy = abs(a._y - sg._mapY);
		if (dy > 0x7F)
			dy = 0x100 - dy;

		const int near = (dx < 6 && dy < 6) ? (dx + dy * 11 < 0x3C ? SHADOWLORD_RANGE[dx + dy * 11] : 0) : 0;
		if (near != 0 && a._hull++ < SHADOWLORD_CHASE) {
			sg.setActor(object, a);
			chase(object);
			return;
		}

		sg.setActor(object, a);
	} else if ((tile & 0xFC) == TILE_PIRATE) {
		if (sg._windDirection == 0)
			return;

		const uint speed = PIRATE_SPEED[tile - TILE_PIRATE][sg._windDirection - 1];
		if (speed != 4 && speed < (uint)++a._skiffs) {
			a._skiffs = 0;
			sg.setActor(object, a);
			return;
		}

		sg.setActor(object, a);
	}

	chase(object);
}

// Whether a shot from a monster gets to the party, with the objects in the way
bool MonsterLogic::shotReaches(int object, int kind) {
	const Data::Savegame &sg = save();
	const Data::ActorRecord a = sg.getActor(object);

	// What the party sees of the ground, with objects over it
	WorldTiles source;
	byte view[VIEW_TILES][VIEW_TILES];
	g_engine->_visibility.computeView(source, sg._chunkX, sg._chunkY, sg._mapX, sg._mapY, sg._light, view);

	byte tiles[COMBAT_MAP_BYTES];
	memset(tiles, 0, sizeof(tiles));
	for (int y = 0; y < VIEW_TILES; ++y) {
		for (int x = 0; x < VIEW_TILES; ++x)
			tiles[y * COMBAT_MAP_STRIDE + x] = view[y][x];
	}

	for (int i = 0; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord o = sg.getActor(i);
		const byte vx = o._x - (sg._mapX - 5);
		const byte vy = o._y - (sg._mapY - 5);

		if (o._tile != 0 && o._z == sg._mapLevel && vx < VIEW_TILES && vy < VIEW_TILES)
			tiles[vy * COMBAT_MAP_STRIDE + vx] = 0;
	}

	const byte fromX = a._x - sg._mapX + 5;
	const byte fromY = a._y - sg._mapY + 5;
	int lastX = 0;
	int lastY = 0;

	return CombatLogic::traceProjectile(fromX, fromY, 5, 5, kind, tiles, lastX, lastY);
}

void MonsterLogic::pirateShot(int object, int dx, int dy) {
	Data::Savegame &sg = save();
	Data::ActorRecord a = sg.getActor(object);

	if (dx == 0) {
		if (a._animTile == TILE_PIRATE || a._animTile == TILE_PIRATE_BACK)
			a._animTile = (rnd(0, 3) & 2) + TILE_PIRATE_FRONT;
	}

	if (dy == 0) {
		if (a._animTile == TILE_PIRATE_FRONT || a._animTile == TILE_PIRATE_SIDE)
			a._animTile = (rnd(0, 3) & 2) + TILE_PIRATE;
	}

	sg.setActor(object, a);

	if (shotReaches(object, 1))
		TransportLogic(_log).damage();
}

// A monster that has come up to the party
void MonsterLogic::attacked(int object, Result &result) {
	Data::Savegame &sg = save();
	Data::ActorRecord a = sg.getActor(object);

	if ((a._tile & 0xFC) == TILE_WHIRLPOOL) {
		if (sg._partyTile == TILE_AVATAR) {
			TransportLogic(_log).damage();
		} else {
			a._tile = a._animTile = 0;
			sg.setActor(object, a);
			_log.print("\nWHIRLPOOL!\n");
		}
	} else if ((a._tile & 0xFC) != TILE_SAND_TRAP) {
		_log.print("\nAttacked!\n");

		const int ground = tileAt(sg._mapX, sg._mapY);
		if (ground < 4 && ((sg._partyTile & 0xFE) == TILE_CARPET || (sg._partyTile & 0xFC) == TILE_SKIFF)) {
			TransportLogic(_log).damage();
		} else {
			if (!result._fight) {
				result._fight = true;
				result._actor = object;
			}
		}
	} else {
		TransportLogic(_log).damage();
	}
}

// What a monster does to the party if it's near enough. Gives 1 if it did
int MonsterLogic::attackCheck(int object, Result &result) {
	const Data::Savegame &sg = save();
	const Data::ActorRecord a = sg.getActor(object);
	const int tile = a._tile;

	int dx = abs(a._x - sg._mapX);
	if (dx > 0x7F)
		dx = 0x100 - dx;

	int dy = abs(a._y - sg._mapY);
	if (dy > 0x7F)
		dy = 0x100 - dy;

	if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
		attacked(object, result);
		return 1;
	}

	if (tile == TILE_BREATHER_A || tile == TILE_BREATHER_B) {
		if (dx <= 3 && dy <= 3 && rnd(0, 7) == 0) {
			if (shotReaches(object, 3))
				TransportLogic(_log).damage();

			return 1;
		}
	} else if ((tile & 0xFC) == TILE_PIRATE && ((dx == 0 && dy < 4) || (dy == 0 && dx < 4))) {
		_log.print("* BOOOM! *\n\n");
		g_engine->sound().sweep(1000, 200, 5, 300);
		pirateShot(object, dx, dy);
		return 1;
	}

	return 0;
}

MonsterLogic::Result MonsterLogic::turn() {
	Data::Savegame &sg = save();
	Result result;
	if (sg._icon == ICON_TIME_STOP)
		return result;

	if (sg._icon == ICON_QUICK) {
		_quickToggle = !_quickToggle;
		if (_quickToggle)
			return result;
	}

	if ((sg._partyTile & 0xFE) == TILE_HORSE || (sg._partyTile & 0xFE) == TILE_CARPET) {
		_horseToggle = !_horseToggle;
		if (_horseToggle)
			return result;
	}

	if (spawnChance() > rnd(1, 0x1E))
		spawn();

	for (int i = Data::ACTOR_COUNT - 1; i > 0; --i) {
		if (isMonster(sg.getActor(i)._tile)) {
			result._attacks += attackCheck(i, result);

			if (result._attacks == 0)
				move(i);
		}
	}

	// Those that have been left behind are forgotten
	for (int i = Data::ACTOR_COUNT - 1; i > 0; --i) {
		const Data::ActorRecord a = sg.getActor(i);

		if (isMonster(a._tile) && ((byte)(a._x - sg._chunkX) > 0x1F || (byte)(a._y - sg._chunkY) > 0x1F))
			remove(i);
	}

	return result;
}

} // namespace Ultima5
} // namespace Ultima
