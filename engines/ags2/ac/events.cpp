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

#include "ags2/ac/events.h"
#include "ags2/vars.h"

namespace AGS2 {

void set_mouse_cursor(int newcurs) {
	int hotspotx = game.mcurs[newcurs].hotx, hotspoty = game.mcurs[newcurs].hoty;

	set_new_cursor_graphic(game.mcurs[newcurs].pic);
	if (dotted_mouse_cursor) {
		wfreeblock(dotted_mouse_cursor);
		dotted_mouse_cursor = NULL;
	}

	if ((newcurs == MODE_USE) && (game.mcurs[newcurs].pic > 0) &&
		((game.hotdot > 0) || (game.invhotdotsprite > 0))) {
		// If necessary, create a copy of the cursor and put the hotspot
		// dot onto it
		dotted_mouse_cursor = create_bitmap_ex(bitmap_color_depth(mousecurs[0]), mousecurs[0]->w, mousecurs[0]->h);
		blit(mousecurs[0], dotted_mouse_cursor, 0, 0, 0, 0, mousecurs[0]->w, mousecurs[0]->h);

		if (game.invhotdotsprite > 0) {
			block abufWas = abuf;
			abuf = dotted_mouse_cursor;

			draw_sprite_support_alpha(
				hotspotx - spritewidth[game.invhotdotsprite] / 2,
				hotspoty - spriteheight[game.invhotdotsprite] / 2,
				spriteset[game.invhotdotsprite],
				game.invhotdotsprite);

			abuf = abufWas;
		} else {
			putpixel_compensate(dotted_mouse_cursor, hotspotx, hotspoty,
				(bitmap_color_depth(dotted_mouse_cursor) > 8) ? get_col8_lookup(game.hotdot) : game.hotdot);

			if (game.hotdotouter > 0) {
				int outercol = game.hotdotouter;
				if (bitmap_color_depth(dotted_mouse_cursor) > 8)
					outercol = get_col8_lookup(game.hotdotouter);

				putpixel_compensate(dotted_mouse_cursor, hotspotx + get_fixed_pixel_size(1), hotspoty, outercol);
				putpixel_compensate(dotted_mouse_cursor, hotspotx, hotspoty + get_fixed_pixel_size(1), outercol);
				putpixel_compensate(dotted_mouse_cursor, hotspotx - get_fixed_pixel_size(1), hotspoty, outercol);
				putpixel_compensate(dotted_mouse_cursor, hotspotx, hotspoty - get_fixed_pixel_size(1), outercol);
			}
		}
		mousecurs[0] = dotted_mouse_cursor;
		update_cached_mouse_cursor();
	}
	msethotspot(hotspotx, hotspoty);
	if (newcurs != cur_cursor)
	{
		cur_cursor = newcurs;
		mouse_frame = 0;
		mouse_delay = 0;
	}
}


} // namespace AGS2
