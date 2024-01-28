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

#include "wasteland/fod/views/dialogs/message_dialog.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

bool MessageDialog::msgGame(const GameMessage &msg) {
	if (msg._name == "SHOW") {
		_message = msg._stringValue;
		addView();
		return true;
	}

	return false;
}

void MessageDialog::draw() {
	Dialog::draw();

	Surface s = getSurface();
	s.writeString(_message);
}

bool MessageDialog::msgKeypress(const KeypressMessage &msg) {
	close();
	g_engine->_scripts.resume(Logic::ScriptResumeParams());
	return true;
}

bool MessageDialog::msgAction(const ActionMessage &msg) {
	close();
	g_engine->_scripts.resume(Logic::ScriptResumeParams());
	return true;
}

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland
