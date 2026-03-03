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

#ifndef MADS_CORE_TIMER_H
#define MADS_CORE_TIMER_H

#include "common/scummsys.h"

namespace MADS {
namespace MADSV2 {

extern long *timer_address;         /* "Default" timer count address */
extern word timer_rate;                 /* Current timer ticks/sec       */
extern int  timer_service_active;       /* Flag if timer service active  */

extern word timer_sound_on;             /* Flag if sound service active */
extern word timer_noise_on;             /* Flag if sound in noise mode  */

extern word timer_low_priority;         /* Flag if low priority routine active */
extern word timer_low_semaphore;        /* Low priority activity semaphore     */
extern word timer_low_stacking;         /* Low priority stacking count         */
extern word timer_low_deferred;         /* Low priority deferred DOS flag      */
extern void *timer_low_routine;     /* Low priority function pointer       */

extern long timer_600_low;             /* 600/s timer count */
extern long timer_60_low;              /* 60/s timer_count  */
extern long timer_dos_low;


/* extern byte *interrupt_stack_pointer; */
/* extern word interrupt_stack_size;         */

/* timer_1.c */
long timer_read(void);
long timer_read_dos(void);
long timer_read_600(void);
long timer_read_60(void);


/* timer_2.c */
void timer_set_rate(word count_down);


/* timer_3.asm */
void timer_install(void);
void timer_remove(void);
void timer_set_sound_flag(int sound_flag);
void timer_activate_low_priority(void (*(routine))());
byte *timer_get_interrupt_stack(void);

int timer_set_copy_protect(int protect);
int timer_get_copy_protect(void);

} // namespace MADSV2
} // namespace MADS

#endif
