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

#ifndef LEGEND_EARLY_GFX_SCREEN_H
#define LEGEND_EARLY_GFX_SCREEN_H

#include "common/list.h"
#include "common/rect.h"
#include "legend/gfx/font.h"

namespace Legend {
namespace Early {
namespace Gfx {

enum {
	WHITE = 0,
	LIGHT_GRAY = 1,
	DARK_GRAY = 2,
	BLACK = 15
};

class Screen {
private:
	/**
	 * Set an EGA palette
	 */
	static void setEGAPalette(byte *destPalette);

public:
	static const int TEXT_ROWS, TEXT_COLUMNS;
public:
	/**
	 * Sets the EGA palette
	 */
	static void setPalette();
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
