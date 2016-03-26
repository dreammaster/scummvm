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

#include "common/file.h"
#include "common/system.h"
#include "graphics/palette.h"
#include "legend/early/early_screen.h"
#include "legend/legend.h"

namespace Legend {

static const byte INITIAL_PALETTE[16] = {
	0x3F, 0x07, 0x38, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x00
};

EarlyScreen::EarlyScreen(LegendEngine *vm): Screen(vm), _font0(this), _font1(this),
		_activeFont(nullptr), _transitionType(TT_INVALID) {
	Font::init();
	Common::copy(&INITIAL_PALETTE[0], &INITIAL_PALETTE[16], _palette);
}

EarlyScreen::~EarlyScreen() {
}

Font *EarlyScreen::loadFont(int fontNumber) {
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

void EarlyScreen::transition(int index) {
	switch (_transitionType) {
	case TT_1:
		_palette[index] = 36;
		setEGAPalette(_palette);
		break;
	case TT_2:
	case TT_4:
		for (int idx = 0; idx < 64; ++idx) {
			_palette[index * 3] = idx;
			setEGAPalette(_palette);
		}
		break;
	default:
		break;
	}
}

void EarlyScreen::setEGAPalette(const byte *palette) {
	byte tempPalette[16 * 3];
	const byte *srcP = palette;
	byte *destP = &tempPalette[0];

	for (int idx = 0;  idx < 16; ++idx, ++srcP, destP += 3) {
		destP[0] = (*srcP >> 2 & 1) * 0xaa + (*srcP >> 5 & 1) * 0x55;
		destP[1] = (*srcP >> 1 & 1) * 0xaa + (*srcP >> 4 & 1) * 0x55;
		destP[2] = (*srcP & 1) * 0xaa + (*srcP >> 3 & 1) * 0x55;
	}

	g_system->getPaletteManager()->setPalette(tempPalette, 0, 16);
}

} // End of namespace Legend
