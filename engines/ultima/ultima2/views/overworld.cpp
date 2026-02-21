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

#include "ultima/ultima2/views/overworld.h"
#include "ultima/ultima2/ultima2.h"
#include "ultima/shared/early/core/rect.h"

namespace Ultima {
namespace Ultima2 {
namespace Views {

Overworld::Overworld() : View("Overworld") {
	_map.setBounds(Shared::Core::TextRect(0, 0, 40, 20));
	_commands.setBounds(Shared::Core::TextRect(0, 20, 30, 24));
	_stats.setBounds(Shared::Core::TextRect(30, 20, 40, 24));
}

bool Overworld::msgFocus(const FocusMessage &msg) {
	const auto &player = g_engine->_player;
	auto &map = g_engine->_map;
	map.load(player._mapNum);

	map._mapX = player._mapX;
	map._mapY = player._mapY;
	map._playerTileId = (player._class + Data::PLAYER_TILES_OFFSET) * 2;

	return View::msgFocus(msg);
}

} // namespace Views
} // namespace Ultima2
} // namespace Ultima
