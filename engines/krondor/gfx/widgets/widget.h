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

#ifndef KRONDOR_GFX_WIDGETS_WIDGET_H
#define KRONDOR_GFX_WIDGETS_WIDGET_H

#include "krondor/events.h"
#include "krondor/res/request_resource.h"

namespace Krondor {
namespace Gfx {
namespace Widgets {

static const int NO_SHADOW = -1;
static const int COLOR_BLACK = 0;
static const int SHADOW_COLOR = 1;
static const int LIGHT_COLOR = 4;
static const int TEXT_COLOR_NORMAL = 10;
static const int TEXT_COLOR_DISABLED = 11;
static const int TEXT_COLOR_PRESSED = 6;
static const int BUTTON_COLOR_NORMAL = 14;
static const int BUTTON_COLOR_PRESSED = 11;
static const int POPUP_COLOR = 14;
static const int INFO_TEXT_COLOR = 159;

static const int RIGHT_CLICK_OFFSET = 256;
static const int RELEASE_OFFSET = 512;
static const int INVENTORY_OFFSET = 1024;

class Widget : public UIElement {
protected:
	const RequestData *_requestData;
	bool _focused = false;

public:
	Widget(const Common::String &name, const RequestData *reqData);
	virtual ~Widget() {}
	const RequestData *getData() const {
		return _requestData;
	}
};

} // namespace Widgets
} // namespace Gfx
} // namespace Krondor

#endif
