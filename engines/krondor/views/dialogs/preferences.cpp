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
#include "krondor/views/dialogs/preferences.h"
#include "krondor/res/resources.h"

namespace Krondor {
namespace Views {
namespace Dialogs {

bool Preferences::msgFocus(const FocusMessage &msg) {
	// Dummy stuff
	RequestResource req("req_pref.dat");
	PaletteResource pal("options.pal");
	LabelResource lbl("lbl_pref.dat");

	return true;
}

bool Preferences::msgKeypress(const KeypressMessage &msg) {
	// Any keypress to close the view
	close();
	return true;
}

void Preferences::draw() {
	// Draw a bunch of squares on screen
	Graphics::ManagedSurface s = getSurface();

	for (int i = 0; i < 100; ++i)
		s.frameRect(Common::Rect(i, i, 320 - i, 200 - i), i);
}

bool Preferences::tick() {
	return true;
}

} // namespace Dialogs
} // namespace Views
} // namespace Krondor
