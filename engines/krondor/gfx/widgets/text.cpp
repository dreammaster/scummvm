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

#include "krondor/gfx/widgets/text.h"

namespace Krondor {
namespace Gfx {
namespace Widgets {

Text::Text() : UIElement("Text") {
}

Text::Text(const Common::Rect &r, const char *text) : UIElement("Text") {
	setBounds(r);
	if (text)
		setText(text);
}

Text::Text(int x, int y, const Common::String &text) : UIElement("Text") {
	setText(text);
	Common::Rect r(x, y, x + _textWidth, y + _textHeight);
	setBounds(r);
}

void Text::setText(const Common::String &s) {
	_text = s;

	Graphics::Font *font = getFont();
	_textWidth = font->getStringWidth(_text);
	_textHeight = font->getFontHeight();
}

void Text::setColor(const int c) {
	_color = c;
}

void Text::setShadow(const int s, const int xoff, const int yoff) {
	_shadow = s;
	_shadowXoff = xoff;
	_shadowYoff = yoff;
}

void Text::setAlignment(const HorizontalAlignment ha, const VerticalAlignment va) {
	_horAlign = ha;
	_vertAlign = va;
}

void Text::draw() {
	GfxSurface s = getSurface();
	Graphics::Font *font = getFont();
	int i;
	int w;
	int xoff = 0;
	int yoff = 0;

	switch (_horAlign) {
	case HA_LEFT:
		xoff = 0;
		break;
	case HA_CENTER:
		xoff = (_bounds.width() - _textWidth) / 2;
		break;
	case HA_RIGHT:
		xoff = _bounds.width() - _textWidth;
		break;
	}

	switch (_vertAlign) {
	case VA_TOP:
		yoff = 0;
		break;
	case VA_CENTER:
		yoff = (_bounds.height() - _textHeight) / 2;
		break;
	case VA_BOTTOM:
		yoff = _bounds.height() - _textHeight;
		break;
	}
	if ((_shadow > NO_SHADOW) && ((_shadowXoff != 0) || (_shadowYoff != 0))) {
		i = 0;
		w = 0;

		while ((i < (int)_text.size()) && (w + font->getCharWidth(_text[i]) < (int)_bounds.width())) {
			font->drawChar(&s, _text[i], xoff + _shadowXoff + w,
				yoff + _shadowYoff, _shadow);

			w += font->getCharWidth(_text[i]);
			i++;
		}
	}

	i = 0;
	w = 0;
	while ((i < (int)_text.size()) && (w + font->getCharWidth(_text[i]) < (int)_bounds.width())) {
		font->drawChar(&s, _text[i], xoff + w, yoff, _color);
		w += font->getCharWidth(_text[i]);
		i++;
	}
}

} // namespace Widgets
} // namespace Gfx
} // namespace Krondor
