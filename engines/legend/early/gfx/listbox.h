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

#ifndef LEGEND_EARLY_LISTBOX_H
#define LEGEND_EARLY_LISTBOX_H

#include "legend/early/gfx/boxed_element.h"
#include "legend/gfx/picture.h"
#include "legend/gfx/visual_surface.h"

namespace Legend {
namespace Early {

enum ListboxRegion {
	LB_ITEMS = 0, LB_THUMBNAIL = 1, LB_SCROLLBAR = 2, LB_THUMB_UP = 3, LB_THUMB_DOWN = 4
};

#define ARROW_REPEAT_FRAMES 2

class Listbox : public Gfx::VisualItem {
	DECLARE_MESSAGE_MAP;
	bool ShowMsg(CShowMsg &msg);
	bool FrameMsg(CFrameMsg &msg);
	bool MouseWheelMsg(CMouseWheelMsg &msg);
	bool MouseButtonDownMsg(CMouseButtonDownMsg &msg);
	bool MouseButtonUpMsg(CMouseButtonUpMsg &msg);
private:
	StringArray _lines;
	int _topVisible;
	int _selectedIndex;
	int _xOffset;
	int _dividerIndex;
	bool _upPressed, _downPressed;
	uint _pressRepeatExpiry;
	Gfx::Picture *_thumbUp;
	Gfx::Picture *_thumbDown;
	Gfx::Picture *_thumbnail;
	Gfx::Picture *_thumbUpPressed;
	Gfx::Picture *_thumbDownPressed;
private:
	/**
	 * Common initialization method used by the constructors
	 */
	void init();

	/**
	 * Returns the number of visible rows of text the listbox can display
	 */
	uint numVisibleRows() {
		return (_bounds.height() - 10) / (getSurface().getFont()->_lineHeight + 2);
	}

	/**
	 * Returns true if scrolling is enabled
	 */
	bool isScrollingEnabled() {
		return _lines.size() > numVisibleRows();
	}

	/**
	 * Updates the thumbnail position
	 */
	void updateThumbnail();

	/**
	 * Handles drawing the scrollbar
	 */
	void drawScrollbar();

	/**
	 * Draws the list of items
	 */
	void drawItems();
public:
	CLASSDEF;
	Listbox() : Gfx::VisualItem() { init(); }
	Listbox(const Common::Rect &r) : Gfx::VisualItem(r) { init(); }
	Listbox(const Common::String &name) : Gfx::VisualItem(name) { init(); }
	Listbox(const Common::String &name, const Common::Rect &r) : 
		Gfx::VisualItem(name, r) { init(); }
	virtual ~Listbox();

	/**
	 * Loads a listbox with a list of strings
	 */
	void load(const StringArray &lines);

	/**
	 * Loads a listbox with a list of strings based on a resource
	 * containing string identifiers
	 */
	void load(const String &resName);

	/**
	 * Sets the divider line index, which is the dotted line separating
	 * commonly used words from the remainder of the items
	 */
	void setDividerIndex(int index) { _dividerIndex = index; }

	/**
	 * Draws the image on the screen
	 */
	virtual void draw();

	/**
	 * Changes the selected index by a given amount
	 */
	void deltaChange(int delta);
};

} // End of namespace Early
} // End of namespace Legend

#endif
