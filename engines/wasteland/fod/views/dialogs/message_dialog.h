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

#ifndef WASTELAND_FOD_VIEWS_MESSAGE_DIALOG_H
#define WASTELAND_FOD_VIEWS_MESSAGE_DIALOG_H

#include "wasteland/fod/views/dialogs/dialog.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

class MessageDialog : public Dialog {
private:
	Common::String _message;

public:
	MessageDialog() : Dialog("MessageDialog", Window(8, 6, 31, 17)) {}
	virtual ~MessageDialog() {}

	bool msgGame(const GameMessage &msg) override;
	void draw() override;
	bool msgKeypress(const KeypressMessage &msg) override;
	bool msgAction(const ActionMessage &msg) override;
};

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland

#endif
