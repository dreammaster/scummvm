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

#include "wasteland/wasteland1/views/gfx/clickable_text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

ClickableText::ClickableText(UIElement *parent, const Common::String &name) :
	UIElement(name, parent) {
}

bool ClickableText::msgMouseEnter(const MouseEnterMessage &msg) {
	if (isValid() && isEnabled())
		g_events->setCursor(1);

	return true;
}

bool ClickableText::msgMouseLeave(const MouseLeaveMessage &msg) {
	if (isEnabled())
	g_events->setCursor(0);

	return true;
}

void ClickableText::draw() {
	Surface s = getSurface(_bounds);

	s.writeString(_text);
}

bool ClickableText::msgMouseDown(const MouseDownMessage &msg) {
	if (isValid() && isEnabled())
		_parent->send(GameMessage(_name));

	return true;
}

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
