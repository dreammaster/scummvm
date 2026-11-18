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
#include "common/file.h"
#include "common/util.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/data/location_tables.h"

namespace Ultima {
namespace Ultima5 {

constexpr int NPC_FILE_BLOCK_SIZE = 0x240;
constexpr int NPC_FILE_TYPES_OFFSET = 0x200;
constexpr int NPC_FILE_DIALOGS_OFFSET = 0x220;
constexpr int MOVEMENT_RECORD_SIZE = 32;
constexpr int LAST_PERSON_OBJECT = 0x17;
constexpr byte OBJECT_SLEEPING = 0x1E;
constexpr byte OBJECT_STAYS_FLAG = 0xFF;
constexpr byte NPC_KIND_CHEST = 1;
constexpr uint16 NO_TALK = 0xFFFF;
constexpr byte OBJECT_CROWN = 0xB5;
constexpr int VIEW_LAST = 10;

static const char *const NPC_FILES[] = { "towne.npc", "dwelling.npc", "castle.npc", "keep.npc" };

// The people of each location that are marked in the objects with a flag
static const uint32 STAYING_NPCS[Data::LOCATION_FILE_LOCATIONS * 4] = {
	0, 0, 0, 0x28000, 2, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0x3F8, 0x1E0, 0, 0, 0
};

int NpcLogic::scheduleSlot(const Data::NpcSchedule &schedule, byte hour) {
	byte since[Data::NPC_TIME_SLOTS];
	for (int i = 0; i < Data::NPC_TIME_SLOTS; ++i)
		since[i] = hour - schedule._time[i];

	int slot = 0;
	if (since[1] < since[0]) {
		since[0] = since[1];
		slot = 1;
	}

	if (since[2] < since[0]) {
		since[0] = since[2];
		slot = 2;
	}

	if (since[3] < since[0])
		slot = 1;

	return slot;
}

bool NpcLogic::loadFile(Data::Savegame &sg) {
	const int map = sg._mapId - 1;
	if (map < 0 || map >= Data::LOCATION_FILE_LOCATIONS * 4)
		return false;

	Common::File f;
	const int block = (map % Data::LOCATION_FILE_LOCATIONS) * NPC_FILE_BLOCK_SIZE;
	byte dialogs[Data::NPC_COUNT];

	if (!f.open(NPC_FILES[map / Data::LOCATION_FILE_LOCATIONS]) || !f.seek(block) ||
			f.read(sg._npcSchedules, sizeof(sg._npcSchedules)) != sizeof(sg._npcSchedules) ||
			f.read(sg._npcTypes, sizeof(sg._npcTypes)) != sizeof(sg._npcTypes) ||
			f.read(dialogs, sizeof(dialogs)) != sizeof(dialogs))
		return false;

	for (int i = 0; i < Data::NPC_COUNT; ++i) {
		Data::NpcState state = sg.getNpcState(i);
		state._dialog = dialogs[i];
		sg.setNpcState(i, state);
	}

	return true;
}

void NpcLogic::initialize(Data::Savegame &sg) {
	for (int i = 1; i < Data::NPC_COUNT; ++i) {
		Data::NpcState state = sg.getNpcState(i);

		if (sg._npcTypes[i] == 0) {
			state._present = 0;
		} else {
			const Data::NpcSchedule schedule = sg.getSchedule(i);
			const int slot = scheduleSlot(schedule, sg._hour);

			state._present = 1;
			state._x = schedule._x[slot];
			state._y = schedule._y[slot];
			state._z = schedule._z[slot];
			state._tile = sg._npcTypes[i];
			state._slot = slot;
			state._object = 0;

			WRITE_LE_UINT16(&sg._npcTalk[i * 2], NO_TALK);
			sg._npcMovement[i * MOVEMENT_RECORD_SIZE] = 0;
			sg._unk101C[i] = 0;
		}

		sg.setNpcState(i, state);
	}
}

// Looks through the objects for one in a range of tiles, if need be that is also
// out of view of the party
static int findObject(const Data::Savegame &sg, int first, int last, bool outOfView) {
	for (int i = 1; i <= LAST_PERSON_OBJECT; ++i) {
		const Data::ActorRecord actor = sg.getActor(i);

		if (actor._tile >= first && actor._tile <= last && actor._tile != OBJECT_CROWN) {
			if (!outOfView)
				return i;

			if ((byte)(actor._x - sg._mapX + 5) > VIEW_LAST || (byte)(actor._y - sg._mapY + 5) > VIEW_LAST)
				return i;
		}
	}

	return 0;
}

int NpcLogic::findFreeObject(const Data::Savegame &sg) {
	// A spare object if there is one, otherwise the first thing out of sight
	// that can be done without
	static const int RANGES[][3] = {
		{ 0, 0, 0 }, { 1, 0xF, 1 }, { 0x80, 0xFF, 1 }, { 0x10, 0x11, 1 }, { 0x30, 0x7F, 1 },
		{ 1, 0xF, 0 }, { 0x80, 0xFF, 0 }, { 0x10, 0x11, 0 }, { 0x30, 0x7F, 0 }, { 0, 0xFF, 0 }
	};

	for (int i = 0; i < ARRAYSIZE(RANGES); ++i) {
		const int object = findObject(sg, RANGES[i][0], RANGES[i][1], RANGES[i][2] != 0);
		if (object != 0)
			return object;
	}

	return 0;
}

void NpcLogic::placeNpc(Data::Savegame &sg, int npc, int x, int y, int z) {
	const byte type = sg._npcTypes[npc];
	if (type == 0)
		return;

	Data::NpcState state = sg.getNpcState(npc);

	// A person who's on another level isn't on the map
	if (z != sg._mapLevel && state._object != 0) {
		Data::ActorRecord actor = sg.getActor(state._object);
		actor._tile = 0;
		sg.setActor(state._object, actor);
		state._object = 0;
	}

	if (z == sg._mapLevel && state._object == 0) {
		const int object = findFreeObject(sg);

		if (object != 0) {
			Data::ActorRecord actor;
			actor._tile = actor._animTile = type;
			actor._x = x;
			actor._y = y;
			actor._z = z;

			if (type == NPC_KIND_CHEST)
				actor._hull = OBJECT_SLEEPING;
			else if ((STAYING_NPCS[sg._mapId - 1] & (1U << npc)) != 0)
				actor._hull = OBJECT_STAYS_FLAG;

			sg.setActor(object, actor);
			state._object = object;
		}
	}

	if (z == sg._mapLevel && state._object != 0) {
		Data::ActorRecord actor = sg.getActor(state._object);
		actor._x = x;
		actor._y = y;
		actor._z = z;
		sg.setActor(state._object, actor);
	}

	state._x = x;
	state._y = y;
	state._z = z;
	state._present = 1;
	sg.setNpcState(npc, state);
}

void NpcLogic::place(Data::Savegame &sg) {
	for (int i = 1; i < Data::NPC_COUNT; ++i) {
		sg.setActor(i, Data::ActorRecord());

		Data::NpcState state = sg.getNpcState(i);
		state._object = 0;
		sg.setNpcState(i, state);
	}

	for (int i = 1; i < Data::NPC_COUNT; ++i) {
		if (sg._npcTypes[i] == 0)
			continue;

		const Data::NpcSchedule schedule = sg.getSchedule(i);
		const int slot = scheduleSlot(schedule, sg._hour);
		placeNpc(sg, i, schedule._x[slot], schedule._y[slot], schedule._z[slot]);

		Data::NpcState state = sg.getNpcState(i);
		state._slot = slot;
		sg.setNpcState(i, state);
		WRITE_LE_UINT16(&sg._npcTalk[i * 2], NO_TALK);
	}
}

void NpcLogic::despawn(Data::Savegame &sg, int npc) {
	Data::NpcState state = sg.getNpcState(npc);

	if (state._object != 0)
		sg.setActor(state._object, Data::ActorRecord());

	state._present = 0;
	state._x = state._y = state._z = 0;
	state._dialog = 0;
	sg.setNpcState(npc, state);

	Data::NpcSchedule schedule = sg.getSchedule(npc);
	memset(schedule._type, 0, sizeof(schedule._type));
	sg.setSchedule(npc, schedule);
	sg._npcTypes[npc] = 0;
}

bool NpcLogic::arrive(Data::Savegame &sg) {
	sg._drunkTurns = 0;
	sg._updateMap = 1;

	if (!loadFile(sg))
		return false;

	initialize(sg);
	place(sg);

	for (int i = 0; i < Data::NPC_COUNT; ++i) {
		if (sg.isNpcKilled(i))
			despawn(sg, i);
	}

	return true;
}

} // namespace Ultima5
} // namespace Ultima
