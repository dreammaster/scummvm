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

#include "legend/games/gateway/utils/queue.h"
#include "legend/messages.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Utils {

void Queue::add(int param1, int param2) {
	int index = indexOf(param1);

	if (index != -1) {
		_queue[index]._field2 = param2;

	} else if (_queue.size() < QUEUE_SIZE) {
		_queue.push_back(QueueEntry(param1, param2));

	} else {
		TextMessage::display("[ERROR: Queue overflow]\n");
	}
}

int Queue::indexOf(int param1) const {
	for (uint i = 0; i < _queue.size(); ++i) {
		if (_queue[i]._field0 == param1)
			return i;
	}

	return -1;
}


} // namespace Utils
} // namespace Gateway
} // namespace Early
} // namespace Legend
