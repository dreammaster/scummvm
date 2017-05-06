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

#include "legend/later/screen.h"
#include "engines/util.h"
#include "legend/legend.h"

namespace Legend {
namespace Later {

Screen &Screen::get() {
	return *((Screen *)g_vm->_screen);
}

Screen::Screen(LegendEngine *vm) : Gfx::Screen(vm, 320, 200) {
	setDefaultPalette();
}

Screen::~Screen() {
}

void Screen::setDefaultPalette() {
	for (int idx = 0; idx < (16 * 3); ++idx) {
		_gamePalette[idx] = ((idx / 3) * 63) / 15;
	}
	_gamePalette[PALETTE_SIZE - 3] = 0;
	_gamePalette[PALETTE_SIZE - 2] = 0;
	_gamePalette[PALETTE_SIZE - 1] = 0;
	_gamePalette[15 * 3] = 0xff;
	_gamePalette[15 * 3 + 1] = 0xff;
	_gamePalette[15 * 3 + 2] = 0xff;

	setPalette(_gamePalette);
}

} // End of namespace Later
} // End of namespace Legend
