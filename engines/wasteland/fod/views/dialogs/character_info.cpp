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

#include "wasteland/fod/views/dialogs/character_info.h"
#include "wasteland/fod/data/strings.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

bool CharacterInfo::msgGame(const GameMessage& msg) {
	if (msg._name == "INFO") {
		assert(msg._value >= 0 && msg._value < g_engine->_disk1._partyCount);
		_member = &g_engine->_disk1._party[msg._value];

		addView();
		return true;
	}

	return false;
}

void CharacterInfo::draw() {
	drawBorders(1);

	writeStats();
}

void CharacterInfo::writeStats() {
	Surface s = getSurface(Gfx::Window(1, 1, 23, 13));

	s.setInverseColor(_member->_afflicted);
	s.writeCenteredString(_member->_name, 0);
	s.setInverseColor(false);

	s.writeString(Common::String::format("Rank: %u", _member->_rank), 0, 1);
	s.writeString(Common::String::format("Cond: %s",
		_member->_afflicted ? "Afflicted" : "Unafflicted"), 0, 2);

	if (_member->_afflicted)
		s.writeString("(V)iew", 16, 2);

	auto status = _member->getStatus();
	if (status == Data::STATUS_OK) {
		s.writeString(Common::String::format(" Con: %3u  MaxCon:  %3u",
			_member->_con, _member->_conBase), 0, 3);
	} else {
		s.writeString(Common::String::format(" Con: %3s  MaxCon:  %3u",
			Data::STATUSES[status], _member->_conBase), 0, 3);
	}
}


bool CharacterInfo::msgKeypress(const KeypressMessage &msg) {
	close();
	return true;
}

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland
