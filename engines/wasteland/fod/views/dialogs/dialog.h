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

#ifndef WASTELAND_FOD_VIEWS_DIALOGS_DIALOG_H
#define WASTELAND_FOD_VIEWS_DIALOGS_DIALOG_H

#include "wasteland/events.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

class Dialog : public UIElement {
public:
	Dialog(const Common::String &name, int w, int h);
	Dialog(const Common::String &name, const Gfx::Window &win);
	virtual ~Dialog() {}

	void draw() override;
};

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland

#endif
