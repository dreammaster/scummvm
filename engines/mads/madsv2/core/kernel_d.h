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

#ifndef MADS_CORE_KERNEL_D_H
#define MADS_CORE_KERNEL_D_H

#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/font.h"

namespace MADS {
namespace MADSV2 {

extern KernelMessage kernel_message[KERNEL_MAX_MESSAGES];
extern FontPtr kernel_message_font;
extern int kernel_message_spacing;

extern void kernel_message_init(void);
extern int kernel_message_add(char far *text, int x, int y,
	int color, long time_on_screen, int trigger_code, int flags);
extern void kernel_message_teletype(int id, int rate, int quote);
extern void kernel_message_attach(int id, int sequence);
extern void kernel_message_delete(int id);
extern void kernel_message_purge(void);

} // namespace MADSV2
} // namespace MADS

#endif
