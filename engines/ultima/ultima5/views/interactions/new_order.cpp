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

#include "ultima/ultima5/views/interactions/new_order.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

bool NewOrder::keypress(const KeypressMessage &msg) {
	const PartyMemberChooser::Result result = _chooser.handleKey(msg);
	if (result == PartyMemberChooser::PENDING)
		return false;

	if (result == PartyMemberChooser::CANCELLED) {
		_log.print("nobody!\n");
		return true;
	}

	const Data::Savegame &sg = _G(savegame);
	const int member = _chooser.index();
	_log.print(sg._party[member]._name);

	if (member == 0) {
		_log.print(Common::String("\n\n") + sg._party[0]._name + " must lead!\n");
		return true;
	}

	if (_first == -1) {
		_first = member;
		_chooser.reset();
		_log.print("\nwith ");
		return false;
	}

	_log.print("!\n");
	PartyLogic(_log).swapMembers(_first, member);
	return true;
}

void NewOrder::draw(View &view) {
	_chooser.draw(view);
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
