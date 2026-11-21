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

#ifndef ULTIMA5_DATA_DUNGEON_DATA_H
#define ULTIMA5_DATA_DUNGEON_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

extern const byte DUNGEON_ENTRANCE_X[40];
extern const byte DUNGEON_ENTRANCE_Y[40];
extern const byte SIGN_FIRST[8];
extern const byte SIGN_COUNT[8];
extern const byte SIGN_LEFT[12];
extern const byte SIGN_TOP[12];
extern const byte MONSTER_CONTACT[4];
extern const int16 MONSTER_X[4];
extern const int16 MONSTER_Y[8];
extern const int16 SPARK_FIRST[4];
extern const int16 SPARK_LAST[4];
extern const int16 SPARK_COUNT[4];
extern const int16 SPARK_WIDTH[4];
extern const int16 WALL_X[8];
extern const int16 ITEM_X[4];
extern const int16 ITEM_Y[8];
extern const byte WALL_FRAME[6];
extern const byte DOOR_LIGHT_A[13];
extern const byte DOOR_LIGHT_B[13];
extern const byte FLAME_POINTS[48];
extern const byte SHIMMER_POINTS[30];
extern const byte FIELD_POINTS[24];
extern const byte NEAR_POINTS[6];
extern const byte FAR_POINTS[6];
extern const byte FEATURE_FRAME_LEFT[8];
extern const byte FEATURE_FRAME_RIGHT[8];
extern const int8 SIDE_X[4];
extern const int8 SIDE_Y[4];
extern const byte ROOM_CENTRE[8];
extern const char *const SIGN_TEXT[11];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
