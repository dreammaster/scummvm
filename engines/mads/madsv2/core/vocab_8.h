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

#ifndef MADS_CORE_VOCAB_8_H
#define MADS_CORE_VOCAB_8_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

extern void vocab_unload_active();  /* vocab_8.c */
extern void vocab_init_active();
extern int vocab_active_id(word id);
extern int vocab_make_active(word id);
extern int vocab_load_active();

extern char *vocab_text;
extern word vocab_size;
extern word vocab_active;
extern word vocab_list_id[VOCAB_MAX_ACTIVE];
extern word vocab_list_pointer[VOCAB_MAX_ACTIVE];

} // namespace MADSV2
} // namespace MADS

#endif
