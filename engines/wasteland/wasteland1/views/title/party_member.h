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

#ifndef WASTELAND_WASTELAND1_VIEWS_TITLE_PARTY_MEMBER_H
#define WASTELAND_WASTELAND1_VIEWS_TITLE_PARTY_MEMBER_H

#include "wasteland/wasteland1/views/gfx/clickable_text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Title {

/**
 * Implements a party member line in the title roster list
 */
class PartyMember : public ClickableText {
private:
	Common::String _text;
	uint _memberIndex;
	bool _selected = false;

protected:
	bool isValid() const override;

public:
	PartyMember(UIElement *parent, uint memberIndex);
	~PartyMember() override {
	}

	void draw() override;
	bool msgMouseDown(const MouseDownMessage &msg) override;
	bool msgKeypress(const KeypressMessage &msg) override;

	void setSelected(bool val) {
		_selected = val;
		redraw();
	}

};

} // namespace Title
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
