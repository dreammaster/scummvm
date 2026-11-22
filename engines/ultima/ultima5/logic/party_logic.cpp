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
#include "ultima/ultima5/logic/combat_logic.h"
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

void PartyLogic::hurtMember(int member, int amount) {
	Data::Savegame &sg = _G(savegame);
	Data::Character &c = sg._party[member];

	c._hp -= amount;
	if (c._hp <= 0) {
		c._hp = 0;
		c._status = Data::STATUS_DEAD;

		if (member == sg._activeChar)
			sg._activeChar = Data::NO_ACTIVE_CHARACTER;
	}
}

void PartyLogic::turnUpkeep() {
	Data::Savegame &sg = _G(savegame);
	int fit = 0;

	for (int i = 0; i < sg._partySize; ++i) {
		const byte status = sg._party[i]._status;

		if (status == Data::STATUS_DEAD && i == sg._activeChar)
			sg._activeChar = Data::NO_ACTIVE_CHARACTER;

		if (status != Data::STATUS_DEAD && status != Data::STATUS_SLEEP) {
			if (status == Data::STATUS_POISONED)
				hurtMember(i, 1);

			++fit;
		}
	}

	if (sg._hour != sg._hourCopy) {
		if (sg._food == 0) {
			_log.print("Starving!\n");
			hurtParty();
		} else if (sg._hour == 6 || sg._hour == 12 || sg._hour == 18) {
			sg._food = sg._food > fit ? sg._food - fit : 0;
		}

		sg._hourCopy = sg._hour;
	}

	sg._turnCount = sg._turnCount + 1 < 0xFF ? sg._turnCount + 1 : 0xFF;

	if (sg._protection != 0 && sg._protection != 0xFF) {
		if (--sg._protection == 0)
			sg._icon = 0;
	}

	CombatLogic::regenerate();
}

void PartyLogic::wakeSleepers() {
	Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_SLEEP && _G(gameRandom).range(0, 0xF) == 0xF)
			sg._party[i]._status = Data::STATUS_GOOD;
	}
}

bool PartyLogic::allAsleep() {
	const Data::Savegame &sg = _G(savegame);
	bool asleep = false;

	for (int i = 0; i < sg._partySize; ++i) {
		const byte status = sg._party[i]._status;

		if (status == Data::STATUS_GOOD || status == Data::STATUS_POISONED)
			return false;

		if (status == Data::STATUS_SLEEP)
			asleep = true;
	}

	return asleep;
}

void PartyLogic::stepOnPoison(int lowest) {
	Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < sg._partySize; ++i) {
		Data::Character &c = sg._party[i];

		if (c._status != Data::STATUS_DEAD && c._status != Data::STATUS_POISONED) {
			if (c._dex < _G(gameRandom).range(lowest, 0x1E - (lowest == 0 ? 1 : 0))) {
				c._status = Data::STATUS_POISONED;
				_log.print("Poisoned!\n");
			}
		}
	}
}

void PartyLogic::stepOnFire() {
	_log.print("Burning!\n");
	hurtParty();
}

void PartyLogic::hurtParty() {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < 6; ++i) {
		if (i < sg._partySize && sg._party[i]._status != Data::STATUS_DEAD)
			hurtMember(i, _G(gameRandom).range(1, 8));
	}
}

} // namespace Ultima5
} // namespace Ultima
