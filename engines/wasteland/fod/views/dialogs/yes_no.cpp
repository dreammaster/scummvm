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

#include "wasteland/fod/views/dialogs/yes_no.h"
#include "wasteland/fod/fod.h"
#include "wasteland/keymapping.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

void YesNo::show(const Common::String &message, YesNoCallback callback) {
	YesNo *view = static_cast<YesNo *>(g_engine->findView("YesNo"));
	view->_message = message;
	view->_callback = callback;
	view->addView();
}

void YesNo::draw() {
	Dialog::draw();

	Surface s = getSurface();
	s.writeString(Common::String::format("%s\n\n%s",
		_message.c_str(),
		"Y)es  N)o"));
}

bool YesNo::msgAction(const ActionMessage &msg) {
	if (msg._action == KEYBIND_SELECT) {
		response(true);
		return true;
	} else if (msg._action == KEYBIND_ESCAPE) {
		response(false);
		return true;
	}

	return false;
}

bool YesNo::msgKeypress(const KeypressMessage &msg) {
	if (msg.keycode == Common::KEYCODE_y) {
		response(true);
		return true;
	} else if (msg.keycode == Common::KEYCODE_n) {
		response(false);
		return true;
	}

	return false;
}

void YesNo::response(bool isYes) {
	close(); 
}

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland
