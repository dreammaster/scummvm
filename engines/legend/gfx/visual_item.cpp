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

#include "legend/gfx/visual_item.h"
#include "legend/legend.h"

namespace Legend {

EMPTY_MESSAGE_MAP(VisualItem, NamedItem);

Graphics::ManagedSurface VisualItem::getSurface() const {
	assert(!_bounds.isEmpty());
	return Graphics::ManagedSurface(screen(), _bounds);
}

Screen &VisualItem::screen() const {
	return *g_vm->_screen;
}

void VisualItem::setDirty() {
	// Flag the item as dirty
	_isDirty = true;

	// Flag any child items also as dirty, since rendering the parent
	// would cover up any areas that the sub-items render within
	for (TreeItem *treeItem = scan(this); treeItem; treeItem = treeItem->scan(this)) {
		VisualItem *item = dynamic_cast<VisualItem *>(treeItem);
		if (item)
			item->_isDirty = true;
	}
}

} // End of namespace Legend
