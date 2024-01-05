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

#include "wasteland/fod/views/edit_member.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

EditMember::EditMember() : MenuView("EditMember"),
	_mainArea(0, 14, 39, 19),
	_profession11("Profession11", nullptr, 4, 17, "1)Survivalist", Common::KEYCODE_1),
	_profession12("Profession12", nullptr, 4, 18, "2)Vigilante", Common::KEYCODE_2),
	_profession13("Profession13", nullptr, 4, 19, "3)Medic", Common::KEYCODE_3),
	_profession14("Profession14", nullptr, 23, 17, "4)Hood", Common::KEYCODE_4),
	_profession15("Profession15", nullptr, 23, 18, "5)Mechanic", Common::KEYCODE_5),
	_profession21("Profession21", nullptr, 0, 15, "1)Survivalist", Common::KEYCODE_1),
	_profession22("Profession22", nullptr, 0, 16, "2)Vigilante", Common::KEYCODE_2),
	_profession23("Profession23", nullptr, 0, 17, "3)Medic", Common::KEYCODE_3),
	_profession24("Profession24", nullptr, 0, 18, "4)Hood", Common::KEYCODE_4),
	_profession25("Profession25", nullptr, 0, 19, "5)Mechanic", Common::KEYCODE_5),
	_f1("F1", nullptr, 0, 22, "F1>", Common::KEYCODE_F1),
	_f2("F2", nullptr, 0, 23, "F2>", Common::KEYCODE_F2),
	_nameEntry("NameEntry", nullptr, 4, 22, 12) {

	_professions1[0] = &_profession11;
	_professions1[1] = &_profession12;
	_professions1[2] = &_profession13;
	_professions1[3] = &_profession14;
	_professions1[4] = &_profession15;
	_professions2[0] = &_profession21;
	_professions2[1] = &_profession22;
	_professions2[2] = &_profession23;
	_professions2[3] = &_profession24;
	_professions2[4] = &_profession25;

	_f1.setInverseColor(true);
	_f2.setInverseColor(true);
}

bool EditMember::msgGame(const GameMessage& msg) {
	if (msg._name == "ADD_MEMBER") {
		addMember();
		return true;

	} else if (msg._name == "EDIT_MEMBER") {
		editMember(msg._value);
		return true;
	}

	return false;
}

void EditMember::addMember() {
	_rosterNum = g_engine->_disk1._partyCount;
	_mode = SELECT_PROFESSION;
	addView("EditMember");
}

void EditMember::editMember(int rosterNum) {
	_rosterNum = rosterNum;
	addView("EditMember");
}

void EditMember::draw() {
	MenuView::draw();

	switch (_mode) {
	case SELECT_PROFESSION: {
		writePortraitText("Welcome");

		Surface main = getSurface(_mainArea);
		main.writeCenteredString("Choose a Profession:", 1);

		for (int i = 0; i < 5; ++i)
			_professions1[i]->draw();
		break;
	}

	case EDIT_STATS:
		writePortraitText(g_engine->_archetypes._professions[_profession]._name);
		writeSkills();

		for (int i = 0; i < 5; ++i)
			_professions2[i]->draw();

		_nameEntry.draw();
		break;

	default:
		break;
	}
}

bool EditMember::msgKeypress(const KeypressMessage &msg) {
	switch (_mode) {
	case SELECT_PROFESSION:
		if (msg.keycode >= Common::KEYCODE_1 && msg.keycode <= Common::KEYCODE_5) {
			_profession = msg.keycode - Common::KEYCODE_1;
			g_engine->_disk1._partyCount++;

			createNewMember();
			Data::PartyMember &member = g_engine->_disk1._roster[_rosterNum];
			member._name = "Ojnab Bob";

			_mode = EDIT_STATS;
			redraw();
		}
		break;

	case EDIT_STATS:
		switch (msg.keycode) {
		case Common::KEYCODE_F1:
		case Common::KEYCODE_F2:
			if (msg.keycode == Common::KEYCODE_F1 || g_engine->_disk1._partyCount == 2) {
				_rosterNum = msg.keycode - Common::KEYCODE_F1;
				redraw();
			}
			return true;

		default:
			return _nameEntry.msgKeypress(msg);
		}
		break;

	default:
		break;
	}

	return true;
}

bool EditMember::msgMouseDown(const MouseDownMessage& msg) {
	switch (_mode) {
	case SELECT_PROFESSION:
		for (int i = 0; i < 5; ++i) {
			if (_professions1[i]->msgMouseDown(msg))
				return true;
		}
		break;

	case EDIT_STATS:
		for (int i = 0; i < 5; ++i) {
			if (_professions2[i]->msgMouseDown(msg))
				return true;
		}
		break;

	default:
		break;
	}

	return false;
}

void EditMember::createNewMember() {
	Data::PartyMember &member = g_engine->_disk1._roster[_rosterNum];
	Data::Profession &prof = g_engine->_archetypes._professions[_profession];

	// TODO: Origiinal uses loop to clear 12 bytes. Is attributes that large,
	// or is it just using a hack to clear the following fields
	member._unknown1 = member._field23 = 0;

	// Copy over the base attributes and skills for the profession
	Common::copy(prof._attributes, prof._attributes + ATTRIBUTES_COUNT, member._attributes);
	member._field23 = prof._field33;
	Common::copy(prof._activeSkills, prof._activeSkills + SKILLS_COUNT, member._activeSkills);
	Common::copy(prof._passiveSkills, prof._passiveSkills + SKILLS_COUNT, member._passiveSkills);
	Common::fill(member._activeSkills2, member._activeSkills2 + SKILLS_COUNT, 0);
	Common::fill(member._passiveSkills2, member._passiveSkills2 + SKILLS_COUNT, 0);

	member._field58 = 0;
	member._profession = _profession;
	member._field51 = prof._field7B;
	member._sex = Data::SEX_MALE;
}

void EditMember::setupMemberCon() {
	Data::PartyMember &member = g_engine->_disk1._roster[_rosterNum];
	Data::Profession &prof = g_engine->_archetypes._professions[_profession];

	member._con = member._conTemp = member._conBase =
		g_engine->getRandomNumber(prof._unkMin, prof._unkMax);
}

void EditMember::writeSkills() {
	Data::PartyMember &member = g_engine->_disk1._roster[_rosterNum];
	Data::Profession &prof = g_engine->_archetypes._professions[member._profession];

	Surface skillsArea = getSurface(Gfx::Window(14, 1, 38, 12));
	skillsArea.writeCenteredString("Skills", 0);
	skillsArea.writeCenteredString("Active      Passive", 1);


}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
