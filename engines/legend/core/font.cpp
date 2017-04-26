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

#include "legend/core/screen.h"
#include "legend/legend.h"
#include "common/system.h"
#include "common/util.h"
#include "graphics/palette.h"

namespace Legend {

Font2::Font2() {
	_field0 = 0;
	_field2 = 0;
	_field4 = 0;
	_field6 = 0;
	_field8 = 0;
	_fieldA = 0;
	_fieldC = 1;
	_fieldE = 0;
	_field10 = 1;
	_field12 = 0;
	_field14 = 0;
	_field16 = 0;
	_field18 = 0;
	_field1A = 0;
	_field1C = 0;
	_field1E = 0x3FF;
	_field20 = 0x3FF;
	_field22 = 0;
	_color = 0;
	_field26 = 1;
	_field28 = -1;
	_field2A = 1;
	_field2C = 0;
	_field2E = 0;
	_field30 = 0;
	_field32 = 0;
	_field34 = 0;
	_field36 = 0;
	_field38 = 0x3FF;
	_field3A = 0x3FF;
	_field3C = 0;
	_field3E = 0;
	_field40 = 0;
	_field42 = 0;
	_field44 = 0;
	_field46 = 0;
	_field48 = 0;
	_field4A = 0;
	_field4C = 0;
	_field4E = 1;
	_field50 = 3;
	_field52 = 1;
	_field54 = 1;
	_field56 = 1;
}

int Font2::fn1(bool flag, int color, int val3) {
	if (val3 < 0 || val3 > 11)
		return -4001;

	_field22 = val3;
	_color = color;
	_field26 = flag ? 1 : 8;
	return 0;
}

/*-------------------------------------------------------------------*/

Font2 *Font::_font2;
int Font::_fontTabSize;
uint Font::_lineSpacing;
uint Font::_lineSpacingCenter;
int Font::_textX;
int Font::_textY;
int Font::_fgColor;
int Font::_bgColor;
int Font::_overrideColor;
uint Font::_maxCharWidth;
uint Font::_maxCharCenter;
int Font::_fontSectionNum;
int Font::_fontFieldA;

void Font::init() {
	_font2 = new Font2();
	_fontTabSize = 0;
	_lineSpacing = 0;
	_textX = _textY = 0;
	_fgColor = _bgColor = 0;
	_overrideColor = 0;
	_maxCharWidth = 0;
	_maxCharCenter = 0;
	_fontSectionNum = -1;
	_fontFieldA = 0;
}

void Font::deinit() {
	delete _font2;
}

Font::Font(Screen *vm) {
	_counter = 0;
	_sectionNum = -1;
	_fontNumber = -1;
	_field4 = 0;
	_field5 = 0;
	_bytesPerLine = 0;
	_minPrintableChar = 0;
	_maxPrintableChar = 0;
	_linesOffset = 0;
	_linesPerChar = 0;
	_fixedWidth = 0;
	_fixedSpacing = 0;
	_fieldD = 0;
}

void Font::load(Common::SeekableReadStream &s) {
	_field4 = s.readByte();
	_field5 = s.readByte();
	_bytesPerLine = s.readByte();
	_minPrintableChar = s.readByte();
	_maxPrintableChar = s.readByte();
	_linesOffset = s.readByte();
	_linesPerChar = s.readByte();
	_fixedWidth = s.readSByte();
	_fixedSpacing = s.readSByte();
	_fieldD = s.readByte();

	_charWidths.clear();
	_charSpacings.clear();
	_pixelData.clear();

	// For fonts which aren't fixed width, load the widths for each character
	if (_fixedWidth < 0) {
		_charWidths.resize(0x80);
		s.read(&_charWidths[0], 0x80);
	}

	// For fonts without fixed spacing, load the spacing data
	if (_fixedSpacing < 0) {
		_charSpacings.resize(0x80);
		s.read(&_charSpacings[0], 0x80);
	}

	// Load the font pixel data
	_pixelData.resize((_maxPrintableChar - _minPrintableChar + 1)
		* _bytesPerLine * _linesPerChar);
	s.read(&_pixelData[0], _pixelData.size());
}

void Font::writeChar(char c) {
	assert(_surface);

	if (c == '\t') {
		_textX -= (_textX % _fontTabSize) - _fontTabSize;
		if (_textX < (_surface->w - 1))
			return;
	}
	if (c == '\n' || c == 't') {
		_textX = 0;
		_textY += _lineSpacing;
	}

	int charWidth = _fixedWidth ? _fixedWidth : _charWidths[c];
	int charFullWidth = charWidth + (_fixedSpacing >= 0 ? _fixedSpacing :
		(_charSpacings[c].leftSpacing + _charSpacings[c].rightSpacing));
	int var6 = _fieldA;

	if (_bgColor >= 0 && _overrideColor != -1) {
		_font2->fn1(0, _bgColor, 0);
	}

	if (_bgColor >= 0) {
		_surface->fillRect(Common::Rect(_textX, _textY,
			_textX + charFullWidth, _textY + _lineSpacing), _bgColor);
	}

	if (_fixedSpacing < 0) {
		_textX += _charSpacings[c].leftSpacing;
		charFullWidth -= _charSpacings[c].leftSpacing;
	}

	if (c >= _minPrintableChar && c <= _maxPrintableChar) {
		const byte *srcP = &_pixelData[(c - _minPrintableChar) * _linesPerChar * _bytesPerLine];
		
		for (int yCtr = 0, yp = _textY; yCtr < _linesPerChar; ++yCtr, ++yp) {
			byte *destP = (byte *)_surface->getBasePtr(_textX, yp);
			byte bitMask = 0, srcPixel = 0;
			for (int byteCtr = 0, xCtr = 0;  xCtr < charFullWidth; ++xCtr, ++destP, bitMask >>= 1) {
				if ((byteCtr % 8) == 0) {
					bitMask = 0x80;
					srcPixel = *srcP++;
				}

				if (srcPixel & bitMask)
					*destP = _fgColor;
			}
		}

		_textX += charFullWidth;
	}
}

void Font::writeString(const TextMessage &msg) {
	const char *msgP = msg;

	if (_field3) {

	}

	while (*msgP)
		writeChar(*msgP++);
}

uint Font::charWidth(char c) const {
	if (c == '\t')
		return _fontTabSize;
	else if (c == '\n')
		return 0;
	else if (_fixedSpacing < 0)
		return _charWidths[c] + _charSpacings[c].leftSpacing + _charSpacings[c].rightSpacing;
	else if (_fixedWidth < 0)
		return _charWidths[c] + _fixedSpacing;
	else
		return _fixedWidth + _fixedSpacing;
}

uint Font::stringWidth(const TextMessage &msg) const {
	const char *msgP = msg;
	uint total = 0;
	while (*msgP)
		total += charWidth(*msgP++);

	return total;
}

} // End of namespace Legend
