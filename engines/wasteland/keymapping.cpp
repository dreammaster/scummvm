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

#include "common/translation.h"
#include "backends/keymapper/action.h"
#include "backends/keymapper/keymapper.h"
#include "wasteland/keymapping.h"
#include "wasteland/engine.h"

namespace Wasteland {

struct KeybindingRecord {
	KeybindingAction _action;
	const char *_id;
	const char *_desc;
	const char *_key;
	const char *_joy;
};

static const KeybindingRecord MINIMAL_KEYS[] = {
	// I18N: ESC key
	{ KEYBIND_ESCAPE, "ESCAPE", _s("Escape"), "ESCAPE", nullptr },
	{ KEYBIND_NONE, nullptr, nullptr, nullptr, nullptr }
};

static const KeybindingRecord MENU_KEYS[] = {
	// I18N: ESC key
	{ KEYBIND_ESCAPE, "ESCAPE", _s("Escape"), "ESCAPE", nullptr },
	// I18N: Return key
	{ KEYBIND_SELECT, "SELECT", _s("Select"), "RETURN", nullptr },
	{ KEYBIND_NONE, nullptr, nullptr, nullptr, nullptr }
};

static const KeybindingRecord PARTY_KEYS[] = {
	{ KEYBIND_VIEW_PARTY1, "PARTY1", _s("View Party Member 1"), "F1", nullptr },
	{ KEYBIND_VIEW_PARTY2, "PARTY2", _s("View Party Member 2"), "F2", nullptr },
	{ KEYBIND_VIEW_PARTY3, "PARTY3", _s("View Party Member 3"), "F3", nullptr },
	{ KEYBIND_VIEW_PARTY4, "PARTY4", _s("View Party Member 4"), "F4", nullptr },
	{ KEYBIND_VIEW_PARTY5, "PARTY5", _s("View Party Member 5"), "F5", nullptr },
	{ KEYBIND_VIEW_PARTY_ALL, "PARTY_ALL", _s("View All Party Members"), "a", nullptr },
	{ KEYBIND_NONE, nullptr, nullptr, nullptr, nullptr }
};

static const KeybindingRecord NORMAL_KEYS[] = {
	{ KEYBIND_ESCAPE, "ESCAPE", _s("Escape"), "ESCAPE", nullptr },
	{ KEYBIND_SELECT, "SELECT", _s("Select"), "RETURN", nullptr },

	{ KEYBIND_NORTH, "NORTH", _s("Move North"), "UP", nullptr },
	{ KEYBIND_SOUTH, "SOUTH", _s("Move South"), "DOWN", nullptr },
	{ KEYBIND_EAST, "EAST", _s("Move East"), "RIGHT", nullptr },
	{ KEYBIND_WEST, "WEST", _s("Move West"), "LEFT", nullptr },

	{ KEYBIND_NONE, nullptr, nullptr, nullptr, nullptr }
};

static const KeybindingRecord COMBAT_KEYS[] = {
	{ KEYBIND_NONE, nullptr, nullptr, nullptr, nullptr }
};

struct KeysRecord {
	const char *_id;
	const char *_desc;
	const KeybindingRecord *_keys;
};

static const KeysRecord MENU_RECORDS[] = {
	{ "wasteland", _s("Wasteland - Menus"), MENU_KEYS },
	{ nullptr, nullptr, nullptr }
};

static const KeysRecord MINIMAL_RECORDS[] = {
	{ "wasteland_minimal", _s("Wasteland - Minimal Keys"), MINIMAL_KEYS },
	{ nullptr, nullptr, nullptr }
};

static const KeysRecord PARTY_MENU_RECORDS[] = {
	{ "wasteland", _s("Wasteland - Menus"), MENU_KEYS },
	{ "wasteland_party", _s("Wasteland - Party"), PARTY_KEYS },
	{ nullptr, nullptr, nullptr }
};

static const KeysRecord COMBAT_MENU_RECORDS[] = {
	{ "wasteland_combat", _s("Wasteland - Combat"), COMBAT_KEYS },
	{ "wasteland_party", _s("Wasteland - Party"), PARTY_KEYS },
	{ nullptr, nullptr, nullptr }
};

static const KeysRecord NORMAL_RECORDS[] = {
	{ "wasteland", _s("Wasteland"), NORMAL_KEYS },
	{ "wasteland_party", _s("Wasteland - Party"), PARTY_KEYS },
	{ nullptr, nullptr, nullptr }
};

static const KeysRecord *MODE_RECORDS[6] = {
	MENU_RECORDS,
	MINIMAL_RECORDS,
	PARTY_MENU_RECORDS,
	NORMAL_RECORDS,
	COMBAT_MENU_RECORDS,
	nullptr
};

Common::KeymapArray Keymapping::initKeymaps(KeybindingMode mode) {
	Common::KeymapArray keymapArray;
	Common::Keymap *keyMap;
	Common::Action *act;
	const KeysRecord *recPtr = MODE_RECORDS[mode];

	for (int kCtr = 0; recPtr->_id; ++recPtr, ++kCtr) {
		// Core keymaps
		keyMap = new Common::Keymap(Common::Keymap::kKeymapTypeGame,
			recPtr->_id, recPtr->_desc);
		keymapArray.push_back(keyMap);

		for (const KeybindingRecord *r = recPtr->_keys; r->_id; ++r) {
			act = new Common::Action(r->_id, _(r->_desc));
			act->setCustomEngineActionEvent(r->_action);
			act->addDefaultInputMapping(r->_key);
			if (r->_joy)
				act->addDefaultInputMapping(r->_joy);
			if (r->_action == KEYBIND_SELECT)
				act->addDefaultInputMapping("SPACE");

			if (r->_action == KEYBIND_NORTH || r->_action == KEYBIND_SOUTH ||
				r->_action == KEYBIND_EAST || r->_action == KEYBIND_WEST)
				// Allow movement actions to be triggered on keyboard repeats
				act->allowKbdRepeats();

			keyMap->addAction(act);
		}
	}

	return keymapArray;
}

void Keymapping::setKeybindingMode(KeybindingMode mode) {
	Common::Keymapper *const mapper = g_engine->getEventManager()->getKeymapper();
	mapper->cleanupGameKeymaps();

	Common::KeymapArray arr = initKeymaps(mode);

	for (uint idx = 0; idx < arr.size(); ++idx)
		mapper->addGameKeymap(arr[idx]);
}

void Keymapping::executeAction(KeybindingAction keyAction) {
	g_engine->send(ActionMessage(keyAction));
}

} // namespace Wasteland
