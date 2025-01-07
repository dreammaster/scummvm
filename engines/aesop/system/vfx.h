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

#ifndef AESOP_SYSTEM_VFX_H
#define AESOP_SYSTEM_VFX_H

#include "common/scummsys.h"

namespace Aesop {

struct VFX_DESC {
	int scrn_width = 0;
	int scrn_height = 0;
};

typedef void *WINDOW;
typedef void *PANE;

extern VFX_DESC *VFX;

inline void VFX_wait_vblank_leading() {}

} // namespace Aesop

#endif
