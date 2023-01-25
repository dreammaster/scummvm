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

#include "legend/early/logic/queue.h"
#include "legend/early/engine.h"

namespace Legend {
namespace Early {

#define QUEUE_MAX_COUNT 16

void Queue::QueueEntry::synchronize(Common::Serializer &s) {
	s.syncAsByte(_val1);
	s.skip(1);
	s.syncAsUint16LE(_val2);
}

void Queue::add(int val1, int val2) {
	uint idx;
	for (idx = 0; idx < _items.size() && _items[idx]._val1 != val1; ++idx) {
	}

	if (idx != _items.size()) {
		_items[idx]._val2 = val2;
	} else if (idx < QUEUE_MAX_COUNT) {
		_items.push_back(QueueEntry(val1, val2));
	} else {
		g_engine->send(TextMessage("[ERROR: Queue overflow]\n"));
	}
}

void Queue::synchronize(Common::Serializer &s) {
	uint count = _items.size();
	s.syncAsUint16LE(count);

	for (size_t i = 0; i < count; ++i)
		_items[i].synchronize(s);
}

} // namespace Early
} // namespace Legend
