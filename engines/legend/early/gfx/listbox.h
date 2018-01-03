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

#include "legend/gfx/visual_item.h"

namespace Legend {
namespace Early {

class Listbox : public Gfx::VisualItem {
	DECLARE_MESSAGE_MAP;
private:
	StringArray _lines;
	int _topVisible;
	int _xOffset;
private:
	/**
	 * Common initialization method used by the constructors
	 */
	void init();
public:
	CLASSDEF;
	Listbox() : Gfx::VisualItem() { init(); }
	Listbox(const Common::Rect &r) : Gfx::VisualItem(r) { init(); }
	Listbox(const Common::String &name) : Gfx::VisualItem(name) { init(); }
	Listbox(const Common::String &name, const Common::Rect &r) : 
		Gfx::VisualItem(name, r) { init(); }
	virtual ~Listbox() {}

	/**
	 * Loads a listbox with a list of strings
	 */
	void load(const StringArray &lines);

	/**
	 * Loads a listbox with a list of strings based on a resource
	 * containing string identifiers
	 */
	void load(const String &resName);
};

} // End of namespace Early
} // End of namespace Legend

#endif
