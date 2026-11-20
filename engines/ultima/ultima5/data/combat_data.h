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

#ifndef ULTIMA5_DATA_COMBAT_DATA_H
#define ULTIMA5_DATA_COMBAT_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int ENEMY_COUNT = 0x30;

/**
 * What the game knows of a kind of enemy: its strengths, size of the groups it comes in and how likely it leaves treasure
 */
struct EnemyStats {
	byte _str, _dex, _int, _def, _atk, _maxHp, _maxGroup, _treasure;
};

extern const EnemyStats ENEMY_STATS[ENEMY_COUNT];
extern const uint16 ENEMY_FLAGS[ENEMY_COUNT];

// What the enemies can do, in ENEMY_FLAGS
enum EnemyFlag {
	ENEMY_FLAG_MAGIC = 0x8000,
	ENEMY_FLAG_SWIMS = 0x4000,
	ENEMY_FLAG_TELEPORT = 0x2000,
	ENEMY_FLAG_VANISH = 0x1000,
	ENEMY_FLAG_DISAPPEAR = 0x800,
	ENEMY_FLAG_SUMMON = 0x400,
	ENEMY_FLAG_POISON_BITE = 0x200,
	ENEMY_FLAG_BITE = 0x100,
	ENEMY_FLAG_STRONG = 0x80,
	ENEMY_FLAG_CHARM = 0x40,
	ENEMY_FLAG_UNDEAD = 0x20,
	ENEMY_FLAG_DIVIDE = 0x10,
	ENEMY_FLAG_NO_DAMAGE = 0x8,
	ENEMY_FLAG_POISON_STING = 0x4,
	ENEMY_FLAG_STEAL = 0x2,
	ENEMY_FLAG_NO_CORPSE = 0x1
};

extern const byte ENEMY_RANGE[48];
extern const byte ENEMY_SHOT[48];
extern const byte WEAPON_DAMAGE[56];
extern const byte EQUIPMENT_DEFENSE[48];
extern const byte WEAPON_RANGE[56];
extern const byte WEAPON_KIND[56];
extern const byte ENEMY_SPLIT[48];
extern const byte MONSTER_X[16];
extern const byte MONSTER_Y[16];
extern const byte PARTY_X[6];
extern const byte PARTY_Y[6];
extern const int8 SHOT_PATTERN[50];
extern const byte SPARK[16];
extern const byte PROJECTILE_PASS[32];
extern const char *const ENEMY_NAMES[48];
extern const char *const ENEMY_GROUP_NAMES[48];
extern const char *const EQUIPMENT_NAMES[48];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
