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

static const byte INITIAL_LOW_PALETTE[16 * 3] = {
	0xff, 0xff, 0xff, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55, 0xaa, 0x55,
	0x55, 0xaa, 0x55, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0xaa, 0x55, 0xaa,
	0xaa, 0x55, 0xaa, 0x55, 0x55, 0xff, 0x55, 0xff, 0x55, 0x55, 0xff, 0xff
};

Screen &Screen::get() {
	return *((Screen *)g_vm->_screen);
}

Screen::Screen(LegendEngine *vm) : Gfx::Screen(vm, 640, 480) {
	Gfx::Font::init();

	Common::copy(&INITIAL_LOW_PALETTE[0], &INITIAL_LOW_PALETTE[16 * 3], _gamePalette);
	Common::fill(&_gamePalette[16 * 3], &_gamePalette[PALETTE_SIZE], 0);
	setPalette();

	_defaultTextColor = BLACK;
}

Screen::~Screen() {
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

	setGamePalette(tempPalette);
}

void Screen::cyclePaleteEntry(int index) {
	byte *pPal = &_gamePalette[index * 3];

	for (int intensity = 0; intensity < 64; ++intensity) {
		for (int idx = 0; idx < 3; ++idx)
			pPal[idx] = (intensity == 63) ? 255 : intensity * 4;

		setPalette();
		update();
		g_system->delayMillis(20);
	}
}

} // End of namespace Early
} // End of namespace Legend
