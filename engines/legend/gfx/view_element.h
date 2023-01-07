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

#ifndef LEGEND_GFX_VIEW_ELEMENT_H
#define LEGEND_GFX_VIEW_ELEMENT_H

#include "legend/gfx/font.h"
#include "legend/gfx/region.h"
#include "legend/events.h"

namespace Legend {
namespace Gfx {

class ViewElement : public UIElement {
	class FontPtr {
	private:
		const int &_fontNumber;
	public:
		FontPtr(int &fontNumber) : _fontNumber(fontNumber) {
		}
		Gfx::Font * operator->() const {
			return Gfx::Font::loadFont(_fontNumber);
		}
	};
private:
	int _fontNumber = 1;
protected:
	FontPtr _font;
	Common::Point _textPos;
	int _fgColor = 0, _bgColor = 0;
	Regions _regions;

	/**
	 * Draw a view's elements
	 */
	void drawElements() override;

public:
	ViewElement(const String &name, UIElement *uiParent = nullptr) :
		UIElement(name, uiParent), _font(_fontNumber) {}
	ViewElement() : UIElement("", nullptr), _font(_fontNumber) {}
	ViewElement(UIElement *uiParent) : UIElement("", uiParent), _font(_fontNumber) {}
	ViewElement(UIElement *uiParent, const Common::Rect &area);
	ViewElement(UIElement *uiParent, const String &name);
	ViewElement(UIElement *uiParent, const String &name,
		const Common::Rect &area);
	ViewElement(const Common::Rect &area);

	~ViewElement() override {}

	/**
	 * Set the font the element uses, if any
	 */
	void setFont(int fontNumber) {
		if (fontNumber != _fontNumber) {
			_fontNumber = fontNumber;
			_needsRedraw = true;
		}
		metricsChanged();
	}

	/**
	 * Returns true if the view has an associated font
	 */
	bool hasFont() const {
		return _fontNumber != -1;
	}

	/**
	 * Write a string
	 */
	void writeString(const Common::Point &pt, const String &str);
	void writeString(const String &str);

	/**
	 * Write a character
	 */
	void writeChar(const Common::Point &pt, char c);
	void writeChar(char c);

	/**
	 * Set the font color
	 */
	void setFontColor(int fgColor, int bgColor);
};

} // namespace Gfx
} // namespace Legend

#endif
