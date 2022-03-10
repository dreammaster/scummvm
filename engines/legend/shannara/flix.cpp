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

#include "legend/shannara/flix.h"
#include "legend/memory.h"
#include "legend/shannara/globals.h"

namespace Legend {
namespace Shannara {

void q_init(int y, PalettePtr pal) {
	if (!_GS(qbuffer) && pal) {
		_GS(qpalette) = pal;
		_GS(qbuffer) = (byte *)new_pointer(0x13C06);

		if (_GS(qbuffer)) {
			_GS(qbuffer_orig) = _GS(qbuffer);
			_GS(video_buffer) = gxVideoAddr(0, y);
			_GS(q_y1) = y;
			_GS(q_y2) = y + 287;
			memset(_GS(qpalette), 0, PALETTE_SIZE);
		}
	}
}

} // namespace Shannara
} // namespace Legend
