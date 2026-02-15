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

#ifndef ULTIMA2_DATA_PLAYER_H
#define ULTIMA2_DATA_PLAYER_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima2 {

constexpr int MAX_NAME_LENGTH = 12;

extern const char *SEX[];
extern const char *RACE[];
extern const char *CLASS[];

enum Sex {
	SEX_MALE, SEX_FEMALE
};
enum Race {
	RACE_HUMAN = 1, RACE_ELF = 2, RACE_DWARF = 3, RACE_HOBBIT = 4
};
enum Class {
	CLASS_FIGHTER = 1, CLASS_CLERIC = 2, CLASS_WIZARD = 3, CLASS_THIEF = 4
};

struct Player {
	char _name[MAX_NAME_LENGTH + 1] = {};
	byte _strength = 0;
	byte _agility = 0;
	byte _stamina = 0;
	byte _charisma = 0;
	byte _wisdom = 0;
	byte _intelligence = 0;
	byte _sex = SEX_MALE;
	byte _race = RACE_HUMAN;
	byte _class = CLASS_FIGHTER;

	byte _field24 = 20;
	byte _field25 = 20;
	byte _field13 = 2;
	byte _field1B = 4;
	byte _field1D = 4;
	byte _field22 = 4;
};

} // namespace Ultima2
} // namespace Ultima

#endif
