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

#include "legend/xanth/background.h"
#include "legend/legend.h"

namespace Legend {
namespace Xanth {

BEGIN_MESSAGE_MAP(Background, VisualContainer)
	ON_MESSAGE(ShowMsg)
END_MESSAGE_MAP()

int Background::_bgPicNum;

bool Background::ShowMsg(CShowMsg &msg) {
	// Set the background picture number to match the common background Id
	load(_bgPicNum, 0);

	return VisualContainer::ShowMsg(msg);
}

} // End of namespace Xanth
} // End of namespace Legend
