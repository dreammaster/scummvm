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

#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/se/se_scripts.h"
#include "ultima/ultima6/usecode/usecode.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

bool SEScripts::lookObj(Obj *obj) {
	Game *game = Game::get_game();

	printfl("YOU_SEE", game->get_obj_manager()->look_obj(obj, true));

	if (game->get_usecode()->look_obj(obj, game->get_player()->get_actor())) {
		print("\n");
		return false;
	}

	print(".\n\n");

	if (isContainerObj(obj->obj_n))
		search(obj);

	return false;
}

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
