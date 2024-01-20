
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

#ifndef WASTELAND_KEYMAPPING_H
#define WASTELAND_KEYMAPPING_H

#include "common/system.h"

namespace Wasteland {

enum KeybindingAction {
	KEYBIND_ESCAPE,
	KEYBIND_SELECT,

	KEYBIND_VIEW_PARTY1,
	KEYBIND_VIEW_PARTY2,
	KEYBIND_VIEW_PARTY3,
	KEYBIND_VIEW_PARTY4,
	KEYBIND_VIEW_PARTY5,
	KEYBIND_VIEW_PARTY_ALL,

	KEYBIND_NORTH,
	KEYBIND_SOUTH,
	KEYBIND_EAST,
	KEYBIND_WEST,

	KEYBIND_NONE
};

enum KeybindingMode {
	KBMODE_MENUS,       ///< Keys for when showing menus
	KBMODE_MINIMAL,     ///< Keys for when doing text entry
	KBMODE_PARTY_MENUS, ///< Keys for menus that allow switching party members
	KBMODE_NORMAL,      ///< Keys available when normal in-game
	KBMODE_COMBAT       ///< Keys when in combat mode
};

class Keymapping {
public:
	/**
	 * Initialize keymaps
	 */
	static Common::KeymapArray initKeymaps(KeybindingMode mode = KBMODE_MENUS);

	/**
	 * Execute an engine keymap action
	 */
	static void executeAction(KeybindingAction keyAction);

	/**
	 * Sets the current set of actions which are active
	 */
	static void setKeybindingMode(KeybindingMode mode);
};

} // namespace Wasteland

#endif
