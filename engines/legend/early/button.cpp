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

#include "legend/early/button.h"
#include "legend/early/screen.h"
#include "legend/legend.h"
#include "legend/core/resources.h"

namespace Legend {
namespace Early {

BEGIN_MESSAGE_MAP(Button, Gfx::VisualItem)
	ON_MESSAGE(MouseButtonDownMsg)
END_MESSAGE_MAP()

void Button::draw() {
	if (!_isDirty)
		return;
	Gfx::VisualItem::draw();

	// Draw the button
	Graphics::ManagedSurface s = getSurface();
	// TODO:
	s.fillRect(s.getBounds(), 15);
}

bool Button::MouseButtonDownMsg(CMouseButtonDownMsg &msg) {
	execute();
	return true;
}

} // End of namespace Early
} // End of namespace Legend
