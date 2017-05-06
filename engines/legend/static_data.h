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

#ifndef LEGEND_STATIC_DATA_H
#define LEGEND_STATIC_DATA_H

#include "common/scummsys.h"

namespace Legend {

/**
 * 16x16 1bpp mouse cursors
 */
struct MouseCursorData {
	int _hotspotX, _hotspotY;
	uint16 _pixels[16];
	uint16 _masks[16];
};
	
namespace Early {

struct HelpMessage {
	const char *_msg1;
	const char *_msg2;
};
extern const HelpMessage HELP_TEXT[38];

} // End of namespace Early

namespace Later {
namespace Xanth {

extern const MouseCursorData XANTH_CURSORS[4];

} // End of namespaec Xanth
} // ENd of namespace Later


} // End of namespace Legend

#endif
