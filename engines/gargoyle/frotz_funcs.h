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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm-misc.svn.sourceforge.net/svnroot/scummvm-misc/trunk/engines/gargoyle/frotz_funcs.h $
 * $Id: frotz_funcs.h 1 2009-09-21 10:46:54Z dreammaster $
 *
 */

#ifndef FROTZ_FUNCS_H
#define FROTZ_FUNCS_H

#include "gargoyle/frotz.h"

namespace Frotz {

long mem_diff(zbyte *a, zbyte *b, zword mem_size, zbyte *diff);

void mem_undiff(zbyte *diff, long diff_length, zbyte *dest);

} // end of namespace Frotz

#endif
