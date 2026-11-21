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

#include "ultima/ultima5/views/interactions/object_command.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

ObjectCommand::ObjectCommand(MessageLog &log, ObjectItems::Kind kind) : Interaction(log), _items(log, kind) {
	if (kind == ObjectItems::KIND_OPEN)
		ObjectItems::closeDoor();

	if (_items.ready())
		_state = STATE_DIRECTION;
	else
		_state = STATE_DONE;
}

bool ObjectCommand::startWho() {
	switch (_actor.begin(_log)) {
	case ActingMemberChooser::PENDING:
		_state = STATE_WHO;
		return false;

	case ActingMemberChooser::CHOSEN:
		_items.finish(_actor.who());
		break;

	default:
		break;
	}

	_state = STATE_DONE;
	return true;
}

bool ObjectCommand::keypress(const KeypressMessage &msg) {
	switch (_state) {
	case STATE_DIRECTION: {
		const DirectionChooser::Result result = _direction.handleKey(msg, _log);
		if (result == DirectionChooser::PENDING)
			return false;

		if (result == DirectionChooser::CANCELLED) {
			_state = STATE_DONE;
			return true;
		}

		int dx = 0, dy = 0;
		switch (_direction.direction()) {
		case DIR_NORTH:
			dy = -1;
			break;
		case DIR_EAST:
			dx = 1;
			break;
		case DIR_SOUTH:
			dy = 1;
			break;
		default:
			dx = -1;
			break;
		}

		if (_items.begin(dx, dy) == ObjectItems::NEED_NONE) {
			_state = STATE_DONE;
			return true;
		}

		return startWho();
	}

	case STATE_WHO:
		if (_actor.handleKey(msg, _log) == ActingMemberChooser::PENDING)
			return false;

		if (_actor.who() != -1)
			_items.finish(_actor.who());

		_state = STATE_DONE;
		return true;

	default:
		return true;
	}
}

void ObjectCommand::draw(View &view) {
	_actor.draw(view);
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
