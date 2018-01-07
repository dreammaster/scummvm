/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/early/gateway/commset.h"
#include "legend/early/gfx/screen.h"
#include "legend/legend.h"
#include "legend/core/resources.h"

namespace Legend {
namespace Early {

BEGIN_MESSAGE_MAP(Commset, VisualContainer)
	ON_MESSAGE(ShowMsg)
	ON_MESSAGE(HideMsg)
	ON_MESSAGE(MouseButtonDownMsg)
	ON_MESSAGE(KeyCharMsg)
END_MESSAGE_MAP()

enum {
	BTN_NONE, BTN_1, BTN_2, BTN_3, BTN_4, BTN_5, BTN_6,
	BTN_7, BTN_8, BTN_9, BTN_PREV, BTN_0, BTN_NEXT
};

Commset::Commset() : VisualContainer(), _logo(nullptr),
		_btn1("1", BTN_1, Common::Rect(44, 113, 84, 135)),
		_btn2("2", BTN_2, Common::Rect(88, 113, 128, 135)),
		_btn3("3", BTN_3, Common::Rect(132, 113, 172, 135)),
		_btn4("4", BTN_4, Common::Rect(44, 139, 84, 161)),
		_btn5("5", BTN_5, Common::Rect(88, 139, 128, 161)),
		_btn6("6", BTN_6, Common::Rect(132, 139, 172, 161)),
		_btn7("7", BTN_7, Common::Rect(44, 165, 84, 187)),
		_btn8("8", BTN_8, Common::Rect(88, 165, 128, 187)),
		_btn9("9", BTN_9, Common::Rect(132, 165, 172, 187)),
		_btnPrev("Prev", BTN_PREV, Common::Rect(44, 191, 84, 213)),
		_btn0("0", BTN_0, Common::Rect(88, 191, 128, 213)),
		_btnNext("Next", BTN_NEXT, Common::Rect(132, 191, 172, 213)) {
	_name = "Commset";
	setBounds(Screen::get().getBounds());

	// Add the buttons
	ValueButton *btns[12] = { &_btn1, &_btn2, &_btn3, &_btn4, &_btn5, &_btn6,
		&_btn7, &_btn8, &_btn9, &_btnPrev, &_btn0, &_btnNext };
	for (int idx = 0; idx < 12; ++idx)
		btns[idx]->addUnder(this);

	// Set up the regions
	const int TEXT_COLUMNS = 72;
	const int TEXT_ROWS = 25;
	// TODO: Regions
}

Commset::~Commset() {
	delete _logo;
}

void Commset::draw() {
	if (!_isDirty)
		return;

	// Fill out the background
	Gfx::VisualSurface s = getSurface();
	s.fillRect(s.getBounds(), 2);

	s.blitFrom(*_logo);

	// Draw the buttons
	VisualContainer::draw();
}

bool Commset::ShowMsg(CShowMsg &msg) {
	_logo = g_vm->_picFile->load(574, 1);
	return true;
}

bool Commset::HideMsg(CHideMsg &msg) {
	delete _logo;
	_logo = nullptr;
	return true;
}

bool Commset::MouseButtonDownMsg(CMouseButtonDownMsg &msg) {
	return true;
}

bool Commset::KeyCharMsg(CKeyCharMsg &msg) {
	return true;
}

} // End of namespace Early
} // End of namespace Legend
