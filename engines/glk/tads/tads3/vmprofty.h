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

#ifndef TADS3_VMPROFTY_H
#define TADS3_VMPROFTY_H

/* Basic type definitions for the profiler
 *
 * This defines some basic types for the profiler.  We use a separate header to allow
 * finer-grained inclusions.
 */

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Profiler time record.  We use a 64-bit value for the time; this gives
 *   the time as a delta from some arbitrary zero point, defined by the OS,
 *   as a 64-bit quantity in units defined by the OS.
 *   
 *   We use a 64-bit value to allow for OS-provided timers with very high
 *   precision.  The OS doesn't necessarily have to use the full 64 bits; if
 *   only 32-bit timer values are available, the OS code can simply set the
 *   high-order part to zero.
 */
struct vm_prof_time
{
    /* the high-order and low-order 32 bits of the time value */
    unsigned long hi;
    unsigned long lo;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
