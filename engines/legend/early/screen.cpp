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
#include "engines/util.h"
#include "graphics/palette.h"
#include "legend/early/screen.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {

static const byte INITIAL_LOW_PALETTE[16] = {
	0x3F, 0x07, 0x38, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x00
};

Screen &Screen::get() {
	return *((Screen *)g_vm->_screen);
}

Screen::Screen(LegendEngine *vm): Gfx::Screen(vm), _sectionType(TT_2) {
	Gfx::Font::init();
	Common::copy(&INITIAL_LOW_PALETTE[0], &INITIAL_LOW_PALETTE[16], _palette);
	defaultPalette();
}

void Screen::setupGraphics() {
	initGraphics(640, 480);
}

Screen::~Screen() {
}

void Screen::transition(int index) {
	switch (_sectionType) {
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

void Screen::setEGAPalette(const byte *palette) {
	byte tempPalette[PALETTE_SIZE];
	const byte *srcP = palette;
	byte *destP = &tempPalette[0];

	for (int idx = 0;  idx < PALETTE_COUNT; ++idx, ++srcP, destP += 3) {
		destP[0] = (*srcP >> 2 & 1) * 0xaa + (*srcP >> 5 & 1) * 0x55;
		destP[1] = (*srcP >> 1 & 1) * 0xaa + (*srcP >> 4 & 1) * 0x55;
		destP[2] = (*srcP & 1) * 0xaa + (*srcP >> 3 & 1) * 0x55;
	}

	g_system->getPaletteManager()->setPalette(tempPalette, 0, PALETTE_COUNT);
}

void Screen::defaultPalette() {
	int index;

	switch (_sectionType) {
	case 1:
		_palette[0] = 63;
		_palette[1] = 7;
		_palette[2] = 56;
		_palette[15] = 0;
		break;
	case 2:
	case 3:
	case 4:
		// White color
		index = _sectionType == TT_3 ? PALETTE_SIZE - 3 : 0;
		Common::fill(&_palette[index], &_palette[index + 3], 63);

		// Black color
		index = _sectionType == TT_3 ? 0 : 45;
		Common::fill(&_palette[index], &_palette[index + 3], 0);

		// Miscellaneous
		_palette[3] = _palette[4] = _palette[5] = 42;
		_palette[6] = _palette[7] = _palette[8] = 21;
		break;

	default:
		break;
	}

	setEGAPalette(_palette);
}

} // End of namespace Early
} // End of namespace Legend
