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

#ifndef LEGEND_EARLY_COMMSET_H
#define LEGEND_EARLY_COMMSET_H

#include "legend/gfx/visual_container.h"
#include "legend/gfx/picture.h"
#include "legend/early/gfx/button.h"

namespace Legend {
namespace Early {

/**
 * Implements the contents area of the Commset
 */
class CommsetContents : public Gfx::VisualItem {
	DECLARE_MESSAGE_MAP;
public:
	CLASSDEF;
	CommsetContents(const Common::Rect &r) : Gfx::VisualItem(r) {}
	virtual ~CommsetContents() {}

	/**
	 * Draws the visual item on the screen
	 */
	virtual void draw();
};

/**
 * Implements the Commset interface
 */
class Commset : public Gfx::VisualContainer {
	DECLARE_MESSAGE_MAP;
	bool ShowMsg(CShowMsg &msg);
	bool HideMsg(CHideMsg &msg);
	bool MouseButtonDownMsg(CMouseButtonDownMsg &msg);
	bool KeyCharMsg(CKeyCharMsg &msg);
private:
	CommsetContents _contents;
	Gfx::Picture *_logo;
	ValueButton _btn1, _btn2, _btn3, _btn4, _btn5, _btn6;
	ValueButton _btn7, _btn8, _btn9, _btnPrev, _btn0, _btnNext;
private:
	/**
	 * Draws the background for the Commset
	 */
	void drawBackground();
public:
	CLASSDEF;
	Commset();
	virtual ~Commset();

	/**
	 * Draws the visual item on the screen
	 */
	virtual void draw();
};

} // End of namespace Early
} // End of namespace Legend

#endif
