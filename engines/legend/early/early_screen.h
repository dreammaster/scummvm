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

#ifndef LEGEND_EARLY_SCREEN_H
#define LEGEND_EARLY_SCREEN_H

#include "common/list.h"
#include "common/rect.h"
#include "legend/gfx/font.h"
#include "legend/gfx/screen.h"

namespace Legend {
namespace Early {

enum TransitionType {
	TT_INVALID = -1, TT_NONE = 0, TT_1 = 1, TT_2 = 2, TT_3 = 3, TT_4 = 4
};

class EarlyScreen: public Screen {
private:
	byte _palette[64];
public:
	TransitionType _transitionType;
public:
	EarlyScreen(LegendEngine *vm);
	virtual ~EarlyScreen();

	/**
	 * Initialises graphics mode
	 */
	virtual void setupGraphics();

	/**
	 * Handle a palette transition
	 */
	void transition(int index);

	/**
	 * Set an EGA palette
	 */
	void setEGAPalette(const byte *palette);
};

} // End of namespace Early
} // End of namespace Legend

#endif
