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

#ifndef WASTELAND_WASTELAND1_VIEWS_GFX_CLICKABLE_TEXT_H
#define WASTELAND_WASTELAND1_VIEWS_GFX_CLICKABLE_TEXT_H

#include "wasteland/events.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

class ClickableText : public UIElement {
private:
	Common::String _text;
	bool _enabled = true;

protected:
	virtual bool isValid() const {
		return !_text.empty();
	}

public:
	ClickableText(UIElement *parent, const Common::String &name);
	~ClickableText() override {
	}

	bool msgMouseEnter(const MouseEnterMessage &msg) override;
	bool msgMouseLeave(const MouseLeaveMessage &msg) override;
	void draw() override;
	bool msgMouseDown(const MouseDownMessage &msg) override;

	void setText(const Common::String &text) {
		_text = text;
		redraw();
	}

	bool isEnabled() const {
		return _enabled;
	}
	void setEnabled(bool enabled) {
		_enabled = enabled;
	}
};

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
