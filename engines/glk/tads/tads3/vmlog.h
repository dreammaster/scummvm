/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef TADS3_VMLOG_H
#define TADS3_VMLOG_H

/* System log file
 *
 * Functions for writing messages to the system log file
 *
 * Strings passed to vm_log_xxx functions are expected to be in the local file system
 * character set, unless otherwise noted per function.  These functions generally just
 * write the bytes of the strings as given, without any character set mapping, so it's
 * up to the caller to map to the file character set if necessary.  Plain ASCII strings
 * don't need mapping, since virtually all modern machines use character sets that
 *  include ASCII as a subset - this includes UTF-8, ISO-8859-X, Win CP12xx, etc,
 * but excludes 16-bit Unicode mappings (UCS-2) and EBCDIC.
 */

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmglob.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* log a message with sprintf-style formatting */
void vm_log_fmt(VMG_ const char *fmt, ...);

/* log a message string */
void vm_log(VMG_ const char *str, size_t len);

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif

