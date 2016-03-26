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
#include "legend/early/early_screen.h"
#include "legend/later/later_screen.h"

namespace Legend {

Screen *Screen::init(LegendEngine *vm) {
	if (vm->isLater())
		return new LaterScreen(vm);
	else
		return new EarlyScreen(vm);
}

Screen::Screen(LegendEngine *vm): _vm(vm) {
	Font::init();
}

Screen::~Screen() {
}

void Screen::addArea(ScreenArea *area) {
	_screenAreas.push_back(area);
}

void Screen::removeArea(ScreenArea *area) {
	_screenAreas.remove(area);
}

} // End of namespace Legend
