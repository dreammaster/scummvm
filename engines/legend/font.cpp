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

#include "legend/screen.h"
#include "legend/legend.h"
#include "common/system.h"
#include "common/util.h"
#include "graphics/palette.h"

namespace Legend {

int Font::_fontTabSize;
uint Font::_lineSpacing;
uint Font::_lineSpacingCenter;
int Font::_textX;
int Font::_textY;
int Font::_fgColor;
int Font::_bgColor;
uint Font::_maxCharWidth;
uint Font::_maxCharCenter;
int Font::_fontSectionNum;
int Font::_fontFieldA;

void Font::init() {
	_fontTabSize = 0;
	_lineSpacing = 0;
	_textX = _textY = 0;
	_fgColor = _bgColor = 0;
	_maxCharWidth = 0;
	_maxCharCenter = 0;
	_fontSectionNum = -1;
	_fontFieldA = 0;
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
	if (c == '\t') {
		_textX = (_textX / 8 + 1) * 8;
	}
	if (c == '\n' || (c == '\t' && _textX >= _screen->w)) {
		_textX = 0;
		_textY += _lineSpacing;
		return;
	}

	int charWidth = _fixedWidth ? _fixedWidth : _charWidths[c];
	int charFullWidth = charWidth + (_fixedSpacing >= 0 ? _fixedSpacing :
		(_charSpacings[c].leftSpacing + _charSpacings[c].rightSpacing));

	if (_bgColor >= 0) {
		_screen->fillRect(Common::Rect(_textX, _textY,
			_textX + charFullWidth, _textY + _lineSpacing), _bgColor);
	}

	if (_fixedSpacing < 0) {
		_textX += _charSpacings[c].leftSpacing;
		charFullWidth -= _charSpacings[c].leftSpacing;
	}

	if (c >= _minPrintableChar && c <= _maxPrintableChar) {
		const byte *srcP = &_pixelData[(c - _minPrintableChar) * _linesPerChar * _bytesPerLine];
		
		for (int yCtr = 0, yp = _textY; yCtr < _linesPerChar; ++yCtr, ++yp) {
			byte *destP = (byte *)_screen->getBasePtr(_textX, yp);
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
