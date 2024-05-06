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

#include "krondor/gfx/gfx_surface.h"
#include "krondor/events.h"

namespace Krondor {
namespace Gfx {

void GfxSurface::writeChar(char c) {
	writeString(Common::String::format("%c", c));
}

void GfxSurface::writeString(const Common::String &str) {
	Graphics::Font *font = UIElement::getFont();
	int strWidth = font->getStringWidth(str);
	font->drawString(this, str, _textX, _textY, strWidth, _textColor);

	_textX += strWidth;
}

} // namespace Gfx
} // namespace Krondor
