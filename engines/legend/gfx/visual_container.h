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

#ifndef LEGEND_VISUAL_CONTAINER_H
#define LEGEND_VISUAL_CONTAINER_H

#include "legend/gfx/image.h"

namespace Legend {
namespace Gfx {

class VisualContainer : public Image {
	DECLARE_MESSAGE_MAP;
	bool MouseButtonDownMsg(CMouseButtonDownMsg *msg);
	bool MouseButtonUpMsg(CMouseButtonUpMsg *msg);
	bool MouseMoveMsg(CMouseMoveMsg *msg);
	bool MouseDoubleClickMsg(CMouseDoubleClickMsg *msg);
	bool MouseWheelMsg(CMouseWheelMsg *msg);
private:
	VisualItem *_mouseFocusItem;
private:
	/**
	 * Called to handle mouse messagaes on the view
	 */
	VisualItem *handleMouseMsg(CMouseMsg *msg);
public:
	CLASSDEF;
	VisualContainer() : Image(), _mouseFocusItem(nullptr) {}
	VisualContainer(const Common::Rect &r) : Image(r), _mouseFocusItem(nullptr) {}
	VisualContainer(const String &name) : Image(name), _mouseFocusItem(nullptr) {}
	VisualContainer(const String &name, const Common::Rect &r) : Image(name, r),
		_mouseFocusItem(nullptr) {}
	virtual ~VisualContainer() {}

	/**
	 * Draws the container by iterating through each child and letting it draw
	 */
	virtual void draw();
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
