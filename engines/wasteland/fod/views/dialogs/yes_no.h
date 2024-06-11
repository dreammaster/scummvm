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

#ifndef WASTELAND_FOD_VIEWS_DIALOGS_YES_NO_H
#define WASTELAND_FOD_VIEWS_DIALOGS_YES_NO_H

#include "wasteland/fod/views/dialogs/dialog.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

typedef void (*YesNoCallback)(bool response);

class YesNo : public Dialog {
private:
	Common::String _message;
	YesNoCallback _callback = nullptr;

	/**
	 * Passed the yes/no selection
	 */
	void response(bool isYes);

public:
	static void show(YesNo *view, const Common::String &message, YesNoCallback callback);

public:
	YesNo() : Dialog("YesNo", Window(8, 6, 31, 17)) {}
	YesNo(const Common::String &name) : Dialog(name, Window(8, 6, 31, 17)) {}
	virtual ~YesNo() {}

	void draw() override;
	bool msgAction(const ActionMessage &msg) override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

class LeaveMap : public YesNo {
public:
	LeaveMap() : YesNo("LeaveMap") {}
	virtual ~LeaveMap() {}

	static void show();
};

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland

#endif
