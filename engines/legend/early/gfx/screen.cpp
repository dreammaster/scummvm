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
#include "legend/early/gfx/screen.h"
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

Screen::Screen(LegendEngine *vm): Gfx::Screen(vm, 640, 480) {
	Gfx::Font::init();

	Common::copy(&INITIAL_LOW_PALETTE[0], &INITIAL_LOW_PALETTE[16], _palette);
	Common::fill(&_palette[16], &_palette[PALETTE_SIZE], 0);
	defaultPalette();

	_defaultTextColor = BLACK;
}

Screen::~Screen() {
}

void Screen::transition(int index) {
	for (int idx = 0; idx < 64; ++idx) {
		_palette[index * 3] = idx;
		setEGAPalette(_palette);
	}
}

void Screen::setEGAPalette(const byte *palette) {
	const byte *srcP = palette;
	byte *destP = &_gamePalette[0];

	for (int idx = 0;  idx < PALETTE_COUNT; ++idx, ++srcP, destP += 3) {
		destP[0] = (*srcP >> 2 & 1) * 0xaa + (*srcP >> 5 & 1) * 0x55;
		destP[1] = (*srcP >> 1 & 1) * 0xaa + (*srcP >> 4 & 1) * 0x55;
		destP[2] = (*srcP & 1) * 0xaa + (*srcP >> 3 & 1) * 0x55;
	}

	g_system->getPaletteManager()->setPalette(_gamePalette, 0, PALETTE_COUNT);
}

void Screen::defaultPalette() {
	// Set default palette entries
	Common::fill(&_palette[WHITE * 3], &_palette[WHITE * 3], 63);
	Common::fill(&_palette[BLACK * 3], &_palette[BLACK * 3 + 3], 0);
	Common::fill(&_palette[LIGHT_GRAY * 3], &_palette[LIGHT_GRAY * 3 + 3], 42);
	Common::fill(&_palette[DARK_GRAY * 3], &_palette[DARK_GRAY * 3 + 3], 21);

	setEGAPalette(_palette);
}

} // End of namespace Early
} // End of namespace Legend
