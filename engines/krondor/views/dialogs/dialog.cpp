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
#include "krondor/gfx/widgets/text.h"

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

	// Create all the request based controls
	for (const auto &req : _request._data)
		_children.push_back(createWidget(&req));

	// Create elements for all the labels
	for (const auto &lbl : _label._data)
		_children.push_back(createWidget(&lbl));

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
}

Gfx::Widgets::Widget *Dialog::createWidget(const RequestData *reqData) {
	switch (reqData->_widget) {
	case REQ_TEXTBUTTON:
		return new Gfx::Widgets::TextButton(reqData);
	case REQ_SELECT: {
		auto *widget = new Gfx::Widgets::Select(reqData);
		widget->setImage(_normal.getImage(reqData->_image + 1),
			_normal.getImage(reqData->_image));
		return widget;
	}
	default:
		error("Invalid widget type - %d", reqData->_widget);
	}
}

Gfx::Widgets::Text *Dialog::createWidget(const LabelData *lblData) {
	uint width = 1;
	Graphics::Font *font = getFont();
	uint strWidth = font->getStringWidth(lblData->_label);

	switch (lblData->_type) {
	case LBL_STANDARD:
		width = strWidth + 1;
		break;
	case LBL_TITLE:
		width = this->_bounds.width();
		break;
	default:
		break;
	}

	Gfx::Widgets::Text *label = new Gfx::Widgets::Text(
		Common::Rect(lblData->_xpos, lblData->_ypos,
			lblData->_xpos + width,
			lblData->_ypos + font->getFontHeight() + 1),
		lblData->_label.c_str());
	label->setColor(lblData->_color);

	if (lblData->_type == LBL_TITLE) {
		label->setShadow(lblData->_shadow, 0, 1);
		label->setAlignment(Gfx::Widgets::HA_CENTER,
			Gfx::Widgets::VA_TOP);
	}

	return label;
}

} // namespace Dialogs
} // namespace Views
} // namespace Krondor
