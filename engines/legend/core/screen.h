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

#ifndef LEGEND_SCREEN_H
#define LEGEND_SCREEN_H

#include "legend/core/font.h"
#include "common/list.h"
#include "common/rect.h"
#include "graphics/screen.h"
#include "legend/messages.h"
#include "legend/core/screen_area.h"

namespace Legend {

class LegendEngine;
class ScreenArea;

class Screen: public Graphics::Screen {
	friend class Message;
private:
	Common::List<ScreenArea *> _screenAreas;
protected:
	LegendEngine *_vm;
	byte _gamePalette[PALETTE_SIZE];
public:
	bool _paletteDirty;
	bool _picPalette;
public:
	Screen(LegendEngine *vm);
	virtual ~Screen();
	static Screen *init(LegendEngine *vm);

	/**
	 * Adds a visual element from the screen
	 */
	void addArea(ScreenArea *area);

	/**
	 * Removes a visual element from the screen
	 */
	void removeArea(ScreenArea *area);

	/**
	 * Sets the game palette
	 * @param palette	Palette to set
	 * @param from6Bit	If set, the passed palette is 6 bit per pixel,
	 * and will be automatically converted to 8-bit
	 */
	void setGamePalette(byte palette[PALETTE_SIZE], bool from6Bit = true);

	/**
	 * Reset the palette to blank
	 */
	void resetPalette();

	/**
	 * Set the palette
	 */
	void setPalette(const byte palette[PALETTE_SIZE]);

	/**
	 * Set a subsection of the palette
	 */
	void setPalette(const byte *palette, uint start, uint num);

	/**
	 * Set the palette to the currently loaded game palette
	 */
	void setPalette() { setPalette(_gamePalette); }
};

} // End of namespace Legend

#endif
