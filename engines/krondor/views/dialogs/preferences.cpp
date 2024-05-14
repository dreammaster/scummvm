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

#include "krondor/views/dialogs/preferences.h"
#include "krondor/core/vars.h"

namespace Krondor {
namespace Views {
namespace Dialogs {

enum {
	PREF_CANCEL = 46,
	PREF_DEFAULTS = 32,
	PREF_OK = 24,

	PREF_STEP_SMALL = 192,
	PREF_STEP_MEDIUM = 193,
	PREF_STEP_LARGE = 194,

	PREF_TURN_SMALL = 195,
	PREF_TURN_MEDIUM = 196,
	PREF_TURN_LARGE = 197,

	PREF_DETAIL_MIN = 198,
	PREF_DETAIL_LOW = 199,
	PREF_DETAIL_HIGH = 200,
	PREF_DETAIL_MAX = 201,

	PREF_TEXT_WAIT = 202,
	PREF_TEXT_MEDIUM = 203,
	PREF_TEXT_FAST = 204,

	PREF_SOUND = 205,
	PREF_MUSIC = 206,
	PREF_COMBAT_MUSIC = 207,
	PREF_INTRODUCTION = 208,
	PREF_CD_MUSIC = 209
};

Preferences::Preferences() : Dialog("Preferences",
		"req_pref.dat", "options.pal", "options2.scx",
		"game.fnt", "lbl_pref.dat") {
}

bool Preferences::msgFocus(const FocusMessage &msg) {
	Dialog::msgFocus(msg);

	loadSelections();
	return true;
}

bool Preferences::msgAction(const ActionMessage &msg) {
	switch (msg._action) {
	case PREF_OK:
		// Copy over prefs and close dialog
		g_vars->_prefs = g_vars->_tempPrefs;
		g_vars->_prefs.apply();
		replaceView("Options");
		break;

	case PREF_CANCEL:
		// Close dialog
		replaceView("Options");
		break;

	case PREF_DEFAULTS:
		// Reset settings
		g_vars->_tempPrefs.setDefaults();
		loadSelections();
		break;

	default:
		_stepSize.action(msg._action);
		_turnSize.action(msg._action);
		_textSpeed.action(msg._action);
		_detail.action(msg._action);
		_sound.action(msg._action);
		_music.action(msg._action);
		_combatMusic.action(msg._action);
		_cdMusic.action(msg._action);
		_introduction.action(msg._action);

		redraw();
		break;
	}

	return true;
}

void Preferences::loadSelections() {
	g_vars->_tempPrefs = g_vars->_prefs;
	Prefs &prefs = g_vars->_tempPrefs;

	_stepSize.load(this, (int)prefs.getStepSize(),
		[](int newValue) { g_vars->_tempPrefs.setStepSize((StepTurnSize)newValue); },
		PREF_STEP_SMALL, STS_SMALL,
		PREF_STEP_MEDIUM, STS_MEDIUM,
		PREF_STEP_LARGE, STS_LARGE
	);
	_turnSize.load(this, (int)prefs.getTurnSize(),
		[](int newValue) { g_vars->_tempPrefs.setTurnSize((StepTurnSize)newValue); },
		PREF_TURN_SMALL, STS_SMALL,
		PREF_TURN_MEDIUM, STS_MEDIUM,
		PREF_TURN_LARGE, STS_LARGE
	);
	_textSpeed.load(this, (int)prefs.getTextSpeed(),
		[](int newValue) { g_vars->_tempPrefs.setTextSpeed((TextSpeed)newValue); },
		PREF_TEXT_WAIT, TS_WAIT,
		PREF_TEXT_MEDIUM, TS_MEDIUM,
		PREF_TEXT_FAST, TS_FAST
	);
	_detail.load(this, (int)prefs.getDetail(),
		[](int newValue) { g_vars->_tempPrefs.setDetail((LevelOfDetail)newValue); },
		PREF_DETAIL_MIN, LOD_MIN,
		PREF_DETAIL_LOW, LOD_LOW,
		PREF_DETAIL_HIGH, LOD_HIGH,
		PREF_DETAIL_MAX, LOD_MAX
	);

	_sound.load(this, PREF_SOUND, prefs.getSound(),
		[](bool newValue) { g_vars->_tempPrefs.setSound(newValue); });
	_music.load(this, PREF_MUSIC, prefs.getMusic(),
		[](bool newValue) { g_vars->_tempPrefs.setMusic(newValue); });
	_combatMusic.load(this, PREF_COMBAT_MUSIC, prefs.getCombatMusic(),
		[](bool newValue) { g_vars->_tempPrefs.setCombatMusic(newValue); });
	_cdMusic.load(this, PREF_CD_MUSIC, prefs.getCdMusic(),
		[](bool newValue) { g_vars->_tempPrefs.setCdMusic(newValue); });
	_introduction.load(this, PREF_INTRODUCTION, prefs.getIntroduction(),
		[](bool newValue) { g_vars->_tempPrefs.setIntroduction(newValue); });

	redraw();
}


void Preferences::RadioButtons::load(Preferences *owner, int value,
	Preferences::RadioCallback callback,
	int action1, int value1, int action2, int value2,
	int action3, int value3, int action4, int value4) {
	int actions[4] = { action1, action2, action3, action4 };
	int values[4] = { value1, value2, value3, value4 };

	_callback = callback;
	_items.clear();

	for (uint i = 0; i < 4; ++i) {
		for (auto *element : owner->_children) {
			auto *radio = dynamic_cast<Gfx::Widgets::Choice *>(element);

			if (actions[i] != -1 && radio && radio->getData()->_action == actions[i]) {
				radio->setSelected(values[i] == value);
				_items.push_back(RadioButton(radio, values[i]));
				break;
			}
		}
	}

	assert(_items.size() >= 3);
}

void Preferences::RadioButtons::action(int action) {
	// Check if the radiobuttons contains the action
	bool hasAction = false;
	for (const auto &rb : _items)
		hasAction |= rb._element->getData()->_action == action;

	if (hasAction) {
		for (const auto &rb : _items) {
			bool isSelected = rb._element->getData()->_action == action;
			rb._element->setSelected(isSelected);

			if (isSelected)
				_callback(rb._value);
		}
	}
}

void Preferences::Checkbox::load(Preferences *owner,
		int action, bool value, CheckboxCallback callback) {
	_callback = callback;

	for (auto *element : owner->_children) {
		auto *checkbox = dynamic_cast<Gfx::Widgets::Choice *>(element);
		if (checkbox && checkbox->getData()->_action == action) {
			_element = checkbox;
			_element->setSelected(value);
			break;
		}
	}

	assert(_element);
}

void Preferences::Checkbox::action(int action) {
	if (_element->getData()->_action == action) {
		_element->setSelected(!_element->getSelected());
		_callback(_element->getSelected());
	}
}

} // namespace Dialogs
} // namespace Views
} // namespace Krondor
