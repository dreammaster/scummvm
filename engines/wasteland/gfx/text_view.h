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

#ifndef WASTELAND_GFX_TEXT_VIEW_H
#define WASTELAND_GFX_TEXT_VIEW_H

#include "common/str-array.h"
#include "wasteland/events.h"

namespace Wasteland {
namespace Shared {
namespace Gfx {

class TextView : public UIElement {
private:
	Common::StringArray _text;

	/**
	 * Returns the number of rows that the view contains
	 */
	uint size() const {
		return _bounds.height() / FONT_H;
	}
public:
	TextView(const Common::String &name, UIElement *uiParent);
	TextView(const Common::String &name, UIElement *uiParent,
		const Common::Rect &r);

	void setBounds(const Common::Rect &r) override {
		UIElement::setBounds(r);
		clear();
	}

	void draw() override;

	void clear();
	void print(const char *fmt, ...);
	void print(const Common::String &str);
};

} // namespace Gfx
} // namespace Shared
} // namespace Wasteland

#endif
