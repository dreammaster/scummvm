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
#include "common/file.h"
#include "common/util.h"
#include "graphics/palette.h"

namespace Legend {

Screen::Screen(LegendEngine *vm): _vm(vm), _font0(this), _font1(this), 
		_activeFont(nullptr) {
	Font::init();
}

Screen::~Screen() {
}

Font *Screen::loadFont(int fontNumber) {
	Font *fontList[2] = { &_font0, &_font1 };
	int idx;
	Common::File f;

	for (idx = 0; idx < 2; ++idx) {
		if (fontList[idx]->_counter)
			fontList[idx]->_counter++;
	}

	for (idx = 0; idx < 2; ++idx) {
		if (fontList[idx]->_sectionNum == Font::_fontSectionNum && fontList[idx]->_fontNumber == fontNumber)
			break;
	}

	if (idx < 2) {
		// Use existing loaded font
		_activeFont = fontList[idx];
		_activeFont->_counter = 1;
	} else {
		_activeFont = fontList[idx];
		if (fontNumber)
			f.open(_vm->_res->getFilename(FILETYPE_FNT,
				Font::_fontSectionNum * 100 + fontNumber));

		_activeFont->_counter = 1;
		_activeFont->_sectionNum = Font::_fontSectionNum;
		_activeFont->_fontNumber = fontNumber;

		// Original used font 0 for text mode font, which we don't need
		assert(fontNumber && f.isOpen());
		_activeFont->load(f);
	}

	// Final font setup
	Font::_maxCharWidth = _activeFont->charWidth('M');
	Font::_lineSpacing = _activeFont->getLineSpacing();
	Font::_maxCharCenter = Font::_maxCharWidth / 2;

	return _activeFont;
}

} // End of namespace Legend
