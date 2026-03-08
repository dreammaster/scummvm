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

#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/buffer.h"
#include "mads/madsv2/core/font.h"
#include "mads/madsv2/core/video.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/inter.h"
#include "mads/madsv2/core/mouse.h"
#include "mads/madsv2/core/object.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/screen.h"
#include "mads/madsv2/core/ems.h"
#include "mads/madsv2/core/himem.h"
#include "mads/madsv2/core/echo.h"
#include "mads/madsv2/core/mcga.h"
#include "mads/madsv2/core/timer.h"
#include "mads/madsv2/core/cycle.h"
#include "mads/madsv2/core/player.h"
#include "mads/madsv2/core/keys.h"
#include "mads/madsv2/core/pack.h"
#include "mads/madsv2/core/room.h"
#include "mads/madsv2/core/demo.h"
#include "mads/madsv2/core/xms.h"
#include "mads/madsv2/core/lock.h"
#include "mads/madsv2/core/tile.h"
#include "mads/madsv2/core/popup.h"
#include "mads/madsv2/core/pal.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/fileio.h"
#include "mads/madsv2/core/vocab.h"
#include "mads/madsv2/core/kernel_1.h"
#include "mads/madsv2/core/midi.h"
#include "mads/madsv2/core/digi.h"

namespace MADS {
namespace MADSV2 {

/*
/*      kernel_game_shutdown()
/*
/*      Game level system shutdown.
*/
void kernel_game_shutdown() {
	int check_mode;


	sprite_free(&box_param.menu, true);
	sprite_free(&box_param.logo, true);
	sprite_free(&box_param.series, true);

	buffer_free(&scr_inter_orig);

	vocab_unload_active();

	/* Drop cursor */

	if (cursor != NULL) mem_free(cursor);
	cursor = NULL;

	/* Free main video work buffer */

	pack_set_special_buffer(NULL, NULL);

	object_unload();
	/* inter_deallocate_objects(); */

	popup_available = false;

	/* Remove special keyboard handler */

	keys_remove();

	/* Unload interface fonts */

	if (font_misc != NULL) mem_free(font_misc);
	if (font_menu != NULL) mem_free(font_menu);
	if (font_conv != NULL) mem_free(font_conv);
	if (font_inter != NULL) mem_free(font_inter);
	if (font_main != NULL) mem_free(font_main);

	font_main = font_conv = font_inter = NULL;

	/* Deallocate main screen buffer */

	if (work_screen_ems_handle < 0) buffer_free(&scr_main);

	/* Turn of speech system */

	/* pl if (speech_system_active) speech_shutdown(); */

	/* Return video to text mode */

	mouse_hide();
	check_mode = video_mode;
	mouse_init(false, text_mode);
	video_init(text_mode, (check_mode != text_mode));

	/* Deallocate EMS/XMS memory */

	himem_shutdown();

	/* Remove timer interrupt stuff */

	midi_uninstall();
	digi_uninstall();

	timer_activate_low_priority(NULL);

	timer_remove();

	/* mem_free (interrupt_stack_pointer); */
}


void kernel_force_refresh(void) {
	int count;
	int purge_flag = true;

	for (count = 0; count < (int)image_marker; count++) {
		if (image_list[count].flags == IMAGE_REFRESH) {
			purge_flag = false;
		}
	}

	if (purge_flag) matte_refresh_work();
}



static char digital_name[12] = "digital.aga";

/*
/*      kernel_game_startup()
/*
/*      Game level system startup.
*/
int kernel_game_startup(int game_video_mode, int load_flag,
	char *release_version, char *release_date) {
	int error_flag = true;
	int count, count2;
	int ems_temp;
	int ems_error;
	int pages;
	int reserve[EMS_PAGING_CLASSES];
	byte *interrupt_stack;
#ifdef demo
	char temp_buf[20];
#endif
#ifndef disable_error_check
	int error_code = 0;
#endif

	/*
	interrupt_stack_pointer = mem_get_name(KERNEL_INTERRUPT_STACK_SIZE, "$istack$");
	if (interrupt_stack_pointer == NULL) goto done;

	interrupt_stack_size = KERNEL_INTERRUPT_STACK_SIZE;
	*/

	/* Set up EMS/XMS paging system, if any */

	himem_startup();

	ems_error = true;

	if (ems_exists) {
		work_screen_ems_handle = ems_get_page_handle(4);
		if (work_screen_ems_handle >= 0) {
			ems_error = false;
		}
	}

	if (tile_setup()) ems_error = true;

	if (ems_error) {
		if (ems_exists) {
			error_report(ERROR_NO_MORE_EMS, SEVERE, MODULE_KERNEL, ems_pages, work_screen_ems_handle);
		} else {
			error_report(ERROR_KERNEL_NO_EMS, SEVERE, MODULE_KERNEL, ems_exists, work_screen_ems_handle);
		}
	}

	/* pl  if (speech_system_requested) {
		char digital_search[20];
		char digital_path[80];
		speech_ems_status = SPEECH_DELUXE;
		digital_search[0] = 0;
		if (!fileio_exist(digital_name)) {
		  strcat (digital_search, "*");
		}
		strcat (digital_search, digital_name);
		env_get_path (digital_path, digital_search);
		speech_init (digital_path, speech_board_address,
								  speech_board_type,
								  speech_board_irq,
								  speech_board_drq);
	  }
	  */

	if (ems_exists) {
		if (load_flag & KERNEL_STARTUP_POPUP) {
			object_ems_handle = ems_get_page_handle(4);

			ems_temp = ems_get_page_handle(4);
			if (ems_temp >= 0) popup_preserve_initiator[0] = ems_temp;

			ems_temp = ems_get_page_handle(4);
			if (ems_temp >= 0) popup_preserve_initiator[1] = ems_temp;

			ems_temp = ems_get_page_handle(4);
			if (ems_temp >= 0) popup_preserve_initiator[2] = ems_temp;
		}
	}


	if (ems_exists) {
		pages = ems_pages_free;
		for (count = 0; count < EMS_PAGING_CLASSES; count++) {
			reserve[EMS_PAGING_CLASSES] = 0;
		}
		if (pages >= 4) {
			reserve[EMS_PAGING_SYSTEM] = 4;
			pages -= 4;
		}
		ems_temp = MIN(pages >> 1, 64);
		reserve[EMS_PAGING_ROOM] = ems_temp;
		pages -= ems_temp;

		ems_temp = pages >> 2;
		reserve[EMS_PAGING_SECTION] = ems_temp;
		pages -= ems_temp;

		for (count = 0; count < EMS_PAGING_CLASSES; count++) {
			ems_paging_reserve[count] = 0;
			for (count2 = count - 1; count2 >= 0; count2--) {
				ems_paging_reserve[count] += reserve[count2];
			}
		}
	}

	/* Some preliminary copy protection stuff */

	/* lock_preliminary_check(); */

	/* Initialize sound driver jump table */

	/* pl sound_driver_null(); */
	timer_set_sound_flag(0);

	/* Video initialization */

	screen_dominant_mode(game_video_mode);
	video_init(game_video_mode, (game_video_mode != text_mode));
	mouse_init(true, game_video_mode);

	if (game_video_mode == mcga_mode) {
		mcga_compute_retrace_parameters();
	}

	/* Initialize the main screen work buffer & its sub-buffers */

	if (work_screen_ems_handle >= 0) {
		scr_main.x = video_x;
		scr_main.y = video_y;
		scr_main.data = ems_page[0];
		ems_map_buffer(work_screen_ems_handle);
	} else {
		buffer_init_name(&scr_main, video_x, video_y, "$scrmain");
	}
	if (scr_main.data == NULL) {
#ifndef disable_error_check
		error_code = ERROR_NO_MORE_MEMORY;
#endif
		goto done;
	}

	scr_work.x = scr_inter.x = video_x;
	scr_work.y = display_y;
	scr_inter.y = inter_size_y;

	scr_work.data = scr_main.data;
	scr_inter.data = (byte *)mem_normalize(buffer_pointer(&scr_main, 0, inter_base_y));

	buffer_fill(scr_main, 0);

	/* Load the main interface fonts */

	if (load_flag & KERNEL_STARTUP_FONT) {
		font_main = font_load("*FONTMAIN.FF");
		font_inter = font_load("*FONTINTR.FF");
		font_conv = font_load("*FONTCONV.FF");
		font_menu = font_load("*FONTMENU.FF");
		font_misc = font_load("*FONTMISC.FF");

		if ((font_main == NULL) || (font_inter == NULL) ||
			(font_conv == NULL) || (font_menu == NULL) ||
			(font_misc == NULL)) {
#ifndef disable_error_check
			error_code = ERROR_KERNEL_NO_FONTS;
#endif
			goto done;
		}
	}


	/* Install timer handler & low priority cycling interrupt */

	if (load_flag & KERNEL_STARTUP_INTERRUPT) {
		timer_install();

		midi_install();
		digi_install();

		/*
		if (!lock_verification()) {
		  error_report (ERROR_COPY_PROTECTION, SEVERE, MODULE_LOCK, 0, 0);
		}
		timer_remove();
		timer_install();
		*/

		cycling_active = false;
		timer_activate_low_priority(cycle_colors);
		keys_install();
	}

	/* Log in demo copy */

#ifdef demo
	if (game_video_mode != text_mode) demo_log_in(release_version, release_date);
#endif

	/* Mention EMS paging situation */

#ifdef demo
	if (ems_paging_active) {
		ltoa(((long)ems_pages * EMS_PAGE_SIZE) >> 10, temp_buf, 10);
		echo(temp_buf, false);
		echo("K of EMS memory available.", true);
	} else {
		echo("EMS memory not available.", true);
	}

	if (xms_exists) {
		echo("XMS memory system detected.", true);
	}
#endif

	/* Load the objects list */

	if (load_flag & KERNEL_STARTUP_OBJECTS) {
		/* inter_allocate_objects(); */
		if (object_load()) {
#ifndef disable_error_check
			error_code = ERROR_KERNEL_NO_OBJECTS;
#endif
			goto done;
		}
		if (inven_num_objects > 0) {
			active_inven = 0;
		}
	}

	/* Allow packing routines to use lower interrupt stack */

	interrupt_stack = timer_get_interrupt_stack();
	pack_set_special_buffer(interrupt_stack, NULL);

	/* Initialize player data structures */

	if (load_flag & KERNEL_STARTUP_PLAYER) player_init();

	popup_available = true;

	/* video_update (&scr_main, 0, 0, 0, 0, video_x, video_y); */

	Common::strcpy_s(box_param.name, "*BOX.SS");

	if (load_flag & KERNEL_STARTUP_CURSOR) {
		/* Wipe palette & prepare for cursor */
		pal_init(KERNEL_RESERVED_LOW_COLORS, KERNEL_RESERVED_HIGH_COLORS);
		pal_white(master_palette);
		if (video_mode == mcga_mode) {
			mcga_setpal_range(&master_palette, 0, 4);
		}

		/* Load cursor sprite series */

		cursor = sprite_series_load("*CURSOR.SS", PAL_MAP_RESERVED | PAL_MAP_DEFINE_RESERVED);
		if (cursor == NULL) {
#ifndef disable_error_check
			error_code = ERROR_KERNEL_NO_CURSOR;
#endif
			goto done;
		}

		/* Activate main cursor sprite as mouse cursor */

		cursor_last = cursor_id = (cursor->num_sprites > 1) ? 2 : 1;
		mouse_cursor_sprite(cursor, cursor_id);
	}

	if (load_flag & KERNEL_STARTUP_VOCAB) {
		vocab_load_active();
	}

	if (load_flag & KERNEL_STARTUP_INTERFACE) {
		buffer_init_name(&scr_inter_orig, video_x, inter_size_y, "$scrintr");
		if (scr_inter_orig.data == NULL) {
			error_code = ERROR_NO_MORE_MEMORY;
			goto done;
		}
	}

	if (load_flag & KERNEL_STARTUP_POPUP) {
		if (popup_box_load()) {
			error_code = ERROR_KERNEL_NO_POPUP;
			goto done;
		}
	}

	error_flag = false;

done:
	if (load_flag & KERNEL_STARTUP_CURSOR_SHOW) mouse_show();
	if (error_flag) {
#ifndef disable_error_check
		error_check_memory();
		error_report(error_code, ERROR, MODULE_KERNEL, 0, 0);
#endif
		kernel_game_shutdown();
	}
	return (error_flag);
}

} // namespace MADSV2
} // namespace MADS
