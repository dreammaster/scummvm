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

#ifndef ULTIMA5_DATA_SPELL_DATA_H
#define ULTIMA5_DATA_SPELL_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int SPELL_COUNT = 48;

// The words of power that the letters stand for, and what spells they make up when typed in order
extern const char *const SPELL_SYLLABLES[26];
extern const char *const SPELL_LETTERS[SPELL_COUNT];

// Where each can be cast, as bits: 8 outdoors, 4 in towns, 2 in dungeons and 1 in combat
extern const byte SPELL_PLACES[SPELL_COUNT];

// What each is mixed from, a bit for each of the reagents from the first
extern const byte SPELL_RECIPES[SPELL_COUNT];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
