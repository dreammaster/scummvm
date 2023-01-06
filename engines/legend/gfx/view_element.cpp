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

#include "legend/gfx/view.h"

namespace Legend {
namespace Gfx {

ViewElement::ViewElement(UIElement *uiParent, const Common::Rect &area) :
		UIElement("", uiParent), _font(_fontNumber) {
	setBounds(area);
}

ViewElement::ViewElement(const Common::Rect &area) :
		UIElement("", nullptr), _font(_fontNumber) {
	setBounds(area);
}

ViewElement::ViewElement(UIElement *uiParent, const String &name) :
		UIElement(name, uiParent), _font(_fontNumber) {
}

ViewElement::ViewElement(UIElement *uiParent, const String &name,
		const Common::Rect &area) :
		UIElement(name, uiParent), _font(_fontNumber) {
	setBounds(area);
}


void ViewElement::drawElements() {
	// Ensure the correct font is active
	if (_fontNumber != -1)
		_font->setColor(_fgColor, _bgColor);

	UIElement::drawElements();
}

void ViewElement::setFontColor(int fgColor, int bgColor) {
	assert(_fontNumber != -1);
	_fgColor = fgColor;
	_bgColor = bgColor;
	_font->setColor(_fgColor, _bgColor);
}

void ViewElement::writeString(const Common::Point &pt, const String &str) {
	_textPos = pt;
	writeString(str);
}

void ViewElement::writeString(const String &str) {
	Graphics::ManagedSurface s = getSurface();
	_font->writeString(s, _textPos, str);
	_textPos.x += _font->stringWidth(str);
}

void ViewElement::writeChar(const Common::Point &pt, char c) {
	_textPos = pt;
	writeChar(c);
}

void ViewElement::writeChar(char c) {
	Graphics::ManagedSurface s = getSurface();
	_font->writeChar(s, _textPos, c);
	_textPos.x += _font->charWidth(c);
}

} // namespace Gfx
} // namespace Legend
