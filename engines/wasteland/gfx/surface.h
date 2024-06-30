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

#ifndef WASTELAND_GFX_SURFACE_H
#define WASTELAND_GFX_SURFACE_H

#include "common/rect.h"
#include "graphics/font.h"
#include "graphics/managed_surface.h"

namespace Wasteland {
namespace Shared {
namespace Gfx {

#define TEXT_W 40
#define TEXT_H 25
#define FONT_W 8
#define FONT_H 8

struct Window : public Common::Rect {
	int _xOffset = 0;
	int _yOffset = 0;

	Window() : Common::Rect() {
	}
	Window(int x1, int y1, int x2, int y2, int xOffset = 0, int yOffset = 0) :
		Common::Rect(x1, y1, x2, y2), _xOffset(xOffset), _yOffset(yOffset) {
	}
	Window(const Common::Rect &r, int xOffset = 0, int yOffset = 0) :
		Common::Rect(r), _xOffset(xOffset), _yOffset(yOffset) {
	}
};

struct Position : public Common::Point {
	int _xOffset = 0;
	int _yOffset = 0;

	Position() : Common::Point() {
	}
	Position(int x, int y) : Common::Point(x, y) {
	}
	Position(int x, int y, int xOffset, int yOffset) : Common::Point(x, y),
		_xOffset(xOffset), _yOffset(yOffset) {
	}
};

/**
 * Implements the surface class views use when they call getSurface
 */
class Surface : public Graphics::ManagedSurface {
private:
	bool _inverseColor = false;
	int _textX = 0, _textY = 0;
	Graphics::Font *_currentFont;

public:
	/**
	 * Sets the EGA palette
	 */
	static void setupPalette();

public:
	Surface();
	Surface(ManagedSurface &surf, const Common::Rect &bounds);

	void setFont(int fontNum);
	void setInverseColor(bool isInverse) {
		_inverseColor = isInverse;
	}
	void writeString(const Common::String &str);
	void writeString(const Common::String &str, int x, int y);
	void writeCenteredString(const Common::String &str, int y);
	void writeChar(unsigned char c);
	void writeChar(unsigned char c, int x, int y);
	void setTextPos(int x, int y);
};

} // namespace Gfx
} // namespace Shared

using Shared::Gfx::Window;
using Shared::Gfx::Surface;

} // namespace Wasteland

#endif
