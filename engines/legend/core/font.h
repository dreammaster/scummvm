/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LEGEND_FONT_H
#define LEGEND_FONT_H

#include "legend/core/resources.h"
#include "common/array.h"
#include "common/stream.h"
#include "graphics/managed_surface.h"

namespace Legend {

class LegendEngine;
class Screen;

#define FONT_COUNT 2

struct Spacing {
	uint leftSpacing : 4;
	uint rightSpacing : 4;
};

struct Font2 {
	int _field0;
	int _field2;
	int _field4;
	int _field6;
	int _field8;
	int _fieldA;
	int _fieldC;
	int _fieldE;
	int _field10;
	int _field12;
	int _field14;
	int _field16;
	int _field18;
	int _field1A;
	int _field1C;
	int _field1E;
	int _field20;
	int _field22;
	int _color;
	int _field26;
	int _field28;
	int _field2A;
	int _field2C;
	int _field2E;
	int _field30;
	int _field32;
	int _field34;
	int _field36;
	int _field38;
	int _field3A;
	int _field3C;
	int _field3E;
	int _field40;
	int _field42;
	int _field44;
	int _field46;
	int _field48;
	int _field4A;
	int _field4C;
	int _field4E;
	int _field50;
	int _field52;
	int _field54;
	int _field56;
public:
	/**
	 * Constructor
	 */
	Font2();

	int fn1(bool flag, int color, int val3);
};

class Font {
private:
	Graphics::ManagedSurface *_surface;
private:
	static Font *getFreeSlot();
public:
	int _counter;
	int _sectionNum;
	int _fontNumber;

	int _field3;
	int _field4;
	int _field5;
	int _fieldA;

	int _bytesPerLine;
	int _minPrintableChar;
	int _maxPrintableChar;
	int _linesOffset;
	int _linesPerChar;
	int _fixedWidth;
	int _fixedSpacing;
	int _fieldD;
	Common::Array<uint> _charWidths;
	Common::Array<Spacing> _charSpacings;
	Common::Array<byte> _pixelData;
public:
	static Font *_fonts[FONT_COUNT];
	static Font *_activeFont;
	static Font2 *_font2;
	static int _currentSection;
	static int _textX;
	static int _textY;
	static int _tabWidth;
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
	 * Set the position for writing text to
	 */
	static void setTextPos(const Common::Point &pt);

	/**
	 * Sets the width of tab characters
	 */
	static int setTabWidth(int width);
public:
	Font();
	~Font() {}

	/**
	 * Load the data for a font
	 */
	void load(Common::SeekableReadStream &s);

	/**
	 * Draw a character
	 */
	void writeChar(char c);

	/**
	 * Write out a string
	 */
	void writeString(const TextMessage &msg);

	/**
	 * Return the width of a character
	 */
	uint charWidth(char c) const;

	/**
	 * Return the width of a string
	 */
	uint stringWidth(const TextMessage &msg) const;

	/**
	 * Returns the height of the font
	 */
	uint lineHeight() const { return _linesOffset + _linesPerChar; }

	/**
	 * Sets the given font as the currently active one
	 */
	void setActive();
};

} // End of namespace Legend

#endif
