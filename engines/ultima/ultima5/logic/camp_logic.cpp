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

#include "common/file.h"
#include "ultima/ultima5/logic/camp_logic.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/logic/location_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/npc_movement.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr byte TILE_AVATAR_ON_FOOT = 0x1C;
constexpr byte TILE_DOCKED_FRIGATE = 0x24;
constexpr int REPAIR_STEPS = 5;
constexpr int REPAIR_STEP_MINUTES = 5;
constexpr int HULL_MAX = 99;
constexpr int HULL_FIRST_WORKING = 10;
constexpr int STAT_MAX = 0x1E;
constexpr int HP_PER_LEVEL = 0x1E;
constexpr int KARMA_PER_TEXT = 20;
constexpr int KARMA_TEXTS = 4;
constexpr int KARMA_TEXT_SIZE = 2000;

// Where in the file of what the apparition says each text is
static const int KARMA_OFFSETS[] = { 0, 0x84, 0x10D, 0x19A, 0x222 };
constexpr int KARMA_LAST_OFFSET = 0x29F;

static Data::Savegame &save() {
	return _G(savegame);
}

CampLogic::Block CampLogic::check(Views::MessageLog &log) {
	const Data::Savegame &sg = save();

	// A dungeon can be camped in wherever the party is
	if (DungeonLogic::isDungeon(sg._mapId))
		return BLOCK_NONE;
	const byte ground = g_engine->getMapTile(sg._mapX, sg._mapY);

	if (ground != 0 && ground < 4) {
		log.print("On land or ship!\n\n");
		return BLOCK_WATER;
	}

	if (sg._partyTile != TILE_AVATAR_ON_FOOT) {
		log.print("On foot!\n");
		return BLOCK_ON_FOOT;
	}

	return BLOCK_NONE;
}

int CampLogic::fitMembers() {
	const Data::Savegame &sg = save();
	int count = 0;

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_GOOD || sg._party[i]._status == Data::STATUS_POISONED)
			++count;
	}

	return count;
}

bool CampLogic::repairShip(Views::MessageLog &log, MonsterLogic::Result &attack) {
	Data::Savegame &sg = save();

	for (int i = 0; i < REPAIR_STEPS; ++i) {
		attack = MonsterLogic(log).turn();
		if (attack._fight)
			return false;

		if ((sg._partyTile & 0xFC) != TILE_DOCKED_FRIGATE)
			return false;

		TimeLogic::advance(sg, REPAIR_STEP_MINUTES);
	}

	Data::ActorRecord ship = sg.getActor(0);
	do {
		ship._hull += _G(gameRandom).range(1, 3);
		if (ship._hull > HULL_MAX)
			ship._hull = HULL_MAX;
	} while (ship._hull < HULL_FIRST_WORKING);
	sg.setActor(0, ship);

	log.print(Common::String::format("Hull now %d!\n\n", ship._hull));
	return true;
}

constexpr int BED_HOURS_WRAP = 0x17;
constexpr int BED_WARM_UP_TURNS = 0x10;
constexpr int BED_STEP_MINUTES = 10;
constexpr int BED_FIRST_NIGHT_HOUR = 0x14;
constexpr int BED_FIRST_DAY_HOUR = 5;
constexpr byte ASK_TO_ATTACK = 0x61;

bool CampLogic::sleepInBed(Views::MessageLog &log, int hours) {
	Data::Savegame &sg = save();

	int target = sg._hour + hours;
	if (target > BED_HOURS_WRAP)
		target -= BED_HOURS_WRAP;

	// The people go about their business first, and one may not like the look of the party
	for (int i = 0; i < BED_WARM_UP_TURNS; ++i) {
		NpcMovement(sg, _G(location), _G(gameRandom)).turn();

		if (sg._unk1018 == ASK_TO_ATTACK)
			return false;
	}

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_GOOD)
			sg._party[i]._status = Data::STATUS_SLEEP;
	}

	log.print("Zzzzzzz...\n");

	int lastHour = sg._hour;
	while (target != sg._hour) {
		TimeLogic::advance(sg, BED_STEP_MINUTES);

		if (lastHour != sg._hour && (sg._hour == BED_FIRST_NIGHT_HOUR || sg._hour == BED_FIRST_DAY_HOUR))
			LocationLogic::updateForHour();

		lastHour = sg._hour;
		PartyLogic(log).turnUpkeep();
		NpcLogic::place(sg);

		// Somebody has the bed
		if (WorldLogic::objectTileAt(sg, sg._mapX, sg._mapY) != 0) {
			target = -1;
			break;
		}
	}

	if (target == -1)
		log.print("Thrown out of bed!\n");

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_SLEEP)
			sg._party[i]._status = Data::STATUS_GOOD;
	}

	// The party gets up on the other side
	++sg._mapX;
	return true;
}

static void raise(byte &stat) {
	stat = stat + 1 < STAT_MAX ? stat + 1 : STAT_MAX;
}

bool CampLogic::levelUp(Views::MessageLog &log, int member) {
	Data::Character &c = save()._party[member];

	int level = 1;
	for (int experience = c._exp / 100; experience > 0; experience >>= 1)
		++level;

	if (c._level == level)
		return false;

	c._level = level;
	c._hp = c._maxHp = level * HP_PER_LEVEL;

	log.print(Common::String::format("\n\"Hail, %s!\nFor thy valiant deeds, I shall reward thee!\nThou art now level %d, and\n", c._name, level));

	switch (_G(gameRandom).range(1, 3)) {
	case 1:
		log.print("stronger!");
		raise(c._str);
		break;
	case 2:
		log.print("quicker!");
		raise(c._dex);
		break;
	default:
		log.print("wiser!");
		raise(c._int);
		break;
	}

	log.print("\" ");
	return true;
}

Common::String CampLogic::karmaText() {
	const int which = save()._karma / KARMA_PER_TEXT;
	const int offset = which < KARMA_TEXTS ? KARMA_OFFSETS[which] : KARMA_LAST_OFFSET;
	Common::File f;
	Common::String text;

	if (!f.open("karma.dat") || !f.seek(offset))
		return text;

	for (int i = 0; i < KARMA_TEXT_SIZE; ++i) {
		byte ch;
		if (f.read(&ch, 1) != 1 || ch == 0)
			break;

		text += (char)ch;
	}

	return text;
}

} // namespace Ultima5
} // namespace Ultima
