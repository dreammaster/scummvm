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

#include "engines/util.h"
#include "legend/gfx/screen.h"
#include "legend/legend.h"
#include "legend/early/screen.h"
#include "legend/later/screen.h"

namespace Legend {
namespace Gfx {

#define VGA_COLOR_TRANS(x) ((x) * 255 / 63)

Screen *Screen::init(LegendEngine *vm) {
	if (vm->isLater())
		return new Later::Screen(vm);
	else
		return new Early::Screen(vm);
}

Screen &Screen::get() {
	return *((Screen *)g_vm->_screen);
}

Screen::Screen(LegendEngine *vm, int width, int height): Graphics::Screen(width, height),
		_vm(vm), _paletteDirty(false), _picPalette(false) {
	Font::init();
	Common::fill(&_gamePalette[0], &_gamePalette[PALETTE_SIZE], 0);
	initGraphics(width, height);
}

Screen::~Screen() {
}

void Screen::setGamePalette(byte palette[PALETTE_SIZE], bool from6Bit) {
	if (from6Bit) {
		for (int idx = 0; idx < PALETTE_SIZE; ++idx)
			_gamePalette[idx] = VGA_COLOR_TRANS(palette[idx]);
	} else {
		// Palette already in 8-bit format, so simply copy it
		Common::copy(palette, palette + PALETTE_SIZE, _gamePalette);
	}
}

void Screen::resetPalette() {
	byte pal[PALETTE_SIZE];
	Common::fill(&pal[0], &pal[PALETTE_SIZE], 0);
	setPalette(pal);
}

void Screen::setPalette(const byte palette[PALETTE_SIZE]) {
	Graphics::Screen::setPalette(palette);
	_paletteDirty = false;
}

void Screen::setPalette(const byte *palette, uint start, uint num) {
	Graphics::Screen::setPalette(palette, start, num);
	_paletteDirty = false;
}

} // End of namespace Gfx
} // End of namespace Legend
