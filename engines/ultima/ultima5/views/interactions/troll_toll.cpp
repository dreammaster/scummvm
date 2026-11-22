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

#include "ultima/ultima5/views/interactions/troll_toll.h"
#include "ultima/ultima5/views/interactions/combat.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int TOLL_BASE = 99;
constexpr int TOLL_PER_STRENGTH = 3;
constexpr int SNEAK_RANGE = 0x1E;
constexpr byte TILE_TROLLS = 0xE4;

TrollToll::TrollToll(MessageLog &log) : Interaction(log) {
	const Data::Savegame &sg = _G(savegame);

	_log.print("\nThou spieth trolls under the bridge!\n\n");

	for (int i = 0; i < sg._partySize; ++i) {
		const Data::Character &c = sg._party[i];

		if (c._status == Data::STATUS_DEAD || c._status == Data::STATUS_SLEEP)
			continue;

		_log.print(c._name);
		_log.print(" sneaks across...\n\n");

		if (c._dex < _G(gameRandom).range(1, SNEAK_RANGE)) {
			// The first of the party who is fit is who the toll is worked out for
			int first = 0;
			for (int j = 0; j < sg._partySize; ++j) {
				if (sg._party[j]._status == Data::STATUS_GOOD || sg._party[j]._status == Data::STATUS_POISONED) {
					first = j;
					break;
				}
			}

			_toll = TOLL_BASE - TOLL_PER_STRENGTH * sg._party[first]._str;
			_log.print("Caught!\n\nThe trolls demand a ");
			_log.print(Common::String::format("%d", _toll));
			_log.print(" gp toll!\n\nDost thou pay?");
			_asking = true;
			return;
		}
	}

	_log.print("Trolls evaded!\n");
}

TrollToll::~TrollToll() {
	delete _next;
}

void TrollToll::fight() {
	Data::Savegame &sg = _G(savegame);
	int slot = -1;

	for (int i = 1; i < Data::ACTOR_COUNT && slot < 0; ++i) {
		if (sg.getActor(i)._tile == 0)
			slot = i;
	}

	if (slot < 0)
		return;

	Data::ActorRecord trolls;
	trolls._tile = TILE_TROLLS;
	trolls._animTile = 0;
	trolls._x = sg._mapX;
	trolls._y = sg._mapY;
	trolls._z = 0;
	sg.setActor(slot, trolls);

	_next = Combat::forObject(_log, slot);
}

bool TrollToll::keypress(const KeypressMessage &msg) {
	Data::Savegame &sg = _G(savegame);
	const int key = toupper(msg.ascii);

	if (key != 'Y' && key != 'N')
		return false;

	_log.printChar(key);
	_log.printChar('\n');
	_asking = false;

	if (key == 'Y' && sg._gold >= _toll) {
		sg._gold -= _toll;
		return true;
	}

	fight();
	return true;
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
