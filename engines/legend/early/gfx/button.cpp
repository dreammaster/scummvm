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

#include "legend/early/gfx/button.h"
#include "legend/early/gfx/screen.h"
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

	// Get the surface
	Gfx::VisualSurface s = getSurface();

	// White background
	Common::Rect r = s.getBounds();
	s.fillRect(r, WHITE);
	// Black edges
	s.hLine(r.left + 1, r.top, r.right - 2, BLACK);
	s.hLine(r.left + 1, r.bottom - 1, r.right - 2, BLACK);
	s.vLine(r.left, r.top + 1, r.bottom - 2, BLACK);
	s.vLine(r.right - 1, r.top + 1, r.bottom - 2, BLACK);
	// Dark grey bottom/right button inner edges
	s.hLine(r.left + 1, r.bottom - 2, r.right - 2, DARK_GRAY);
	s.hLine(r.left + 2, r.bottom - 3, r.right - 2, DARK_GRAY);
	s.vLine(r.right - 3, r.top + 2, r.bottom - 4, DARK_GRAY);
	s.vLine(r.right - 2, r.top + 1, r.bottom - 4, DARK_GRAY);
	// Inner button content
	s.fillRect(Common::Rect(r.left + 3, r.top + 3, r.right - 3, r.bottom - 3), LIGHT_GRAY);

	// Handle writing the button text
	s.loadFont(1);
	s.setFontColor(BLACK);
	int strWidth = s.stringWidth(_text);
	int xp = _bounds.left + (_bounds.width() - strWidth) / 2;
	s.writeString(Common::Point(xp, _bounds.top + 5), _text);
}

bool Button::MouseButtonDownMsg(CMouseButtonDownMsg &msg) {
	execute();
	return true;
}

} // End of namespace Early
} // End of namespace Legend
