/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LEGEND_GFX_SCREEN_H
#define LEGEND_GFX_SCREEN_H

#include "graphics/screen.h"

namespace Legend {
namespace Gfx {

/**
 * Base class for the overall screen
 */
class Screen : public Graphics::Screen {
public:
	Screen() : Graphics::Screen() {}
	Screen(int w, int h) : Graphics::Screen(w, h) {}

	/**
	 * Reset the palette back to default
	 */
	virtual void resetPalette() = 0;
};

} // namespace Gfx
} // namespace Legend

#endif
