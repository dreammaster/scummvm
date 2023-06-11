/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LEGEND_EARLY_GFX_LISTBOX_H
#define LEGEND_EARLY_GFX_LISTBOX_H

#include "legend/early/gfx/boxed_element.h"
#include "legend/gfx/picture.h"
#include "legend/utils/string.h"

namespace Legend {
namespace Early {
namespace Gfx {

enum ListboxRegion {
	LB_ITEMS = 0, LB_THUMBNAIL = 1, LB_SCROLLBAR = 2, LB_THUMB_UP = 3, LB_THUMB_DOWN = 4
};

#define ARROW_REPEAT_FRAMES 2
#define LISTBOX_ITEMS_X 7
#define LISTBOX_ITEMS_Y 10

class Listbox : public BoxedElement {
private:
	StringArray _lines;
	int _topVisible;
	int _selectedIndex;
	int _xOffset;
	int _dividerIndex;
	bool _upPressed, _downPressed;
	uint _pressRepeatExpiry;
	bool _draggingThumbnail;
	int _dragOffset;
	bool _itemSelected;
	Legend::Gfx::Picture *_thumbUp;
	Legend::Gfx::Picture *_thumbDown;
	Legend::Gfx::Picture *_thumbnail;
	Legend::Gfx::Picture *_thumbUpPressed;
	Legend::Gfx::Picture *_thumbDownPressed;
private:
	/**
	 * Common initialization method used by the constructors
	 */
	void init();

	/**
	 * Returns the line height of items in the listbox
	 */
	uint itemsLineHeight() const;

	/**
	 * Returns the number of visible rows of text the listbox can display
	 */
	uint numVisibleRows() {
		return (_bounds.height() - LISTBOX_ITEMS_Y) / itemsLineHeight();
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
	Listbox(UIElement *parent) : BoxedElement(parent) {
		init();
	}
	Listbox(UIElement *parent, const Common::Rect &r) : BoxedElement(parent, r) {
		init();
	}
	Listbox(UIElement *parent, const String &name) : BoxedElement(parent, name) {
		init();
	}
	Listbox(UIElement *parent, const String &name, const Common::Rect &r) :
		BoxedElement(parent, name, r) {
		init();
	}
	~Listbox() override;

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
	void setDividerIndex(int index) {
		_dividerIndex = index;
	}

	/**
	 * Draws the image on the screen
	 */
	virtual void draw();

	/**
	 * Changes the selected index by a given amount
	 */
	void deltaChange(int delta);

	bool msgFocus(const FocusMessage &msg) override;
	bool tick() override;
	bool msgMouseWheel(const MouseWheelMessage &msg) override;
	bool msgMouseDown(const MouseDownMessage &msg) override;
	bool msgMouseUp(const MouseUpMessage &msg) override;
	bool msgMouseDrag(const MouseDragMessage &msg) override;
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
