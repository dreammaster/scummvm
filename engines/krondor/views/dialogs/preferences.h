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

#ifndef KRONDOR_VIEWS_DIALOGS_PREFERENCES_H
#define KRONDOR_VIEWS_DIALOGS_PREFERENCES_H

#include "krondor/views/dialogs/dialog.h"
#include "krondor/gfx/widgets/choice.h"

namespace Krondor {
namespace Views {
namespace Dialogs {

class Preferences : public Dialog {
	typedef void (*RadioCallback)(int value);
	typedef void (*CheckboxCallback)(bool value);

	struct RadioButton {
		Gfx::Widgets::Choice *_element = nullptr;
		int _value = 0;
		RadioButton() {}
		RadioButton(Gfx::Widgets::Choice *element, int value) :
			_element(element), _value(value) {}
	};

	class RadioButtons {
	private:
		Common::Array<RadioButton> _items;
		RadioCallback _callback = nullptr;
	public:
		void load(Preferences *owner, int value, RadioCallback callback,
			int action1, int value1,
			int action2, int value2,
			int action3, int value3,
			int action4 = -1, int value4 = -1);

		void action(int action);
	};
	class Checkbox {
	private:
		Gfx::Widgets::Choice *_element = nullptr;
		CheckboxCallback _callback = nullptr;
	public:
		void load(Preferences *owner, int action, bool value,
			CheckboxCallback callback);

		void action(int action);
	};

private:
	RadioButtons _stepSize;
	RadioButtons _turnSize;
	RadioButtons _textSpeed;
	RadioButtons _detail;
	Checkbox _sound;
	Checkbox _music;
	Checkbox _combatMusic;
	Checkbox _cdMusic;
	Checkbox _introduction;

private:
	void loadSelections();

public:
	Preferences();
	~Preferences() override {}

	bool msgFocus(const FocusMessage &msg) override;
	bool msgAction(const ActionMessage &msg) override;
};

} // namespace Dialogs
} // namespace Views
} // namespace Krondor

#endif
