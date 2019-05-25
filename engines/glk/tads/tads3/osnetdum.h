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

#ifndef TADS3_OSNETDUM_H
#define TADS3_OSNETDUM_H

/* Dummy implementation of TADS networking for non-supporting
 *
 * This module provides a stub implementation of the portions of the portable networking
 * interface that are required to link the rest of the system.  This should be used on
 * platforms that don't provide TADS networking functionality.
*/

namespace Glk {
namespace TADS {
namespace TADS3 {

class OS_Counter
{
public:
    OS_Counter(long c = 1) { cnt = c; }
    long get() const { return cnt; }
    long inc() { return ++cnt; }
    long dec() { return --cnt; }

private:
    long cnt;
};

#include "glk/tads/tads3/vmrefcnt.h"

class OS_Event: public CVmRefCntObj
{
public:
    OS_Event(int /*manual_reset*/) { }
    void signal() { }
    void reset() { }
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
