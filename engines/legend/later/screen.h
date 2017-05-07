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

#ifndef LEGEND_LATER_SCREEN_H
#define LEGEND_LATER_SCREEN_H

#include "legend/gfx/screen.h"

namespace Legend {
namespace Later {

class Screen : public Gfx::Screen {
private:
	byte _subPalette1[16 * 3];
	byte _subPalette2[48 * 3];
public:
	Screen(LegendEngine *vm);
	virtual ~Screen();
	static Screen &get();

	/**
	 * Sets a default game palette
	 */
	void setDefaultPalette();

	/**
	 * Takes a section of the game palette loaded from a specified picture,
	 * for use in later use in restoring after loading images to keep these sections unchanged
	 */
	void saveSubPalette1(int picNumber = 31);

	/**
	 * Takes a section of the game palette loaded from a specified picture,
	 * for use in later use in restoring after loading images to keep these sections unchanged
	 */
	void saveSubPalette2(int picNumber = 256);

	/**
	 * Restores previously saved palette sub-sections into the game palette
	 */
	void restoreSubPalettes();
};

} // End of namespace Later
} // End of namespace Legend

#endif
