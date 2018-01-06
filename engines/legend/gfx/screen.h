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

#include "legend/gfx/font.h"
#include "common/list.h"
#include "common/rect.h"
#include "graphics/screen.h"
#include "legend/messages.h"

namespace Legend {

class LegendEngine;

namespace Gfx {

class Screen: public Graphics::Screen {
protected:
	LegendEngine *_vm;
	byte _gamePalette[PALETTE_SIZE];
public:
	bool _paletteReset;
	bool _picPalette;
public:
	Screen(LegendEngine *vm, int width, int height);
	virtual ~Screen();
	static Screen *init(LegendEngine *vm);
	static Screen &get();

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
	 * Handles updating the screen palete after a picture loading has set a new palette
	 */
	void checkPalette();

	/**
	 * Set the palette to the currently loaded game palette
	 */
	void setPalette() { setPalette(_gamePalette); }

	/**
	 * Fades in to the palette in _gamePalette
	 */
	void fadeIn();

	/**
	 * Fades out to black
	 */
	void fadeOut();

	/**
	 * Loads a font
	 */
	Font *loadFont(int fontNumber) { return Font::loadFont(fontNumber); }
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
