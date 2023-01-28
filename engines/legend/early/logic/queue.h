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

#ifndef LEGEND_EARLY_LOGIC_QUEUE_H
#define LEGEND_EARLY_LOGIC_QUEUE_H

#include "common/array.h"
#include "common/serializer.h"

namespace Legend {
namespace Early {

/**
 * Unknown queue class
 */
class Queue {
	struct QueueEntry {
		int _id = 0;
		int _val2 = 0;
		QueueEntry() {}
		QueueEntry(int val1, int val2) : _id(val1), _val2(val2) {}

		/**
		 * Synchronize the entry
		 */
		void synchronize(Common::Serializer &s);
	};
private:
	Common::Array<QueueEntry> _items;
public:
	/**
	 * Add a new queue entry
	 */
	void add(int val1, int val2);

	/**
	 * Returns the index of an entry with the given Id
	 */
	int indexOf(int id) const;

	/**
	 * Returns true if an entry with the given id exists
	 */
	bool exists(int id) const {
		return indexOf(id) != -1;
	}

	/**
	 * Synchronize the queue
	 */
	void synchronize(Common::Serializer &s);
};

} // namespace Early
} // namespace Legend

#endif
