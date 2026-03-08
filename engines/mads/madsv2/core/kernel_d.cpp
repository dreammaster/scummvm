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
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/font.h"
#include "mads/madsv2/core/player.h"
#include "mads/madsv2/core/quote.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/kernel_1.h"
#include "mads/madsv2/core/kernel_8.h"
#include "mads/madsv2/core/kernel_n.h"

namespace MADS {
namespace MADSV2 {

#define MESSAGE_COLOR           (((KERNEL_MESSAGE_COLOR_BASE   + 1) << 8) + KERNEL_MESSAGE_COLOR_BASE)

KernelMessage kernel_message[KERNEL_MAX_MESSAGES];
FontPtr kernel_message_font;
int kernel_message_spacing;

void kernel_message_init(void) {
	int count;

	for (count = 0; count < KERNEL_MAX_MESSAGES; count++) {
		kernel_message[count].flags = 0;
	}

	kernel_message_font = font_conv;
	kernel_message_spacing = -1;
}

int kernel_message_add(char *text, int x, int y, int color,
	long time_on_screen, int trigger_code, int flags) {
	int result = -1;
	int id = -1;
	int count;
	KernelMessagePtr my_message = NULL;

	for (count = 0; (id < 0) && (count < KERNEL_MAX_MESSAGES); count++) {
		my_message = &kernel_message[count];
		if (!(my_message->flags & KERNEL_MESSAGE_ACTIVE)) id = count;
	}

	if (id < 0) {
		if (trigger_code) {
			error_report(ERROR_KERNEL_MESSAGE_LIST_FULL, ERROR, MODULE_KERNEL, KERNEL_MAX_MESSAGES, trigger_code);
		}
		goto done;
	}

	my_message->message = text;

	my_message->flags = (KERNEL_MESSAGE_ACTIVE | flags);
	my_message->color = color;
	my_message->x = x;
	my_message->y = y;
	my_message->matte_message_handle = -1;
	my_message->expire_ticks = time_on_screen;
	my_message->update_time = kernel.clock;
	my_message->trigger_code = (byte)trigger_code;
	my_message->trigger_dest = (byte)kernel.trigger_setup_mode;

	for (count = 0; count < 3; count++) {
		my_message->trigger_words[count] = player2.words[count];
	}

	if (flags & KERNEL_MESSAGE_PLAYER) my_message->update_time = player.clock;

	result = id;

done:
	return result;
}

void kernel_message_teletype(int id, int rate, int quote) {
	if (id >= 0) {
		kernel_message[id].flags |= KERNEL_MESSAGE_TELETYPE;
		if (quote) kernel_message[id].flags |= KERNEL_MESSAGE_QUOTE;
		kernel_message[id].strobe_marker = 0;
		kernel_message[id].strobe_rate = rate;
		kernel_message[id].strobe_time = kernel.clock;
		kernel_message[id].strobe_save = *kernel_message[id].message;
		kernel_message[id].strobe_save_2 = *(kernel_message[id].message + 1);
		if (kernel_message[id].flags & KERNEL_MESSAGE_PLAYER) {
			kernel_message[id].strobe_time = player.clock;
		}
		kernel_message[id].update_time = kernel_message[id].strobe_time;
	}
}

void kernel_message_attach(int id, int sequence) {
	if (id >= 0) {
		kernel_message[id].flags |= KERNEL_MESSAGE_ATTACHED;
		kernel_message[id].sequence_id = (byte)sequence;
	}
}

void kernel_message_anim(int id, int anim, int segment) {
	if (id >= 0) {
		kernel_message[id].flags |= KERNEL_MESSAGE_ANIM;
		kernel_message[id].sequence_id = (byte)anim;
		kernel_message[id].segment_id = (byte)segment;
	}
}

void kernel_message_delete(int id) {
	if (kernel_message[id].flags & KERNEL_MESSAGE_ACTIVE) {
		if (kernel_message[id].flags & KERNEL_MESSAGE_TELETYPE) {
			kernel_message[id].message[kernel_message[id].strobe_marker] = kernel_message[id].strobe_save;
			kernel_message[id].message[kernel_message[id].strobe_marker + 1] = kernel_message[id].strobe_save_2;
		}
		if (kernel_message[id].matte_message_handle >= 0) {
			matte_clear_message(kernel_message[id].matte_message_handle);
		}
		kernel_message[id].flags &= ~KERNEL_MESSAGE_ACTIVE;
	}
}

void kernel_message_purge(void) {
	int count;
	for (count = 0; count < KERNEL_MAX_MESSAGES; count++) {
		kernel_message_delete(count);
	}

	kernel_random_purge();
}

int kernel_message_player(int quote_id, long delay, int trigger) {
	int id;

	id = kernel_message_add(quote_string(kernel.quotes, quote_id),
		0, 0, MESSAGE_COLOR, delay, trigger,
		KERNEL_MESSAGE_PLAYER | KERNEL_MESSAGE_CENTER);

	return id;
}

} // namespace MADSV2
} // namespace MADS
