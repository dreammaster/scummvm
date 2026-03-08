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

#ifndef MADS_CORE_LOCK_H
#define MADS_CORE_LOCK_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

extern char *lock_program_name;
extern char *lock_search_mark;
extern int  lock_search_length;
extern word *lock_hash_value;

/* lock_2.cpp */
word lock_get_disk_hash(void);

/* lock_3.cpp */
long lock_search_hash_offset(void);

/* lock_4.cpp */
void lock_write_new_hash(void);

/* lock_5.cpp */
int lock_get_copy_code(void);

/* lock_6.cpp */
void lock_short_protection_check(void);

/* lock_7.cpp */
int  lock_verification(void);
void lock_sabotage(void);

/* lock_8.cpp */
void lock_long_protection_check(void);

/* lock_9.cpp */
void lock_secret_protection_check(void);

/* lock_a.cpp */
void lock_preliminary_check(void);

} // namespace MADSV2
} // namespace MADS

#endif
