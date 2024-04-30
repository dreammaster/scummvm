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

#include "krondor/res/rect.h"

namespace Krondor {

void Rect::clear() {
	left = top = right = bottom = 0;
}

void Rect::synchronize(Common::Serializer &s) {
	s.syncAsSint16LE(left);
	s.syncAsSint16LE(top);

	int w = width();
	int h = height();
	s.syncAsSint16LE(w);
	s.syncAsSint16LE(h);

	if (s.isLoading()) {
		setWidth(w);
		setHeight(h);
	}
}

void Rect::read(Common::SeekableReadStream *src) {
	Common::Serializer s(src, nullptr);
	synchronize(s);
}

void Rect::write(Common::WriteStream *dest) {
	Common::Serializer s(nullptr, dest);
	synchronize(s);
}

} // namespace Krondor
