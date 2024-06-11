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

#ifndef WASTELAND_GFX_CLICKABLE_TEXT_H
#define WASTELAND_GFX_CLICKABLE_TEXT_H

#include "wasteland/events.h"

namespace Wasteland {
namespace Shared {
namespace Gfx {

class ClickableText : public UIElement {
private:
	Common::String _text;
	bool _inverseColor = false;
	Common::KeyCode _keycode;
	Common::String _message;
	int _tag = 0;

public:
	ClickableText(const Common::String &name, UIElement *owner, int x, int y,
		const Common::String &text, Common::KeyCode keycode);
	ClickableText(const Common::String &name, UIElement *owner, const Position &pos, const Common::String &text, Common::KeyCode keycode);
	ClickableText(const Common::String &name, UIElement *owner, int x, int y,
		const Common::String &text, const Common::String &message, int tag = 0);
	ClickableText(const Common::String &name, UIElement *owner, const Position &pos, const Common::String &text, const Common::String &message, int tag = 0);

	void draw() override;
	bool msgMouseDown(const MouseDownMessage &msg) override;

	void setInverseColor(bool isInverse) {
		_inverseColor = isInverse;
		redraw();
	}
	void setText(const Common::String &text);
};

} // namespace Gfx
} // namespace Shared
} // namespace Wasteland

#endif
