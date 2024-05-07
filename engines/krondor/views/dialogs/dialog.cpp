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

#include "common/system.h"
#include "graphics/paletteman.h"
#include "krondor/views/dialogs/dialog.h"
#include "krondor/gfx/widgets/text_button.h"
#include "krondor/gfx/widgets/select.h"

namespace Krondor {
namespace Views {
namespace Dialogs {

bool Dialog::msgFocus(const FocusMessage &msg) {
	_request.load(_requestName);
	_palette.load(_paletteName);
	_screen.load(_screenName);
	_normal.load(_normalName);
	_font.load(_fontName);
	_label.load(_labelName);

	_palette.setActive();
	_font.setActive();

	return true;
}

bool Dialog::msgUnfocus(const UnfocusMessage &msg) {
	_request.clear();
	_palette.clear();
	_screen.clear();
	_normal.clear();
	_font.clear();
	_label.clear();

	// Remove added children controls
	for (auto *child : _children)
		delete child;
	_children.clear();

	return true;
}

void Dialog::draw() {
	GfxSurface s = getSurface();
	s.blitFrom(_screen);

	s.writeString(Common::Point(50, 50), "Testing testing");
}

Gfx::Widgets::Widget *Dialog::createWidget(const RequestData *reqData) {
	switch (reqData->_widget) {
	case REQ_TEXTBUTTON:
		return new Gfx::Widgets::TextButton(reqData);
	case REQ_SELECT:
		return new Gfx::Widgets::Select(reqData);
	default:
		error("Invalid widget type - %d", reqData->_widget);
	}
}

} // namespace Dialogs
} // namespace Views
} // namespace Krondor
