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

#ifndef ULTIMA_SHARED_GFX_SURFACE_H
#define ULTIMA_SHARED_GFX_SURFACE_H

#include "common/ptr.h"
#include "graphics/font.h"
#include "graphics/managed_surface.h"

namespace Ultima {
namespace Shared {
namespace Gfx {

class GfxSurface : public Graphics::ManagedSurface {
private:
	Common::Point _textPos;
	bool _scrollable = false;
	byte _fgColor = 255;
	byte _bgColor = 0;
	Common::SharedPtr<Graphics::Font> _font;

	void newLine();

public:
	GfxSurface(Graphics::Font *font = nullptr);
	GfxSurface(Graphics::ManagedSurface &surf, const Common::Rect &bounds, Graphics::Font *font = nullptr);
	~GfxSurface() override {}

	/**
	 * Write some text to the surface
	 */
	void writeString(const Common::Point &pt, const Common::String &str,
		Graphics::TextAlign align = Graphics::kTextAlignLeft);
	void writeString(const Common::String &str, Graphics::TextAlign align = Graphics::kTextAlignLeft);
	virtual void writeChar(uint32 chr);
	int getStringWidth(const Common::String &str) const {
		return _font->getStringWidth(str);
	}
	int getFontHeight() const {
		return _font->getFontHeight();
	}

	void setTextPos(const Common::Point &pt);
	byte setColor(byte color);
	byte setColor(byte r, byte g, byte b);
	void setColor(byte fgColor, byte bgColor);
	byte setBgColor(byte color);
	byte setBgColor(byte r, byte g, byte b);
	void reverseColor();
	void setScrollable(bool scrollable) {
		_scrollable = scrollable;
	}

	void xorBlitFrom(const ManagedSurface &src, const Common::Point &destPos);
};

} // namespace Gfx
} // namespace Shared
} // namespace Ultima

#endif
