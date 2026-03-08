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

#ifndef MADS_CORE_MIDI_H
#define MADS_CORE_MIDI_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/room.h"
#include "mads/madsv2/core/color.h"

namespace MADS {
namespace MADSV2 {

void midi_install(void);
void midi_play(char name[30]);
void midi_stop(void);
void midi_pause(void);
void midi_resume(void);
void midi_loop(void);
void midi_uninstall(void);

} // namespace MADSV2
} // namespace MADS

#endif
