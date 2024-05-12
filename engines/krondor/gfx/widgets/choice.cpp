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

#include "krondor/gfx/widgets/choice.h"
#include "krondor/vars.h"

namespace Krondor {
namespace Gfx {
namespace Widgets {

void Choice::draw() {
	// Get the icon to use
	int index = _requestData->_image;

	if (_requestData->_alt) {
		index = 50;

	} else if (_requestData->_widget == REQ_IMAGEBUTTON ||
			(_requestData->_widget == REQ_CHOICE && _requestData->_alt2)) {
		if (_selected)
			++index;
	} else {
		index += _selected ? 3 : 2;
	}

	const Graphics::ManagedSurface *surf = g_vars->getIcon(index);

	// Draw the icon
	GfxSurface s = getSurface();
	s.blitFrom(*surf);
}

} // namespace Widgets
} // namespace Gfx
} // namespace Krondor
