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

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/video.h"
#include "mads/madsv2/core/mouse.h"

namespace MADS {
namespace MADSV2 {

extern Buffer scr_work;

void mouse_video_init() {
	mouse_set_work_buffer(scr_work.data, scr_work.x);
	mouse_set_view_port_loc(0, 0, scr_work.x - 1, scr_work.y - 1);
	mouse_set_view_port(0, 0);
}

void mouse_video_update(int from_x, int from_y, int unto_x, int unto_y,
		int size_x, int size_y) {
	int refresh_flag;

	mouse_freeze();
	refresh_flag = mouse_refresh_view_port();

	video_update(&scr_work, from_x, from_y, unto_x, unto_y, size_x, size_y);

	if (refresh_flag) mouse_refresh_done();
	mouse_thaw();
}

} // namespace MADSV2
} // namespace MADS



