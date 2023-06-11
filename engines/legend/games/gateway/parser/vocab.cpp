/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/games/gateway/parser/vocab.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Parser {

#define CAPITALIZE_COUNT 18
const uint16 CAPITALIZE_IDS[18] = {
	153, 879, 953, 959, 1043, 1193, 1253, 1260, 1460,
	1497, 1621, 1630, 1978, 1989, 2332, 2357, 2610, 2640
};

bool Vocab::capitalizeVocab(int vocabId) const {
	for (int i = 0; i < CAPITALIZE_COUNT; ++i) {
		if (vocabId == CAPITALIZE_IDS[i])
			return true;
	}

	return false;
}

} // namespace Parser
} // namespace Gateway
} // namespace Early
} // namespace Legend
