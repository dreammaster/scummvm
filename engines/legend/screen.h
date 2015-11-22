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

#include "legend/font.h"
#include "common/list.h"
#include "common/rect.h"
#include "graphics/surface.h"

namespace Legend {

#define PALETTE_SIZE 768
#define PALETTE_COUNT 256
#define VGA_COLOR_TRANS(x) ((x) * 255 / 63)

class LegendEngine;

class Screen {
private:
	Common::List<Common::Rect> _dirtyRects;
	Graphics::Surface _screenSurface;
	Font _font0;
	Font _font1;
private:
	/**
	 * Merges together overlapping dirty areas of the screen
	 */
	void mergeDirtyRects();

	/**
	 * Returns the union of two dirty area rectangles
	 */
	bool unionRectangle(Common::Rect &destRect, const Common::Rect &src1, const Common::Rect &src2);
protected:
	LegendEngine *_vm;

	/**
	 * Clear the current dirty rects list
	 */
	void clearDirtyRects() { _dirtyRects.clear(); }

	/**
	 * Adds a rectangle to the list of modified areas of the screen during the
	 * current frame
	 */
	void addDirtyRect(const Common::Rect &r);
public:
	Font *_activeFont;
public:
	Screen(LegendEngine *vm);
	virtual ~Screen();

	int w() const { return _screenSurface.w; }
	int h() const { return _screenSurface.h; }
	byte *getBasePtr(int16 x, int16 y) { 
		return (byte *)_screenSurface.getBasePtr(x, y);
	}

	/**
	 * Fill an area of the screen
	 */
	void fillRect(const Common::Rect &r, int color);

	/**
	 * Handles updating any dirty areas of the screen Surface object to the physical screen
	 */
	void update();

	/**
	 * Makes the whole screen dirty
	 */
	void makeAllDirty();

	/**
	 * Load a specified font number
	 */
	Font *loadFont(int fontNumber);
};

} // End of namespace Legend

#endif
