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

#ifndef LEGEND_VISUAL_ITEM_H
#define LEGEND_VISUAL_ITEM_H

#include "graphics/managed_surface.h"
#include "legend/core/named_item.h"
#include "legend/gfx/screen.h"

namespace Legend {
namespace Gfx {

/**
 * Acts as the base class for all classes that have a visual representation on the screen
 */
class VisualItem : public NamedItem {
	DECLARE_MESSAGE_MAP;
	bool ShowMsg(CShowMsg &msg);
	bool HideMsg(CHideMsg &msg);
private:
	/**
	 * Common initialization method used by the constructors
	 */
	void init();
protected:
	Common::Rect _bounds;
	bool _isDirty;
	int _fontNumber;
	Common::Point _fontCenter;
	int _fontHorizSpacings;
	int _fontDetailsFlags1;
	Common::Point _fontWritePos;
public:
	CLASSDEF;
	VisualItem() { init(); }
	VisualItem(const Common::Rect &r) : _bounds(r) { init(); }
	VisualItem(const Common::String &name) : NamedItem(name) { init(); }
	VisualItem(const Common::String &name, const Common::Rect &r) : NamedItem(name),
		_bounds(r) { init(); }

	virtual ~VisualItem() {}

	/**
	 * Draws the visual item on the screen
	 */
	virtual void draw() { _isDirty = false; }

	/**
	 * Gets a managed surface representing the portion of the screen defined by _bounds.
	 * This allows drawing to be done without worrying about offsets or clipping
	 */
	Graphics::ManagedSurface getSurface() const;

	/**
	 * Gets the bounds for the item
	 */
	const Common::Rect &getBounds() { return _bounds; }

	/**
	 * Sets the bounds for the item
	 * @remarks Does not erase the item if it's already been drawn
	 */
	void setBounds(const Common::Rect &r);

	/**
	 * Set the position for the item
	 * @remarks Does not erase the item if it's already been drawn
	 */
	void setPosition(const Common::Point &pt);

	/**
	 * Flags the item as being changed, requiring a full redraw
	 */
	void setDirty();

	/**
	 * Helper function to switch to a different visual item
	 */
	void changeView(const Common::String &name);

	/**
	 * Loads a font to use for the item
	 */
	Font *loadFont(int fontNumber);

	/**
	 * Set the position for writing text in screen-coordinates
	 * @remarks		All text is clipped to the bounds of the visual item
	 */
	void setTextPos(const Common::Point &pt) {
		_fontWritePos = Common::Point(pt.x - _bounds.left, pt.y - _bounds.top);
	}

	/**
	 * Sets the postion or writing text relative to the top-left corner of the visual item's bounds
	 */
	void setRelativeTextPos(const Common::Point &pt) { _fontWritePos = pt; }

	/**
	 * Set the font color
	 */
	void setFontColor(int fgColor, int bgColor = -1);

	/**
	 * Writes a string
	 */
	void writeString(const Common::String &msg);
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
