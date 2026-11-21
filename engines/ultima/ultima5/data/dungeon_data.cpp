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

#include "ultima/ultima5/data/dungeon_data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

const byte DUNGEON_ENTRANCE_X[40] = {
	232, 81, 36, 58, 159, 106, 22, 187,
	88, 152, 104, 216, 45, 176, 201, 153,
	86, 196, 84, 86, 88, 98, 136, 136,
	49, 15, 64, 248, 148, 218, 28, 146,
	240, 91, 72, 126, 156, 58, 239, 128
};

const byte DUNGEON_ENTRANCE_Y[40] = {
	135, 106, 222, 43, 20, 184, 128, 169,
	120, 24, 216, 120, 62, 208, 59, 91,
	107, 245, 106, 105, 106, 145, 90, 158,
	58, 160, 240, 8, 74, 107, 50, 241,
	73, 67, 168, 20, 27, 102, 240, 128
};

const byte SIGN_FIRST[8] = {
	0, 1, 2, 2, 3, 7, 10, 11
};

const byte SIGN_COUNT[8] = {
	1, 1, 0, 1, 4, 3, 1, 0
};

const byte SIGN_LEFT[12] = {
	7, 8, 8, 8, 9, 9, 9, 10,
	9, 9, 7, 0
};

const byte SIGN_TOP[12] = {
	10, 9, 10, 11, 10, 10, 10, 11,
	11, 11, 10, 0
};

const byte MONSTER_CONTACT[4] = {
	1, 3, 2, 3
};

const int16 VIEW_MONSTER_X[4] = {
	40, 72, 80, 88
};

const int16 VIEW_MONSTER_Y[8] = {
	86, 86, 96, 98, 16, 40, 70, 85
};

const int16 SPARK_FIRST[4] = {
	16, 56, 80, 92
};

const int16 SPARK_LAST[4] = {
	167, 135, 111, 99
};

const int16 SPARK_COUNT[4] = {
	300, 100, 50, 15
};

const int16 SPARK_WIDTH[4] = {
	7, 7, 5, 2
};

const int16 WALL_X[8] = {
	16, 40, 72, 88, 152, 120, 104, 96
};

const int16 ITEM_X[4] = {
	56, 72, 80, 88
};

const int16 ITEM_Y[8] = {
	152, 120, 104, 96, 15, 39, 71, 87
};

const byte WALL_FRAME[6] = {
	12, 8, 24, 8, 12, 12
};

const byte DOOR_LIGHT_A[13] = {
	40, 47, 66, 100, 146, 46, 50, 62,
	82, 109, 14, 23, 50
};

const byte DOOR_LIGHT_B[13] = {
	14, 23, 50, 98, 159, 40, 45, 60,
	84, 119, 90, 115, 82
};

const byte FLAME_POINTS[48] = {
	90, 115, 82, 119, 95, 123, 87, 125,
	91, 131, 80, 133, 80, 133, 80, 133,
	87, 115, 94, 119, 81, 122, 93, 125,
	95, 129, 85, 130, 89, 135, 80, 139,
	84, 117, 90, 117, 90, 124, 80, 126,
	85, 136, 89, 138, 89, 138, 89, 138
};

const byte SHIMMER_POINTS[30] = {
	22, 7, 19, 10, 13, 11, 23, 20,
	13, 27, 18, 6, 17, 15, 23, 15,
	12, 16, 15, 26, 15, 8, 22, 11,
	16, 19, 11, 21, 18, 28
};

const byte FIELD_POINTS[24] = {
	11, 1, 15, 4, 9, 6, 11, 8,
	10, 2, 14, 2, 12, 10, 12, 10,
	13, 1, 9, 4, 9, 9, 9, 9
};

const byte NEAR_POINTS[6] = {
	6, 0, 5, 1, 7, 2
};

const byte FAR_POINTS[6] = {
	89, 145, 80, 144, 85, 143
};

const byte FEATURE_FRAME_LEFT[8] = {
	0, 31, 0, 31, 0, 0, 0, 0
};

const byte FEATURE_FRAME_RIGHT[8] = {
	0, 0, 31, 31, 55, 39, 47, 63
};

const int8 SIDE_X[4] = {
	-1, 0, 1, 0
};

const int8 SIDE_Y[4] = {
	0, -1, 0, 1
};

const byte ROOM_CENTRE[8] = {
	0, 200, 201, 200, 220, 216, 0, 0
};

const byte CAMP_PARTY_X[6] = {
	5, 6, 5, 4, 4, 6
};

const byte CAMP_PARTY_Y[6] = {
	4, 5, 6, 5, 4, 6
};

const byte ROOM_SPOTS_A[6] = {
	5, 4, 6, 3, 5, 7
};

const byte ROOM_SPOTS_B[6] = {
	6, 7, 7, 8, 8, 8
};

const byte ROOM_SPOTS_C[6] = {
	4, 3, 3, 2, 2, 2
};

const byte ROOM_SPOTS_D[6] = {
	5, 4, 6, 3, 5, 7
};

const byte ENEMY_SPOTS_A[16] = {
	5, 4, 6, 3, 7, 2, 8, 5,
	2, 8, 3, 7, 2, 4, 6, 8
};

const byte ENEMY_SPOTS_B[16] = {
	8, 8, 8, 7, 7, 6, 6, 9,
	8, 8, 9, 9, 10, 10, 10, 10
};

const byte ENEMY_SPOTS_C[16] = {
	2, 2, 2, 3, 3, 4, 4, 1,
	2, 2, 1, 1, 0, 0, 0, 0
};

const byte ENEMY_SPOTS_D[16] = {
	5, 4, 6, 3, 7, 2, 8, 5,
	2, 8, 7, 3, 2, 4, 6, 8
};

const byte CAMP_ENEMY_X[16] = {
	2, 2, 2, 3, 3, 3, 4, 5,
	5, 6, 7, 7, 7, 8, 8, 8
};

const byte CAMP_ENEMY_Y[16] = {
	4, 5, 7, 2, 3, 8, 8, 3,
	8, 3, 4, 7, 8, 3, 6, 7
};

const byte WANDERER_KIND[8] = {
	20, 21, 22, 23, 24, 25, 28, 27
};

const byte WANDERER_MODE[8] = {
	96, 160, 0, 144, 128, 96, 0, 0
};

const byte ROOM_ENEMY_KIND[8] = {
	20, 21, 22, 34, 33, 24, 31, 24
};

const byte TREASURE_RANGE[13] = {
	8, 8, 4, 3, 8, 8, 4, 3,
	6, 3, 8, 1, 8
};

const byte TREASURE_BASE[13] = {
	0, 0, 30, 4, 1, 1, 0, 42,
	9, 45, 1, 1, 1
};

const byte ARRIVAL_FACING[6] = {
	0, 1, 2, 3, 0, 2
};

const byte CLEARED_ROOM_SKIP[6] = {
	80, 91, 65, 70, 75, 76
};

const char *const SIGN_TEXT[11] = {
	"BOTTOMLESS\n   PIT    ",
	"[E MAZE\n OF LO_\n SOULS ",
	"[E PRISON\n   WRO]  ",
	"[E CRYPT",
	"UPPER \nCRYPTS",
	"LOWER \nCRYPTS",
	"DEBTERS\n ALLY  ",
	"D\\P",
	"D\\PER",
	"D\\PE_",
	"MO[ER LODE\n   MAZE   "
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
