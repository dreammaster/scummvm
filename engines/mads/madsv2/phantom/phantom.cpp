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

#include "engines/util.h"
#include "mads/madsv2/core/game.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/phantom/phantom.h"
#include "mads/madsv2/phantom/main.h"
#include "mads/madsv2/phantom/sound_phantom.h"
#include "mads/madsv2/phantom/global.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

Common::Error PhantomEngine::run() {
	initGraphics(320, 200);
	_screen = new Graphics::Screen();
	_soundManager = new PhantomSoundManager(_mixer, _soundFlag);
	_soundManager->validate();

	Phantom::phantom_main();

	return Common::kNoError;
}


void PhantomEngine::main_cold_data_init() {
#if 0
	debugger_reset = game_debugger_reset;
	debugger_update = game_debugger;

	game_menu_routine = global_game_menu;
	game_menu_init = global_menu_system_init;
	game_menu_exit = global_menu_system_shutdown;
	game_emergency_save = global_emergency_save;
#endif
	Common::strcpy_s(config_file_name, "config.pha");
	Common::strcpy_s(save_game_key, "phan");
	Common::strcpy_s(restart_game_key, "phantom");

	Common::strcpy_s(player.series_name, "RAL");
	player.walker_must_reload = true;
	player.walker_loads_first = false;
	player.walker_visible = true;
	player.scaling_velocity = true;

	Common::strcpy_s(kernel_cheating_password, "WIDECHEW");
	kernel_cheating_allowed = strlen(kernel_cheating_password);

	kernel.cheating = (byte)kernel_cheating_allowed;
}

void PhantomEngine::global_section_constructor() {
	Phantom::global_section_constructor();
}

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS
