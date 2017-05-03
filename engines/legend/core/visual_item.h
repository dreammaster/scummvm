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
#include "legend/core/screen.h"

namespace Legend {

/**
 * Acts as the base class for all classes that have a visual representation on the screen
 */
class VisualItem : public NamedItem {
	DECLARE_MESSAGE_MAP;
public:
	Common::Rect _bounds;
public:
	CLASSDEF;
	virtual ~VisualItem() {}

	/**
	 * Draws the visual item on the screen
	 * @remarks		Redrawing only necessary if the state of the item has changed
	 */
	virtual void draw() = 0;

	/**
	 * Gets a managed surface representing the portion of the screen defined by _bounds.
	 * This allows drawing to be done without worrying about offsets or clipping
	 */
	Graphics::ManagedSurface getSurface() const;

	/**
	 * Returns a reference to the screen
	 */
	Screen &screen() const;
};

} // End of namespace Legend

#endif /* LEGEND_VISUAL_ITEM_H */
