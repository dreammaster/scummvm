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

#ifndef ULTIMA5_DATA_SAVEGAME_H
#define ULTIMA5_DATA_SAVEGAME_H

#include "common/serializer.h"
#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int PARTY_MAX = 16;
constexpr int NAME_SIZE = 9;
constexpr int SAVEGAME_SIZE = 0x1060;

constexpr byte GENDER_MALE = 0x0B;
constexpr byte GENDER_FEMALE = 0x0C;

constexpr char STATUS_GOOD = 'G';
constexpr char STATUS_POISONED = 'P';
constexpr char STATUS_DEAD = 'D';
constexpr char STATUS_SLEEP = 'S';
constexpr char STATUS_CHARMED = 'C';

// Set as the active character when there isn't one
constexpr byte NO_ACTIVE_CHARACTER = 0xFF;

// Maps above ground have levels from 0, and the levels below are negative
constexpr byte MAP_LEVEL_UNDERGROUND = 0x80;

/**
 * A character of the party, or one available to join it
 */
struct Character {
	char _name[NAME_SIZE] = {};
	byte _gender = 0;
	byte _class = 0;
	byte _status = 0;
	byte _str = 0;
	byte _dex = 0;
	byte _int = 0;
	byte _mag = 0;
	int16 _hp = 0;
	int16 _maxHp = 0;
	int16 _exp = 0;
	byte _level = 0;
	byte _unk17 = 0;
	byte _unk18 = 0;
	byte _equipment[6] = {};
	byte _mapId = 0;

	void synchronize(Common::Serializer &s);
};

constexpr int ACTOR_COUNT = 32;
constexpr int ACTOR_SIZE = 8;

/**
 * An object on the map, such as the party, a monster or a ship
 */
struct ActorRecord {
	byte _tile = 0;
	byte _animTile = 0;
	byte _x = 0;
	byte _y = 0;
	byte _z = 0;
	byte _hull = 0;
	byte _unk6 = 0;
	byte _skiffs = 0;
};

constexpr int NPC_COUNT = 32;
constexpr byte NPC_KIND_BOX = 0x0E;
constexpr byte NPC_KIND_FIRST_PERSON = 0x40;
constexpr byte NPC_KIND_GUARD = 0x70;
constexpr byte NPC_KIND_STAY_DEAD = 0x80;
constexpr byte NPC_KIND_SHARD = 0xB4;
constexpr int NPC_SCHEDULE_SIZE = 16;
constexpr int NPC_STATE_SIZE = 16;
constexpr int NPC_SCHEDULE_SLOTS = 3;
constexpr int NPC_TIME_SLOTS = 4;

/**
 * Where a person of a location is at different times of the day, and what
 * they're doing there
 */
struct NpcSchedule {
	byte _type[NPC_SCHEDULE_SLOTS] = {};
	byte _x[NPC_SCHEDULE_SLOTS] = {};
	byte _y[NPC_SCHEDULE_SLOTS] = {};
	byte _z[NPC_SCHEDULE_SLOTS] = {};
	byte _time[NPC_TIME_SLOTS] = {};
};

/**
 * What the game keeps track of for a person of a location
 */
struct NpcState {
	uint16 _present = 0;
	int16 _x = 0;
	int16 _y = 0;
	uint16 _z = 0;
	uint16 _tile = 0;
	uint16 _dialog = 0;
	uint16 _object = 0;
	uint16 _slot = 0;
};

/**
 * The state of the game, kept in the same layout as the original saved.gam
 * and init.gam files
 */
struct Savegame {
	uint16 _unk0000 = 0;
	Character _party[PARTY_MAX];
	int16 _food = 0; // Food
	int16 _gold = 0; // Gold
	byte _keys = 0; // Keys
	byte _gems = 0; // Gems
	byte _torches = 0; // Torches
	byte _grapple = 0; // Grappling hook
	byte _carpets = 0; // Magic carpets
	byte _skullKeys = 0; // Skull keys
	byte _skullKeysDay = 0; // Day the skull keys were last taken
	byte _amulet = 0; // Amulet of Lord British
	byte _crown = 0; // Crown of Lord British
	byte _sceptre = 0; // Sceptre of Lord British
	byte _shards[0x4] = {}; // Shards of the Gem of Immortality
	byte _spyGlass = 0; // Spy glasses
	byte _capePlans = 0; // HMS Cape plans
	byte _sextant = 0; // Sextants
	byte _pocketWatch = 0; // Pocket watch
	byte _blackBadge = 0; // Black badge
	byte _sandalwoodBox = 0; // Sandalwood box
	byte _equipment[0x30] = {}; // How many of each weapon, armour and other item the party has
	byte _mixtures[0x30] = {}; // Mixed spells
	byte _scrolls[0x8] = {}; // Scrolls
	byte _potions[0x8] = {}; // Potions
	byte _moonstoneX[0x8] = {}; // Moonstone x coordinates, when buried
	byte _moonstoneY[0x8] = {}; // Moonstone y coordinates, when buried
	byte _moonstoneState[0x8] = {}; // 0 if buried, 255 if in the inventory
	byte _moonstoneZ[0x8] = {}; // 0 for Britannia, 255 for the underworld, when buried
	byte _reagents[0x8] = {}; // Reagents
	byte _harvestDays[0x3] = {}; // Day each of the reagent patches was last harvested
	byte _partySize = 0; // Party members
	byte _unk02B6[0x18] = {};
	uint16 _year = 0; // Year
	int16 _unk02D0 = 0;
	int16 _unk02D2 = 0;
	byte _icon = 0; // Icon
	byte _activeChar = 0; // Active character
	byte _partyTile = 0; // Party tile
	byte _month = 0; // Month
	byte _day = 0; // Day
	byte _hour = 0; // Hour
	byte _hourCopy = 0;
	byte _minute = 0; // Minute
	byte _unk02DC = 0;
	byte _windTicks = 0;
	byte _unk02DE = 0;
	byte _trammelPhase = 0; // Phase of Trammel
	byte _feluccaPhase = 0; // Phase of Felucca
	byte _moongateHeight = 0; // Height of the moongate in pixels
	byte _karma = 0; // Karma
	byte _unk02E3 = 0;
	byte _unk02E4 = 0;
	byte _turnCount = 0; // Turn count
	byte _unk02E6 = 0;
	byte _unk02E7 = 0;
	byte _protection = 0; // Turns left of the protection spell
	byte _unk02E9 = 0;
	byte _unk02EA = 0;
	byte _animate = 0; // Animation flag
	byte _windDirection = 0; // Wind direction
	byte _mapId = 0; // Map the party is in
	byte _combatMapId = 0; // Map to return to after combat
	byte _mapLevel = 0; // Level of the map
	byte _mapX = 0; // Party x
	byte _mapY = 0; // Party y
	byte _crosshairShown = 0;
	byte _crosshairX = 0;
	byte _crosshairY = 0;
	byte _chunkX = 0;
	byte _chunkY = 0;
	byte _attackerWeapon = 0;
	byte _unk02F8 = 0;
	byte _focusShown = 0;
	byte _combatExitDirection = 0;
	byte _combatFlags = 0;
	byte _unk02FC = 0;
	byte _battleWon = 0;
	byte _updateMap = 0;
	byte _light = 0; // Light intensity
	byte _unk0300 = 0;
	byte _unk0301 = 0;
	byte _unk0302[0x20] = {};
	byte _unk0322[0x3] = {};
	byte _unk0325 = 0;
	uint16 _shrineActive = 0; // Shrine quests in progress
	uint16 _shrineDone = 0; // Shrine quests completed
	byte _unk032A[0x8] = {};
	byte _unk0332[0x8] = {};
	byte _unk033A[0xE] = {};
	byte _spellCounts[0x20] = {};
	byte _unk0368[0x20] = {};
	byte _unk0388[0x20] = {};
	byte _unk03A8 = 0;
	byte _doorOpen = 0; // Open door
	byte _doorX = 0;
	byte _doorY = 0;
	byte _doorTurns = 0;
	byte _shipX = 0;
	byte _shipY = 0;
	byte _sailHeading = 0;
	byte _sailBreak = 0;
	byte _drunkTurns = 0;
	byte _unk03B2 = 0;
	byte _unk03B3 = 0;
	byte _mapData[0x200] = {};
	byte _npcKilled[0x80] = {}; // NPC killed flags, 32 bits per settlement
	byte _npcMet[0x80] = {}; // NPC met flags, 32 bits per settlement
	byte _objects[0x100] = {}; // The 32 objects on the map
	uint16 _unk07B4 = 0;
	uint16 _unk07B6 = 0;
	byte _npcSchedules[0x200] = {};
	byte _npcStates[0x200] = {};
	byte _npcMovement[0x400] = {};
	byte _npcTalk[0x40] = {};
	byte _npcTypes[0x20] = {};
	byte _unk1018 = 0;
	byte _unk1019 = 0;
	uint16 _unk101A = 0;
	byte _unk101C[0x40] = {};
	byte _unk105C = 0;
	byte _unk105D = 0;
	byte _unk105E = 0;
	byte _shipOrder = 0;

	/**
	 * Reads or writes the savegame
	 */
	void synchronize(Common::Serializer &s);

	/**
	 * Loads a savegame file from the game folder, such as init.gam
	 * @returns		False if it couldn't be loaded
	 */
	bool load(const char *filename);

	/**
	 * Returns true if there's a game that has a first character with a name
	 */
	bool hasActiveGame() const {
		return _party[0]._name[0] != '\0';
	}

	// The objects of Britannia and of the underworld, kept while the party is
	// somewhere else. They're stored with the savegame, but after the layout of the game
	byte _worldObjects[2][ACTOR_COUNT * ACTOR_SIZE] = {};

	/**
	 * Reads or writes the objects of the two worlds
	 */
	void synchronizeWorldObjects(Common::Serializer &s);

	/**
	 * Loads the objects of a new game from a file in the game folder, such as init.ool
	 * @returns		False if it couldn't be loaded
	 */
	bool loadWorldObjects(const char *filename);

	/**
	 * Puts the objects the party is among away, for it to come back to
	 */
	void storeWorldObjects(bool underworld) {
		memcpy(_worldObjects[underworld], _objects, sizeof(_objects));
	}

	/**
	 * Brings back the objects of a world, or none at all
	 */
	void restoreWorldObjects(bool underworld) {
		memcpy(_objects, _worldObjects[underworld], sizeof(_objects));
	}

	void clearObjects() {
		memset(_objects, 0, sizeof(_objects));
	}

	/**
	 * Returns one of the objects on the map. The first is the party
	 */
	ActorRecord getActor(int idx) const;

	void setActor(int idx, const ActorRecord &actor);

	NpcSchedule getSchedule(int npc) const;
	void setSchedule(int npc, const NpcSchedule &schedule);
	NpcState getNpcState(int npc) const;
	void setNpcState(int npc, const NpcState &state);

	/**
	 * Returns true if a person of the location the party is in has been killed
	 * for good
	 */
	bool isNpcKilled(int npc) const;

	/**
	 * Marks a person of the location the party is in as killed for good, if
	 * they are of a kind that stays dead
	 */
	void setNpcKilled(int npc);

	/**
	 * Returns true if the party has been told the name of a person of the
	 * location it is in
	 */
	bool isNpcMet(int npc) const;
	void setNpcMet(int npc);
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
