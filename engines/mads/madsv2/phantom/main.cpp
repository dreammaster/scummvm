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

#include "mads/madsv2/phantom/main.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/magic.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/mcga.h"
#include "mads/madsv2/core/mouse.h"
#include "mads/madsv2/core/pal.h"
#include "mads/madsv2/core/quote.h"
#include "mads/madsv2/phantom/main_menu.h"
#include "mads/madsv2/engine.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

constexpr bool SHOW_LINES = true;
constexpr byte LINE_COLOR = 2;

char *quotes;

static void main_menu_main() {
	auto &screen = *g_engine->getScreen();
	Palette palette;

	if (kernel_game_startup(19, KERNEL_STARTUP_CURSOR | KERNEL_STARTUP_INTERRUPT | KERNEL_STARTUP_FONT,
		nullptr, nullptr)) {
		viewing_at_y = (200 - scr_work.y) >> 1;

		mouse_cursor_sprite(cursor, 7);
		mouse_show();
		mouse_force(280, 126);
		mouse_hide();

		mouse_cursor_sprite(cursor, 1);
		matte_init(0xFFFF);
		kernel_seq_init();
		kernel_message_init();
		kernel_animation_init();
		kernel_init_dynamic();

		picture_view_x = 0;
		picture_view_y = 0;

		quotes = quote_load(0, 68, 69, 70, 71, 72, 73, 74, 75, 76,
			77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
			87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
			97, 98, 99, 0);

		global_speech_load(9);
		bool valid = !kernel_room_startup(922);

		master_palette[4].r = 63;
		master_palette[4].g = 50;
		master_palette[4].b = 0;
		master_palette[5].r = 30;
		master_palette[5].g = 15;
		master_palette[5].b = 0;
		mcga_setpal_range(&master_palette, 4, 2);

		new_background = true;

		if (valid) {
			memset(&palette, 0, sizeof(palette));
			mcga_setpal(&palette);
			mouse_cursor_sprite(cursor, 1);

			if (SHOW_LINES && viewing_at_y != 0) {
				screen.hLine(0, viewing_at_y - 2, 319, LINE_COLOR);
				screen.hLine(0, scr_work.y + viewing_at_y + 1, 319, LINE_COLOR);
			}

			kernel_load_sound_driver("*#SOUND.PH9", 'N', 544, 0, 49);

			menu_control();

			if (selected_item >= 0) {
				// Zero out the first 3 entries of both magic color arrays
				for (int i = 0; i < 3; i++) {
					magic_color_values[i] = 0;
					magic_color_flags[i] = 0;
				}

				mcga_getpal(&palette);

				magic_fade_to_grey(palette, 0, 0x10, 1, 1, 0, 0, 0);
			}
		}

		free(quotes);
		kernel_unload_sound_driver();
		kernel_game_shutdown();
	}

	mcga_reset();

	// Handle menu item selection
	warning("Selected item = %d", selected_item);
}

void phantom_main() {
	pack_enable_pfab_explode();
	if (!env_verify())
		env_search_mode = ENV_SEARCH_CONCAT_FILES;

	main_menu_main();
}

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS
