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

#include "ultima/ultima5/views/interactions/attack.h"
#include "ultima/ultima5/views/interactions/combat.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

Attack::Attack(MessageLog &log) : Interaction(log), _logic(log) {
	_done = !_logic.canAttack();
}

Attack::~Attack() {
	delete _next;
}

bool Attack::keypress(const KeypressMessage &msg) {
	const DirectionChooser::Result result = _chooser.handleKey(msg, _log);
	if (result == DirectionChooser::PENDING)
		return false;

	// A turn is used in a town even if nobody was chosen
	_time = _G(savegame)._mapId != 0;
	if (result == DirectionChooser::CANCELLED)
		return true;

	static const int8 DX[] = { 0, 1, 0, -1 };
	static const int8 DY[] = { -1, 0, 1, 0 };

	AttackLogic::Fight fight;
	_time = _logic.attack(DX[_chooser.direction()], DY[_chooser.direction()], fight);

	if (fight._started) {
		_logic.prepare(fight);

		Combat *combat = new Combat(_log);
		combat->setAftermath(fight);

		if (!combat->start(fight._kind, fight._map, fight._lone) || combat->done())
			delete combat;
		else
			_next = combat;
	}

	return true;
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
