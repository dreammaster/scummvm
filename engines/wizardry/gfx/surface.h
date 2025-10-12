
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

#ifndef WIZARDRY_GFX_SURFACE_H
#define WIZARDRY_GFX_SURFACE_H

#include "graphics/managed_surface.h"

namespace Wizardry {

class Surface : public Graphics::ManagedSurface {
private:
	Common::Point _textPos;
	int _textColor = 1;

public:
	Surface(Graphics::ManagedSurface &src, const Common::Rect &bounds) :
		Graphics::ManagedSurface(src, bounds) {}

	void MVCURSOR(int x, int y) {
		_textPos.x = x;
		_textPos.y = y;
	}
	void PRINTCHR(char c);
	void writeString(const Common::String &str);
	void writeString(int x, int y, const Common::String &str) {
		MVCURSOR(x, y);
		writeString(str);
	}

	void CLRRECT(int x1, int y1, int x2, int y2);
};

} // namespace Wizardry

#endif
