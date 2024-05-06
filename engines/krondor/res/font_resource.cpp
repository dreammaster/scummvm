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

#include "graphics/surface.h"
#include "krondor/events.h"
#include "krondor/res/font_resource.h"

namespace Krondor {

void FontResource::clear() {
	_fontHeight = 0;
	_firstChar = '\0';
	_numChars = 0;
	_maxCharWidth = 0;
	_glyphs.clear();
}

void FontResource::load(const Common::String &name) {
	clear();
	File f(name);
	loadIndex(&f);

	Common::SeekableReadStream *font = getTag(&f, TAG_FNT);

	font->skip(2);
	_fontHeight = font->readUint16LE() & 0xff;
	_firstChar = (char)font->readByte();
	_numChars = font->readByte();
	font->skip(2);

	if (font->readByte() != 0x01)
		error("Unsupported font compression");

	Common::SeekableReadStream *glyphs = DecompressRLE(font).decompress();
	Common::Array<int> glyphOffset(_numChars);
	_glyphs.resize(_numChars);

	for (uint i = 0; i < _numChars; i++)
		glyphOffset[i] = glyphs->readUint16LE();

	uint glyphDataStart = glyphs->pos();
	for (uint i = 0; i < _numChars; i++) {
		FontGlyph &glyph = _glyphs[i];

		glyphs->seek(glyphDataStart + i);
		glyph._width = glyphs->readByte();
		_maxCharWidth = MAX<byte>(_maxCharWidth, glyph._width);

		glyphs->seek(glyphDataStart + _numChars + glyphOffset[i]);

		for (uint j = 0; j < _fontHeight; j++) {
			glyph._data[j] = (uint16)glyphs->readByte() << 8;
			if (glyph._width > 8) {
				glyph._data[j] += (uint16)glyphs->readByte();
			}
		}
	}
}

int FontResource::getFontHeight() const {
	return _fontHeight;
}

int FontResource::getCharWidth(uint32 chr) const {
	assert(chr >= _firstChar && chr < (_firstChar + _numChars));
	return _glyphs[chr - _firstChar]._width;
}

int FontResource::getMaxCharWidth() const {
	return _maxCharWidth;
}

void FontResource::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
	assert(chr >= _firstChar && chr < (_firstChar + _numChars));
	const FontGlyph &glyph = _glyphs[chr - _firstChar];
	uint16 pixels;

	for (uint yp = 0; yp < _fontHeight; ++yp, ++y) {
		byte *destP = (byte *)dst->getBasePtr(x, y);
		pixels = glyph._data[yp];

		for (uint xp = 0; xp < glyph._width; ++xp, ++destP, pixels <<= 1) {
			if (pixels & 0x8000)
				*destP = color;
		}
	}
}

void FontResource::setActive() {
	UIElement::setFont(this);
}

} // namespace Krondor
