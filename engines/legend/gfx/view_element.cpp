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
		UIElement("", uiParent) {
	setBounds(area);
}

ViewElement::ViewElement(const Common::Rect &area) : UIElement("", nullptr) {
	setBounds(area);
}

ViewElement::ViewElement(UIElement *uiParent, const Common::String &name) :
		UIElement(name, uiParent) {
}

ViewElement::ViewElement(UIElement *uiParent, const Common::String &name,
		const Common::Rect &area) : UIElement(name, uiParent) {
	setBounds(area);
}


void ViewElement::drawElements() {
	// Ensure the correct font is active
	if (_fontNumber != -1)
		_font = Gfx::Font::loadFont(_fontNumber);

	UIElement::drawElements();
}

void ViewElement::writeString(const Common::Point &pt, const Common::String &str) {
	_textPos = pt;
	writeString(str);
}

void ViewElement::writeString(const Common::String &str) {
	assert(_font);
	Graphics::ManagedSurface s = getSurface();
	_font->writeString(s, _textPos, str);
	_textPos.x += _font->stringWidth(str);
}

void ViewElement::writeChar(const Common::Point &pt, char c) {
	_textPos = pt;
	writeChar(c);
}

void ViewElement::writeChar(char c) {
	assert(_font);
	Graphics::ManagedSurface s = getSurface();
	_font->writeChar(s, _textPos, c);
	_textPos.x += _font->charWidth(c);
}

} // namespace Gfx
} // namespace Legend
