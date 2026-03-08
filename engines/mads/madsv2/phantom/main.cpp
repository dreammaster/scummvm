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
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/mouse.h"
#include "mads/madsv2/core/quote.h"
#include "mads/madsv2/phantom/main_menu.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

char *quotes;

static void main_menu_main() {
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
#if 0
		kernel_room_startup(922);
#endif
	}
}

void phantom_main() {
	main_menu_main();
}

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS
