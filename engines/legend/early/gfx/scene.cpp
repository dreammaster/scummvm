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

#include "legend/early/gfx/scene.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {

EMPTY_MESSAGE_MAP(Scene, Gfx::VisualItem);

Scene::Scene() : Gfx::VisualContainer("Scene"),
		_commandButtons("CommandButtons", Common::Rect(0, 0, 120, 136)),
		_compass("Compass", Common::Rect(120, 0, 260, 136)),
		_scenePicture("ScenePicture", Common::Rect(260, 0, 640, 232)),
		_commands("Commands", Common::Rect(0, 136, 117, 480)),
		_items("Items", Common::Rect(116, 136, 260, 480)),
		_textArea("TextArea", Common::Rect(260, 232, 640, 480)) {
	_commandButtons.addUnder(this);
	_compass.addUnder(this);
	_scenePicture.addUnder(this);
	_commands.addUnder(this);
	_items.addUnder(this);
	_textArea.addUnder(this);

	_commands.load("GATEWAY/COMMANDS");
	_commands.setDividerIndex(16);
}

} // End of namespace Early
} // End of namespace Legend
