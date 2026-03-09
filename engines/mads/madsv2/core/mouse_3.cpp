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
#include "mads/madsv2/core/sprite.h"
#include "mads/madsv2/core/buffer.h"
#include "mads/madsv2/core/mouse_1.h"

namespace MADS {
namespace MADSV2 {

int mouse_hot_x = 0;
int mouse_hot_y = 0;

void mouse_cursor_sprite(SeriesPtr series, int id) {
	byte work_area[17][17];
	Buffer load_buffer = { 17, 17 };
	int hot_x, hot_y, count;

	load_buffer.data = &work_area[0][0];

	hot_x = 0;
	hot_y = 0;

	buffer_fill(load_buffer, 255);
	sprite_draw(series, id, &load_buffer, 0, 0);

	for (count = 0; count < 16; count++) {
		if (work_area[count][16] != 255) hot_y = count;
		if (work_area[16][count] != 255) hot_x = count;
	}

	if ((hot_x != mouse_hot_x) || (hot_y != mouse_hot_y)) {
		mouse_hot_x = hot_x;
		mouse_hot_y = hot_y;

		mouse_hide();
		mouse_set_hotspot(hot_x, hot_y);
		buffer_rect_copy(load_buffer, mouse_cursor_buffer, 0, 0, 16, 16);
		mouse_show();
	} else {
		mouse_change_cursor_begin();
		buffer_rect_copy(load_buffer, mouse_cursor_buffer, 0, 0, 16, 16);
		mouse_change_cursor_end();
	}
}

} // namespace MADSV2
} // namespace MADS



