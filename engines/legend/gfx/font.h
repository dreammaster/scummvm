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

#ifndef LEGEND_GFX_FONT_H
#define LEGEND_GFX_FONT_H

#include "common/array.h"
#include "common/stream.h"
#include "graphics/managed_surface.h"

namespace Legend {
namespace Gfx {

#define FONT_COUNT 2

struct Spacing {
	uint leftSpacing : 4;
	uint rightSpacing : 4;
};

union SpacingUnion {
	Spacing _vals;
	byte _byte;
};

class Font {
private:
	/**
	 * Frees the least recently used font, and returns a new font
	 * instance that replaces it
	 */
	static Font *getFreeSlot();
public:
	int _counter = 0;
	int _fontNumber = -1;

	int _field4 = 0;
	int _field5 = 0;
	int _fieldA = 0;

	int _bytesPerLine = 0;
	int _minPrintableChar = 0;
	int _maxPrintableChar = 0;
	int _linesOffset = 0;
	int _linesPerChar = 0;
	int _fixedWidth = 0;
	int _fixedSpacing = 0;
	int _fieldD = 0;
	Common::Array<uint> _charWidths;
	Common::Array<SpacingUnion> _charSpacings;
	Common::Array<byte> _pixelData;
public:
	static Font *_fonts[FONT_COUNT];
	static Font *_activeFont;
	static uint _tabWidth;
	static uint _lineHeight;
	static uint _xCenter, _yCenter;
	static int _fgColor, _bgColor;
	static int _overrideColor;
	static uint _maxCharWidth;
	static uint _maxCharCenter;
	static int _fontSectionNum;
	static int _fontFieldA;
public:
	/**
	 * Initialises static font data
	 */
	static void init();

	/**
	 * De-initializes static font data
	 */
	static void deinit();

	/**
	 * Loads a font and makes it the active one
	 */
	static Font *loadFont(int fontNumber);

	/**
	 * Return the currently active font
	 */
	static Font *getActiveFont() { return _activeFont; }

	/**
	 * Gets the current font colors
	 */
	static void getColor(int &fgColor, int &bgColor);

	/**
	 * Sets the font colors
	 */
	static void setColor(int fgColor, int bgColor = -1);

	/**
	 * Sets the override color
	 */
	static void setOverrideColor(int color);

	/**
	 * Sets the width of tab characters
	 */
	static int setTabWidth(int width);
public:
	Font() {}
	~Font() {}

	/**
	 * Load the data for a font
	 */
	void load(Common::SeekableReadStream &s);

	/**
	 * Draw a character
	 */
	void writeChar(Graphics::ManagedSurface &surface, Common::Point &textPos, char c);

	/**
	 * Write out a string
	 */
	void writeString(Graphics::ManagedSurface &surface, Common::Point &textPos, const Common::String &msg);

	/**
	 * Return the width of a character
	 */
	uint charWidth(char c) const;

	/**
	 * Return the width of a string
	 */
	uint stringWidth(const Common::String &msg) const;

	/**
	 * Returns the height of the font
	 */
	uint lineHeight() const { return _linesOffset + _linesPerChar; }

	/**
	 * Sets the given font as the currently active one
	 */
	void setActive();
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
