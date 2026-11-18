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

#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

// How long a torch burns outside, in minutes
constexpr byte TORCH_DURATION = 0xF0;

void PartyLogic::igniteTorch() {
	Data::Savegame &sg = _G(savegame);
	_log.print("Ignite torch!\n");

	if (sg._torches == 0) {
		_log.print("None owned!\n");
		return;
	}

	--sg._torches;
	sg._unk0301 = TORCH_DURATION;
}

void PartyLogic::swapMembers(int first, int second) {
	Data::Savegame &sg = _G(savegame);
	assert(first > 0 && second > 0 && first < Data::PARTY_MAX && second < Data::PARTY_MAX);

	SWAP(sg._party[first], sg._party[second]);
}

} // namespace Ultima5
} // namespace Ultima
