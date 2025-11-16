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

#include "ags2/ac/gui/interface.h"
#include "ags2/vars.h"

namespace AGS2 {

void DisableInterface() {
	play.disabled_user_interface++;
	guis_need_update = 1;
	set_mouse_cursor(CURS_WAIT);
}

void EnableInterface() {
	guis_need_update = 1;
	play.disabled_user_interface--;
	if (play.disabled_user_interface < 1) {
		play.disabled_user_interface = 0;
		set_default_cursor();
	}
}
// Returns 1 if user interface is enabled, 0 if disabled
int IsInterfaceEnabled() {
	return (play.disabled_user_interface > 0) ? 0 : 1;
}

} // namespace AGS2
