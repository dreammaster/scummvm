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

#ifndef ULTIMA5_DATA_SHRINE_DATA_H
#define ULTIMA5_DATA_SHRINE_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int SHRINE_COUNT = 8;

extern const char *const SHRINE_VIRTUES[SHRINE_COUNT];
extern const char *const SHRINE_MANTRAS[SHRINE_COUNT];
extern const char *const SHRINE_VIRTUE_WORDS[SHRINE_COUNT];
extern const byte SHRINE_X[SHRINE_COUNT];
extern const byte SHRINE_Y[SHRINE_COUNT];
extern const uint16 SHRINE_QUEST_TEXT[SHRINE_COUNT];
extern const uint16 SHRINE_CODEX_TEXT[SHRINE_COUNT];
extern const byte SHRINE_STRENGTH[SHRINE_COUNT];
extern const byte SHRINE_DEXTERITY[SHRINE_COUNT];
extern const byte SHRINE_INTELLIGENCE[SHRINE_COUNT];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
