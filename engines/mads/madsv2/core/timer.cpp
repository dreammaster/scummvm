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
#include "mads/madsv2/core/timer.h"

namespace MADS {
namespace MADSV2 {

long *timer_address = dos_timer_address;
word timer_rate = 20;
int  timer_service_active = false;
word timer_sound_on;
word timer_noise_on;
byte timer_copy_protect_in = 0;
byte timer_copy_protect_out = 0;

word timer_low_priority;
word timer_low_semaphore;
word timer_low_stacking;
word timer_low_deferred;
void *timer_low_routine;

long timer_600_low;
long timer_60_low;
long timer_dos_low;


/*
/*      Reads system clock, returns number of ticks since midnight.
*/
long timer_read(void) {
	return (*timer_address);
}


long timer_read_dos(void) {
	return (*dos_timer_address);
}


long timer_read_600(void) {
	return (timer_600_low);
}

long timer_read_60(void) {
	return (timer_60_low);
}

/*
/*      This works around annoying MASM problem requiring us to make
/*      reference to code segment in order to get data segment stuff
/*      linked from an OBJ.  This routine does not exist to be called
/*      but rather to make the required references in case the main
/*      program does not.
*/
void timer_hack(void) {
	timer_install();
	/* pl sound_manager(); */
	/* pl sound_driver_null(); */
	timer_remove();
}


void timer_set_rate(word count_down) {
	warning("TODO: timer_set_rate");
#ifdef TODO
	_asm {
		cli; Interrupts begone

		mov al, 00110110b; Timer chanel 0
		out 43h, al; Declare our intention to reprogram
		mov ax, count_down; Get new countdown value
		out 40h, al; Send low part
		mov al, ah
		out 40h, al; Send high part

		sti; Interrupts come hither
	}
#endif
}

void timer_install() {
	// No implementation in ScummVM
}

void timer_remove() {
	// No implementation in ScummVM
}

void timer_set_sound_flag(int sound_flag) {
	timer_sound_on = sound_flag;
	timer_noise_on = sound_flag;
}

int timer_set_copy_protect(int protect) {
	timer_copy_protect_in = protect;
	timer_copy_protect_out = timer_copy_protect_in;
	return protect;
}

int timer_get_copy_protect() {
	return timer_copy_protect_out;
}

void timer_activate_low_priority(void (*(routine))()) {
	warning("TODO: timer_activate_low_priority");
}

byte *timer_get_interrupt_stack(void) {
	error("TODO: timer_get_interrupt_stack");
}

} // namespace MADSV2
} // namespace MADS
