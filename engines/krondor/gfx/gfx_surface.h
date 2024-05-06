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

#ifndef KRONDOR_GFX_GFX_SURFACE_H
#define KRONDOR_GFX_GFX_SURFACE_H

#include "graphics/managed_surface.h"

namespace Krondor {
namespace Gfx {

class GfxSurface : public Graphics::ManagedSurface {
private:
	int _textX = 0, _textY = 0;
	int _textColor = 0;

public:
	GfxSurface() {}
	GfxSurface(Graphics::ManagedSurface &surf, const Common::Rect &bounds) :
		Graphics::ManagedSurface(surf, bounds) {}
	~GfxSurface() override {}

	void writeChar(char c);
	void writeString(const Common::String &str);

	void setTextPos(const Common::Point &pos) {
		_textX = pos.x;
		_textY = pos.y;
	}
	void writeChar(const Common::Point &pos, char c) {
		setTextPos(pos);
		writeChar(c);
	}
	void writeString(const Common::Point &pos, const Common::String &str) {
		setTextPos(pos);
		writeString(str);
	}
	void writeChar(int textX, int textY, char c) {
		setTextPos(Common::Point(textX, textY));
		writeChar(c);
	}
	void writeString(int textX, int textY, const Common::String &str) {
		setTextPos(Common::Point(textX, textY));
		writeString(str);
	}

	void setTextColor(int col) {
		_textColor = col;
	}
	int getTextColor() const {
		return _textColor;
	}
};

} // namespace Gfx
} // namespace Krondor

#endif
