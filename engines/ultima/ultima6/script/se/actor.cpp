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

#include "ultima/ultima6/actors/actor_manager.h"
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/script/se/se_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

bool SEScripts::outOfAmmo(Actor *attacker, Obj *weapon, bool printMessage) {
	if (!weapon)
		return false;

	if (weapon->obj_n == 32 && !attacker->inventory_has_object(36, 0, false)) {
		if (printMessage)
			print("Out of darts!\n");
		return true;
	}
	if (weapon->obj_n == 31 && !attacker->inventory_has_object(45, 0, false)) {
		if (printMessage)
			print("Out of arrows!\n");
		return true;
	}
	if (weapon->obj_n == 37 && !attacker->inventory_has_object(26, 0, false)) {
		if (printMessage)
			print("Out of spears!\n");
		return true;
	}
	if (weapon->obj_n == 40 && !attacker->inventory_has_object(41, 0, false)) {
		if (printMessage)
			print("Out of ammunition!\n");
		return true;
	}
	// A bamboo flintlock is loaded on frame 1
	if (weapon->obj_n == 50 && weapon->frame_n == 0) {
		if (printMessage)
			print("Flintlock not loaded!\n");
		return true;
	}

	return false;
}

bool SEScripts::advanceTime(uint16 minutes) {
	GameClock *clock = Game::get_game()->get_clock();
	uint8 minute = clock->get_minute();

	clock->inc_minute(minutes);

	if (minute + minutes >= 60)
		Game::get_game()->get_actor_manager()->updateSchedules(false);

	return true;
}

bool SEScripts::actorGetObj(Actor *actor, Obj *obj, Obj *container) {
	Game *game = Game::get_game();

	if (!game->get_obj_manager()->can_get_obj(obj)) {
		print("\nNot possible.");
		return false;
	}

	if (!game->using_hackmove() && !actor->can_carry_weight(obj)) {
		print("\nThe total is too heavy.");
		return false;
	}

	return game->get_obj_manager()->moveto_inventory(obj, actor);
}

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
