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

#include "ultima/ultima5/data/combat_data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

const EnemyStats ENEMY_STATS[ENEMY_COUNT] = {
	{ 10, 15, 20, 0, 15, 10, 3, 20 },
	{ 15, 20, 10, 4, 12, 15, 9, 10 },
	{ 20, 15, 10, 8, 15, 20, 6, 15 },
	{ 25, 25, 25, 7, 30, 20, 1, 25 },
	{ 12, 12, 12, 0, 6, 8, 1, 10 },
	{ 12, 12, 18, 0, 6, 8, 1, 10 },
	{ 12, 18, 12, 0, 6, 8, 1, 10 },
	{ 12, 16, 14, 0, 6, 8, 1, 10 },
	{ 12, 12, 12, 0, 0, 5, 1, 0 },
	{ 12, 12, 12, 0, 0, 5, 1, 0 },
	{ 8, 8, 8, 0, 0, 5, 1, 0 },
	{ 8, 8, 8, 0, 0, 5, 1, 0 },
	{ 22, 30, 10, 6, 30, 99, 8, 5 },
	{ 30, 30, 30, 30, 99, 99, 1, 0 },
	{ 30, 30, 30, 30, 30, 99, 1, 0 },
	{ 30, 30, 30, 30, 99, 99, 1, 0 },
	{ 17, 20, 20, 2, 10, 30, 3, 0 },
	{ 24, 20, 8, 0, 20, 50, 2, 0 },
	{ 17, 17, 8, 2, 30, 70, 1, 0 },
	{ 20, 17, 5, 0, 8, 22, 10, 0 },
	{ 5, 20, 5, 0, 6, 10, 10, 5 },
	{ 5, 30, 5, 0, 6, 5, 16, 0 },
	{ 10, 10, 5, 0, 8, 10, 4, 5 },
	{ 1, 20, 10, 0, 12, 20, 6, 0 },
	{ 6, 6, 2, 0, 4, 10, 16, 0 },
	{ 10, 21, 10, 2, 4, 10, 13, 12 },
	{ 20, 30, 12, 3, 15, 30, 1, 20 },
	{ 20, 25, 12, 4, 20, 40, 3, 25 },
	{ 8, 10, 25, 0, 10, 20, 4, 0 },
	{ 17, 15, 12, 0, 15, 35, 4, 0 },
	{ 20, 10, 5, 15, 20, 40, 1, 0 },
	{ 1, 30, 1, 0, 4, 5, 10, 0 },
	{ 15, 13, 10, 2, 12, 10, 10, 11 },
	{ 10, 20, 5, 0, 12, 20, 8, 13 },
	{ 5, 18, 8, 1, 8, 10, 4, 0 },
	{ 20, 15, 12, 3, 15, 30, 6, 17 },
	{ 19, 12, 8, 2, 12, 20, 8, 12 },
	{ 8, 30, 20, 0, 20, 40, 4, 0 },
	{ 25, 25, 25, 5, 20, 75, 4, 0 },
	{ 30, 25, 25, 10, 30, 99, 2, 30 },
	{ 25, 25, 5, 10, 30, 80, 1, 25 },
	{ 18, 17, 9, 4, 15, 15, 4, 15 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 10, 30, 15, 4, 20, 20, 16, 5 },
	{ 17, 10, 8, 0, 15, 40, 4, 0 },
	{ 5, 17, 6, 0, 6, 5, 10, 0 },
	{ 25, 30, 30, 10, 30, 99, 1, 0 }
};

const uint16 ENEMY_FLAGS[ENEMY_COUNT] = {
	0x8000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB008, 0xBC48, 0xB008,
	0xC001, 0x4005, 0x4001, 0x0001, 0x0300, 0x0001, 0x0204, 0x0821,
	0x0011, 0x0002, 0x0280, 0x8080, 0x8040, 0x0000, 0x0090, 0x0001,
	0x0080, 0x0020, 0x0004, 0x4080, 0x0080, 0x2041, 0x8461, 0x0400,
	0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0300, 0xBB61
};

const byte ENEMY_RANGE[48] = {
	7, 3, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 15, 9, 9, 9,
	5, 7, 9, 1, 1, 1, 1, 1,
	1, 1, 2, 9, 5, 1, 9, 1,
	1, 1, 3, 5, 1, 1, 9, 9,
	1, 5, 1, 1, 1, 1, 1, 9
};

const byte ENEMY_SHOT[48] = {
	4, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 2, 4, 3, 4,
	4, 4, 3, 0, 0, 0, 0, 0,
	0, 0, 5, 4, 6, 0, 7, 0,
	0, 0, 5, 7, 0, 0, 3, 3,
	0, 2, 0, 0, 0, 0, 0, 3
};

const byte WEAPON_DAMAGE[56] = {
	0, 0, 0, 4, 0, 0, 6, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	6, 6, 8, 8, 8, 10, 10, 12,
	15, 15, 10, 1, 12, 1, 15, 20,
	20, 20, 30, 99, 15, 12, 20, 99,
	1, 30, 0, 0, 0, 0, 0, 0,
	16, 30, 99, 18, 0, 21, 0, 0
};

const byte EQUIPMENT_DEFENSE[48] = {
	1, 2, 3, 3, 2, 3, 3, 5,
	0, 1, 2, 3, 4, 5, 7, 10,
	0, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 2, 0, 0, 2, 0
};

const byte WEAPON_RANGE[56] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	3, 4, 0, 4, 0, 5, 4, 0,
	0, 2, 7, 0, 8, 0, 0, 0,
	0, 0, 2, 0, 15, 0, 15, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 15, 15, 15, 15, 15, 0
};

const byte WEAPON_KIND[56] = {
	0, 0, 0, 8, 0, 0, 8, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 7, 8, 3, 0, 0, 2, 0,
	8, 0, 0, 0, 0, 0, 0, 8,
	0, 0, 0, 0, 0, 0, 2, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	4, 3, 4, 5, 6, 3, 4, 0
};

const byte ENEMY_SPLIT[48] = {
	33, 1, 1, 3, 4, 4, 4, 4,
	4, 4, 10, 4, 12, 13, 14, 15,
	17, 16, 17, 19, 33, 21, 20, 33,
	24, 26, 35, 21, 21, 24, 30, 24,
	41, 0, 22, 36, 35, 23, 39, 39,
	40, 20, 42, 43, 44, 45, 20, 38
};

const byte MONSTER_X[16] = {
	2, 5, 8, 4, 6, 1, 5, 9,
	3, 7, 0, 4, 6, 10, 2, 8
};

const byte MONSTER_Y[16] = {
	0, 0, 0, 1, 1, 2, 2, 2,
	3, 3, 4, 4, 4, 4, 5, 5
};

const byte PARTY_X[6] = {
	5, 4, 6, 3, 5, 7
};

const byte PARTY_Y[6] = {
	7, 8, 8, 9, 9, 9
};

const int8 SHOT_PATTERN[50] = {
	-1, -2, -1, 5, 0, 0, 2, 0,
	0, -1, 2, -1, 1, -2, 1, 1,
	-2, -3, 0, -3, -3, -2, 2, -2,
	-3, -1, 3, -1, 2, 0, 2, 2,
	3, 0, 3, 2, 1, 2, -1, 2,
	-2, 0, 0, 0, 4, 1, 0, 3,
	4, 3
};

const byte SPARK[16] = {
	0, 1, 3, 0, 2, 1, 2, 1,
	2, 4, 1, 5, 0, 3, 0, 0
};

const byte PROJECTILE_PASS[32] = {
	255, 243, 195, 143, 255, 255, 255, 192,
	221, 248, 3, 223, 255, 255, 0, 0,
	255, 255, 255, 255, 255, 255, 255, 63,
	255, 255, 255, 254, 255, 255, 255, 255
};

const char *const ENEMY_NAMES[48] = {
	"Mage",
	"Bard",
	"Fighter",
	"Avatar",
	"Villager",
	"Merchant",
	"Jester",
	"Bard",
	"",
	"",
	"Child",
	"Beggar",
	"Guard",
	"Wanderer",
	"Blackthorn",
	"Lord British",
	"Sea Horse",
	"Squid",
	"Sea Serpent",
	"Shark",
	"Giant Rat",
	"Bat",
	"Giant Spider",
	"Ghost",
	"Slime",
	"Gremlin",
	"Mimic",
	"Reaper",
	"Gazer",
	"Crawler",
	"Gargoyle",
	"Insect Swarm",
	"Orc",
	"Skeleton",
	"Python",
	"Ettin",
	"Headless",
	"Wisp",
	"Daemon",
	"Dragon",
	"Sand Trap",
	"Troll",
	"",
	"",
	"Mongbat",
	"Corpser",
	"Rot Worm",
	"Shadow Lord"
};

const char *const ENEMY_GROUP_NAMES[48] = {
	"WIZARDS",
	"BARD",
	"FIGHTER",
	"x",
	"VILLAGER",
	"MERCHANT",
	"JESTER",
	"BARD",
	"PIRATES",
	"x",
	"CHILD",
	"BEGGAR",
	"GUARDS",
	"x",
	"BLACKTHORN",
	"LORD BRITISH",
	"SEA HORSES",
	"SQUIDS",
	"SEA SERPENTS",
	"SHARKS",
	"GIANT RATS",
	"BATS",
	"SPIDERS",
	"GHOSTS",
	"SLIME",
	"GREMLINS",
	"MIMICS",
	"REAPERS",
	"GAZERS",
	"x",
	"GARGOYLE",
	"INSECTS",
	"ORCS",
	"SKELETONS",
	"SNAKES",
	"ETTINS",
	"HEADLESSES",
	"WISPS",
	"DAEMONS",
	"DRAGONS",
	"SAND TRAPS",
	"TROLLS",
	"x",
	"x",
	"MONGBATS",
	"CORPSERS",
	"ROTWORMS",
	"SHADOW LORD"
};

const char *const EQUIPMENT_NAMES[48] = {
	"Leather Helm",
	"Chain Coif",
	"Iron Helm",
	"Spiked Helm",
	"Small Shield",
	"Large Shield",
	"Spiked Shield",
	"Magic Shield",
	"Jewel Shield",
	"Cloth Armour",
	"Leather Armour",
	"Ring Mail",
	"Scale Mail",
	"Chain Mail",
	"Plate Mail",
	"Mystic Armour",
	"Dagger",
	"Sling",
	"Club",
	"Flaming Oil",
	"Main Gauche",
	"Spear",
	"Throwing Axe",
	"Short Sword",
	"Mace",
	"Morning Star",
	"Bow",
	"Arrows",
	"Crossbow",
	"Quarrels",
	"Long Sword",
	"2H Hammer",
	"2H Axe",
	"2H Sword",
	"Halberd",
	"Sword of Chaos",
	"Magic Bow",
	"Silver Sword",
	"Magic Axe",
	"Glass Sword",
	"Jeweled Sword",
	"Mystic Sword",
	"Ring of Invisibility",
	"Ring of Protection",
	"Ring of Regeneration",
	"Amulet/Turning",
	"Spiked Collar",
	"Ankh"
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
