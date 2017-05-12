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

#include "legend/gfx/visual_container.h"

namespace Legend {
namespace Gfx {

BEGIN_MESSAGE_MAP(VisualContainer, VisualItem)
	ON_MESSAGE(MouseButtonDownMsg)
	ON_MESSAGE(MouseButtonUpMsg)
	ON_MESSAGE(MouseMoveMsg)
	ON_MESSAGE(MouseDoubleClickMsg)
END_MESSAGE_MAP()


bool VisualContainer::MouseButtonDownMsg(CMouseButtonDownMsg *msg) {
	if (msg->_buttons & MB_LEFT)
		handleMouseMsg(msg);

	return true;
}

bool VisualContainer::MouseButtonUpMsg(CMouseButtonUpMsg *msg) {
	if (msg->_buttons & MB_LEFT)
		handleMouseMsg(msg);

	return true;
}

bool VisualContainer::MouseDoubleClickMsg(CMouseDoubleClickMsg *msg) {
	if (msg->_buttons & MB_LEFT)
		handleMouseMsg(msg);

	return true;
}

bool VisualContainer::MouseMoveMsg(CMouseMoveMsg *msg) {
	if (msg->_buttons & MB_LEFT)
		handleMouseMsg(msg);

	return true;
}

bool VisualContainer::handleMouseMsg(CMouseMsg *msg) {
	// Iterate through each child and pass the message to the first
	// immediate child the mouse position falls within
	for (TreeItem *child = getFirstChild(); child; child = child->getNextSibling()) {
		VisualItem *item = dynamic_cast<VisualItem *>(child);
		if (item && item->getBounds().contains(msg->_mousePos)) {
			if (msg->execute(item))
				break;
		}
	}

	return true;
}

void VisualContainer::draw() {
	if (!_isDirty)
		return;

	// Handle drawing any background
	Image::draw();

	// Iterate through each child and draw the visual items
	for (TreeItem *child = getFirstChild(); child; child = child->getNextSibling()) {
		VisualItem *item = dynamic_cast<VisualItem *>(child);
		if (item)
			item->draw();
	}
}

} // End of namespace Gfx
} // End of namespace Legend
