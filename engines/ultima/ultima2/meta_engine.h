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

#ifndef ULTIMA2_META_ENGINE
#define ULTIMA2_META_ENGINE

#include "backends/keymapper/keymapper.h"

namespace Ultima {
namespace Ultima2 {

enum KeybindingAction {
	KEYBIND_UP, KEYBIND_DOWN, KEYBIND_LEFT, KEYBIND_RIGHT,
	KEYBIND_NONE
};

enum KeybindingMode {
	KBMODE_NORMAL		///< Keys available when normal in-game
};

class MetaEngine {
private:
	/**
	 * Get the method to execute
	 */
	static Common::String getMethod(KeybindingAction keyAction);

	/**
	 * Adds the default actions for the mouse buttons
	 */
	static void addMouseClickActions(Common::Keymap &keyMap);
public:
	/**
	 * Initialize keymaps
	 */
	static Common::KeymapArray initKeymaps(KeybindingMode mode = KBMODE_NORMAL);

	/**
	 * Execute an engine keymap action
	 */
	static void executeAction(KeybindingAction keyAction);

	/**
	 * Sets the current set of actions which are active
	 */
	static void setKeybindingMode(KeybindingMode mode);
};

} // End of namespace Ultima2
} // End of namespace Ultima

#endif
