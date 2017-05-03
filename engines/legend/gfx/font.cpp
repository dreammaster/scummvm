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

#include "legend/gfx/font.h"
#include "legend/legend.h"
#include "common/system.h"
#include "common/util.h"
#include "graphics/palette.h"

namespace Legend {

Font *Font::_fonts[FONT_COUNT];
Font *Font::_activeFont;
int Font::_currentSection;
int Font::_tabWidth;
uint Font::_lineHeight;
uint Font::_xCenter;
uint Font::_yCenter;
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
	for (int idx = 0; idx < FONT_COUNT; ++idx)
		_fonts[idx] = new Font();
	_currentSection = 0;
	_tabWidth = 0;
	_lineHeight = 0;
	_textX = _textY = 0;
	_fgColor = _bgColor = 0;
	_overrideColor = 0;
	_maxCharWidth = 0;
	_maxCharCenter = 0;
	_fontSectionNum = -1;
	_fontFieldA = 0;
}

void Font::deinit() {
	for (int idx = 0; idx < FONT_COUNT; ++idx)
		delete _fonts[idx];
}

Font::Font() {
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

Font *Font::loadFont(int fontNumber) {
	assert(fontNumber > 0);

	for (int idx = 0; idx < FONT_COUNT; ++idx) {
		if (_fonts[idx]->_counter)
			_fonts[idx]->_counter++;
	}

	// Scan for the presense of the font already
	for (int idx = 0; idx < FONT_COUNT; ++idx) {
		if (_fonts[idx]->_sectionNum == _currentSection && 
				_fonts[idx]->_fontNumber == fontNumber) {
			_fonts[idx]->setActive();
			return _fonts[idx];
		}
	}

	// Not yet present, so select a slot to load it in
	Font *font = getFreeSlot();

	File f;
	if (fontNumber)
		f.open(FILETYPE_FNT, Font::_currentSection * 100 + fontNumber);

	font->_counter = 1;
	font->_sectionNum = Font::_currentSection;
	font->_fontNumber = fontNumber;

	font->load(f);

	// Set the font as active and return it
	font->setActive();
	return font;
}

void Font::getColor(int &fgColor, int &bgColor) {
	fgColor = _fgColor;
	bgColor = _bgColor;
}

void Font::setColor(int fgColor, int bgColor) {
	_fgColor = fgColor;
	_bgColor = bgColor;
}

void Font::setOverrideColor(int color) {
	_overrideColor = color;
}

void Font::setTextPos(const Common::Point &pt) {
	_textX = pt.x;
	_textY = pt.y;
}

Font *Font::getFreeSlot() {
	// Scan for the font slot with the lowest couner tally
	int minIndex = 0;

	for (int idx = 0; idx < (FONT_COUNT - 1) && _fonts[idx]->_counter; ++idx) {
		if (_fonts[idx]->_counter < _fonts[minIndex]->_counter) {
			minIndex = idx;			
		}
	}

	// Delete the old font in the slot, if any, and add a new instance
	delete _fonts[minIndex];
	_fonts[minIndex] = new Font();
	return _fonts[minIndex];
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
	Font *font = _activeFont;
	assert(_surface && font);

	if (c == '\t') {
		_textX -= (_textX % _tabWidth) - _tabWidth;
		if (_textX < (_surface->w - 1))
			return;
	}
	if (c == '\n' || c == '\t') {
		_textX = 0;
		_textY += _lineHeight;
		return;
	}

	int charWidth = _fixedWidth ? _fixedWidth : _charWidths[c];
	int charFullWidth = charWidth + (_fixedSpacing >= 0 ? _fixedSpacing :
		(_charSpacings[c].leftSpacing + _charSpacings[c].rightSpacing));
	int charHeight = _lineHeight;

	if (_bgColor >= 0 && _overrideColor != -1) {
		g_vm->_gfx->fn1(0, _bgColor, 0);
		g_vm->_gfx->eraseRect(Common::Rect(_textX, _textY, _textX + charFullWidth,
			_textY + charHeight), 2);
	}

	//if (_bgColor >= 0) {
	//	_surface->fillRect(Common::Rect(_textX, _textY,
	//		_textX + charFullWidth, _textY + _lineHeight), _bgColor);
	//}

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
	}

	_textX += charFullWidth;
	
	if (_overrideColor >= 0) {
		++_textX;
	} else {
		_textX = 0;
		_textY += _lineHeight;
	}
}

void Font::writeString(const Common::String &msg) {
	const char *msgP = msg.c_str();
	assert(_activeFont && _activeFont->_fontNumber > 0);

	while (*msgP)
		writeChar(*msgP++);
}

uint Font::charWidth(char c) const {
	if (c == '\t')
		return _tabWidth;
	else if (c == '\n')
		return 0;
	else if (_fixedSpacing < 0)
		return _charWidths[c] + _charSpacings[c].leftSpacing + _charSpacings[c].rightSpacing;
	else if (_fixedWidth < 0)
		return _charWidths[c] + _fixedSpacing;
	else
		return _fixedWidth + _fixedSpacing;
}

uint Font::stringWidth(const Common::String &msg) const {
	const char *msgP = msg.c_str();
	uint total = 0;
	while (*msgP)
		total += charWidth(*msgP++);

	return total;
}

void Font::setActive() {
	_counter = 1;
	_maxCharWidth = charWidth('M');
	_lineHeight = lineHeight();
	_xCenter = _maxCharWidth / 2;
	_yCenter = _lineHeight / 2;
}

int Font::setTabWidth(int width) {
	int oldWidth = _tabWidth;
	_tabWidth = width;
	return oldWidth;
}

} // End of namespace Legend
