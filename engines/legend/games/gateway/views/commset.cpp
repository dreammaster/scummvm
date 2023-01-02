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

#include "legend/games/gateway/views/commset.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Views {

enum {
	BTN_NONE, BTN_1, BTN_2, BTN_3, BTN_4, BTN_5, BTN_6,
	BTN_7, BTN_8, BTN_9, BTN_PREV, BTN_0, BTN_NEXT
};

CommSetContents::CommSetContents(UIElement *parent, const Common::Rect &area) :
	Legend::Gfx::ViewElement(parent, area) {
}

void CommSetContents::draw() {
	// TODO
}

CommSet::CommSet() : Legend::Early::Gfx::View("CommSet"), _logo(nullptr),
		_btn1(this, "1", BTN_1, Common::Rect(44, 113, 84, 135)),
		_btn2(this, "2", BTN_2, Common::Rect(88, 113, 128, 135)),
		_btn3(this, "3", BTN_3, Common::Rect(132, 113, 172, 135)),
		_btn4(this, "4", BTN_4, Common::Rect(44, 139, 84, 161)),
		_btn5(this, "5", BTN_5, Common::Rect(88, 139, 128, 161)),
		_btn6(this, "6", BTN_6, Common::Rect(132, 139, 172, 161)),
		_btn7(this, "7", BTN_7, Common::Rect(44, 165, 84, 187)),
		_btn8(this, "8", BTN_8, Common::Rect(88, 165, 128, 187)),
		_btn9(this, "9", BTN_9, Common::Rect(132, 165, 172, 187)),
		_btnPrev(this, "Prev", BTN_PREV, Common::Rect(44, 191, 84, 213)),
		_btn0(this, "0", BTN_0, Common::Rect(88, 191, 128, 213)),
		_btnNext(this, "Next", BTN_NEXT, Common::Rect(132, 191, 172, 213)),
		_contents(this, Common::Rect(188, 56, 596, 424)) {
	setFont(2);
}

CommSet::~CommSet() {
	delete _logo;
}

void CommSet::draw() {
	// TODO

}

} // namespace Views
} // namespace Gateway
} // namespace Early
} // namespace Legend
