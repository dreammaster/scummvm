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

#include "legend/xanth/compendium.h"
#include "legend/legend.h"

namespace Legend {
namespace Xanth {

BEGIN_MESSAGE_MAP(Compendium, VisualContainer)
	ON_MESSAGE(ShowMsg)
END_MESSAGE_MAP()

Compendium::Compendium() : VisualContainer() {
	_bgNumber = 0;
	_name = "Compendium";
}

bool Compendium::ShowMsg(CShowMsg &msg) {
	static const CompendiumBounds BOUNDS[6] = {
		{ 2476, 129, 13, 215, 117 },
		{ 4529, 75, 9, 165, 102 },
		{ 1024, 72, 0, 153, 68 },
		{ 1536, 92, 0, 165, 89 },
		{ 1614, 59, 0, 126, 84 },
		{ 3609, 103, 0, 190, 99 }
	};

	Font *font = screen().loadFont(10);
	font->setColor(15);

	bool foundFlag = false;
	Common::Rect bounds;
	for (int idx = 0; idx < 6 && !foundFlag; ++idx) {
		if (BOUNDS[idx]._bgNumber == _bgNumber) {
			foundFlag = true;
			bounds = Common::Rect(BOUNDS[idx]._left, BOUNDS[idx]._top,
				BOUNDS[idx]._right, BOUNDS[idx]._bottom);
		}
	}

	if (foundFlag) {

	} else {

	}

	return true;
}

} // End of namespace Xanth
} // End of namespace Legend
