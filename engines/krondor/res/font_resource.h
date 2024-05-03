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

#ifndef KRONDOR_RES_FONT_RESOURCE_H
#define KRONDOR_RES_FONT_RESOURCE_H

#include "graphics/font.h"
#include "krondor/res/resource.h"

namespace Krondor {

static const unsigned int MAX_FONT_HEIGHT = 16;

typedef uint16 GlyphData[MAX_FONT_HEIGHT];

struct FontGlyph {
	uint _width = 0;
	GlyphData _data;
};

class FontResource : public TaggedResource, public Graphics::Font {
private:
	int _fontHeight = 0;
	char _firstChar = '\0';
	byte _numChars = 0;
	byte _maxCharWidth = 0;
	Common::Array<FontGlyph> _glyphs;

public:
	FontResource() : TaggedResource(), Graphics::Font() {}
	FontResource(const Common::String &name) : TaggedResource(), Graphics::Font() {
		load(name);
	}
	~FontResource() override {}

	void clear();
	void load(const Common::String &name);

	int getFontHeight() const override;
	int getCharWidth(uint32 chr) const override;
	int getMaxCharWidth() const override;
	void drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const override;

};

} // namespace Krondor

#endif
