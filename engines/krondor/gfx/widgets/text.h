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

#ifndef KRONDOR_GFX_WIDGETS_TEXT_H
#define KRONDOR_GFX_WIDGETS_TEXT_H

#include "krondor/gfx/widgets/widget.h"

namespace Krondor {
namespace Gfx {
namespace Widgets {

enum HorizontalAlignment {
	HA_LEFT,
	HA_CENTER,
	HA_RIGHT
};

enum VerticalAlignment {
	VA_TOP,
	VA_CENTER,
	VA_BOTTOM
};

class Text : public UIElement {
private:
	Common::String _text;
	int _textWidth = 0;
	int _textHeight = 0;
	int _color = 0;
	int _shadow = 0;
	int _shadowXoff = 0;
	int _shadowYoff = 0;
	HorizontalAlignment _horAlign = HA_CENTER;
	VerticalAlignment _vertAlign = VA_CENTER;

public:
	Text();
	Text(const Common::Rect &r, const char *text = nullptr);
	Text(int x, int y, const Common::String &text);
	virtual ~Text() {}

	void draw() override;

	void setColor(const int c);
	void setText(const Common::String &s);
	void setShadow(const int s, const int xoff, const int yoff);
	void setAlignment(const HorizontalAlignment ha, const VerticalAlignment va);
};

} // namespace Widgets
} // namespace Gfx
} // namespace Krondor

#endif
