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

#include "legend/early/gfx/interface.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {
namespace Gfx {

Interface::Interface(UIElement *parent) : ViewElement(parent, "Interface"),
		_commandButtons(parent, "CommandButtons", Common::Rect(0, 0, 120, 136)),
		_compass(parent, "Compass", Common::Rect(120, 0, 260, 136)),
		_scene(parent, "ScenePicture", Common::Rect(260, 0, 640, 232)),
		_commands(parent, "Commands", Common::Rect(0, 136, 117, 480)),
		_items(parent, "Items", Common::Rect(117, 136, 260, 480)),
		_textArea(parent, "TextArea", Common::Rect(260, 232, 640, 480)) {
	_commands.load("commands.bin");
	_commands.setDividerIndex(16);
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
