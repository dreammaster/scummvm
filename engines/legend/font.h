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

#include "legend/resources.h"
#include "common/array.h"
#include "common/stream.h"
#include "graphics/surface.h"

namespace Legend {

class LegendEngine;
class Screen;

struct Spacing {
	uint leftSpacing : 4;
	uint rightSpacing : 4;
};

class Font {
private:
	Screen *_screen;
public:
	int _counter;
	int _sectionNum;
	int _fontNumber;
	int _field4;
	int _field5;
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
	static int _fontTabSize;
	static uint _lineSpacing;
	static uint _lineSpacingCenter;
	static int _textX;
	static int _textY;
	static int _fgColor;
	static int _bgColor;
	static uint _maxCharWidth;
	static uint _maxCharCenter;
	static int _fontSectionNum;
	static int _fontFieldA;

	static void init();
public:
	Font(Screen *vm);
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
	 * Returns the line spacing for the font
	 */
	uint getLineSpacing() const { return _linesOffset + _linesPerChar; }
};

} // End of namespace Legend

#endif
