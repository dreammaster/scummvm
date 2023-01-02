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

#ifndef LEGEND_GFX_VIEW_H
#define LEGEND_GFX_VIEW_H

#include "legend/gfx/font.h"
#include "legend/events.h"

namespace Legend {
namespace Gfx {

class View : public UIElement {
private:
	int _fontNumber = -1;
protected:
	Gfx::Font *_font = nullptr;
	Common::Point _textPos;

	/**
	 * Draw a view's elements
	 */
	void drawElements() override;

public:
	View(const Common::String &name, UIElement *uiParent = nullptr) :
		UIElement(name, uiParent) {}
	View() : UIElement("", nullptr) {}
	View(UIElement *uiParent) : UIElement("", uiParent) {}
	View(UIElement *uiParent, const Common::Rect &area);
	View(const Common::Rect &area);

	~View() override {}

	/**
	 * Set the font the element uses, if any
	 */
	void setFont(int fontNumber) {
		if (fontNumber != _fontNumber) {
			_fontNumber = fontNumber;
			_needsRedraw = true;
		}
	}

	/**
	 * Write a string
	 */
	void writeString(const Common::Point &pt, const Common::String &str);
	void writeString(const Common::String &str);

	/**
	 * Write a character
	 */
	void writeChar(const Common::Point &pt, char c);
	void writeChar(char c);
};

} // namespace Gfx
} // namespace Legend

#endif
