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

#ifndef WASTELAND_GFX_TEXT_ENTRY_H
#define WASTELAND_GFX_TEXT_ENTRY_H

#include "wasteland/events.h"

namespace Wasteland {
namespace Gfx {

class TextEntry : public UIElement {
private:
	Common::String _text;
	size_t _maxLength;
	bool _caretVisible = false;
	int _caretCtr = 0;

public:
	TextEntry(const Common::String &name, UIElement *uiParent, int x, int y,
		size_t maxLength);
	TextEntry(const Common::String &name, UIElement *uiParent, const Position &pos,
		size_t maxLength);

	void draw() override;
	bool msgKeypress(const KeypressMessage &msg) override;
	bool tick() override;

	void setText(const Common::String &text);
	void setPosition(const Position &pos);
};

} // namespace Gfx
} // namespace Wasteland

#endif
