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
#include "graphics/screen.h"
#include "legend/messages.h"
#include "legend/screen_area.h"

namespace Legend {

enum TransitionType {
	TT_INVALID = -1, TT_NONE = 0, TT_1 = 1, TT_2 = 2, TT_3 = 3, TT_4 = 4
};

class LegendEngine;
class ScreenArea;

class Screen: public Graphics::Screen {
	friend class Message;
private:
	Common::List<ScreenArea *> _screenAreas;
	Font _font0;
	Font _font1;
	byte _palette[64];
protected:
	LegendEngine *_vm;
public:
	Font *_activeFont;
	TransitionType _transitionType;
public:
	Screen(LegendEngine *vm);
	virtual ~Screen();

	/**
	 * Adds a visual element from the screen
	 */
	void addArea(ScreenArea *area);

	/**
	* Removes a visual element from the screen
	*/
	void removeArea(ScreenArea *area);

	/**
	 * Load a specified font number
	 */
	Font *loadFont(int fontNumber);

	/**
	 * Handle a palette transition
	 */
	void transition(int index);

	/**
	 * Set an EGA palette
	 */
	void setEGAPalette(const byte *palette);
};

} // End of namespace Legend

#endif
