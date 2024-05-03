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

#include "krondor/res/font_resource.h"

namespace Krondor {

//void FontResource::clear() {}

void FontResource::read(Common::SeekableReadStream *src) {
	clear();

}

int FontResource::getFontHeight() const {
	return 0;
}

int FontResource::getCharWidth(uint32 chr) const {
	return 8;
}


void FontResource::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
}


} // namespace Krondor
