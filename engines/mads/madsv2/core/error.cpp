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

#include "common/textconsole.h"
#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/mouse.h"
#include "mads/madsv2/core/screen.h"
#include "mads/madsv2/core/echo.h"
#include "mads/madsv2/core/ems.h"
#include "mads/madsv2/core/himem.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/screen.h"
#include "mads/madsv2/core/timer.h"
#include "mads/madsv2/core/popup.h"
#include "mads/madsv2/core/keys.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/midi.h"
#include "mads/madsv2/core/digi.h"

namespace MADS {
namespace MADSV2 {

#define disable_popup_errors DISABLED

void (*error_service_routine)() = NULL;
void (*error_service_routine_2)() = NULL;

int  error_abort = WARNING;
char error_string[80] = "";

static void error_explode(char *error_buf, char *module_buf, char *data1_buf, char *data2_buf, long avail, int error) {
	midi_uninstall();
	digi_uninstall();

	timer_remove();
	keys_remove();
	himem_shutdown();

	mouse_init(false, 3);
	screen_dominant_mode(text_mode);

	::error("Execution aborted");
}


#pragma optimize ("egl", on)

void error_report(int error, int severity, int module, long data1, long data2) {
	char error_buf[40], module_buf[40], data1_buf[12], data2_buf[12];
	int handled = false;
	int key;

	if (severity >= error_abort) {
		mads_itoa(error, error_buf, 10);
		mads_itoa(module, module_buf, 10);
		ltoa(data1, data1_buf, 10);
		ltoa(data2, data2_buf, 10);

		if (!handled) {
			error_explode(error_buf, module_buf, data1_buf, data2_buf, mem_get_avail(), error);
		}
	}
}

void error_break_point(int data1, int data2) {
	error_report(ERROR_BREAK_POINT, SEVERE, MODULE_UNKNOWN, data1, data2);
}

void error_watch_point(char *message, long data1, long data2) {
	char temp_buf[80];
	int len;
	int width;

	len = strlen(message);

	width = MAX(len, 12);

	if (popup_create(popup_estimate_pieces(width), POPUP_CENTER, POPUP_CENTER)) goto done;

	popup_center_string(message, true);
	popup_next_line();
	Common::sprintf_s(temp_buf, "DATA: %ld %ld", data1, data2);
	popup_center_string(temp_buf, false);
	popup_next_line();
	popup_center_string("HIT ANY KEY", false);

	popup_and_wait(true);

done:
	;
}

void error_check_memory(void) {
	if (mem_last_alloc_failed) {
		error_report(ERROR_NO_MORE_MEMORY, ERROR, mem_last_alloc_loader,
			mem_last_alloc_size, mem_last_alloc_avail);
	}
}

} // namespace MADSV2
} // namespace MADS
