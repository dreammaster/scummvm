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

#ifndef SPYCRAFT_GAME_FLAG_H
#define SPYCRAFT_GAME_FLAG_H

#include "spycraft/game/object.h"

namespace Spycraft {

#define MAX_DELAY_BEATS 22
#define MAX_BEATS	51
#define MULTI 9999
#define START_COUNT 9998
#define BEAT_TRIGGERED 9997
#define NEVER_SET 9996
#define BEAT_CLOSED 9995


#define FLAG_SIZE 25		

class Flag : public Object {
public:
	unsigned long data[FLAG_SIZE];

	Flag();
	int checkBeat(unsigned theFlag);
	int updateBeat(unsigned theFlag);
	void set(unsigned theFlag);
	void clear(unsigned theFlag);
	int test(unsigned theFlag);
};

extern void Bset(unsigned theFlag);
extern void Bclr(unsigned theFlag);
extern int Btst(unsigned theFlag);
extern int Random(int start_range, int end_range);

extern Flag GameFlag;

} // namespace Spycraft

#endif
