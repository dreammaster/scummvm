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

#include "wasteland/fod/views/main_menu/edit_member.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

struct AttributePrefix {
	int _x;
	int _y;
	const char *_text;
};

static const AttributePrefix ATTRIBUTE_PREFIXES[8] = {
	{21, 2, "ST:"},
	{21, 3, "IQ:"},
	{21, 4, "DX:"},
	{21, 5, "WP:"},
	{30, 2, "AP:"},
	{30, 3, "CH:"},
	{30, 4, "LK:"},
	{29, 5, "Sex:"}
};

// Yes, the original did in fact have an 'Other' sex option, though character
// creation didn't allow you to see it
static const char *SEX_NAMES[3] = {"Other", "Male", "Female"};

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
	_attr0("Attr0", nullptr, ATTRIBUTE_PREFIXES[0]._x + 3, 14 + ATTRIBUTE_PREFIXES[0]._y, "", "ATTR_CLICK", 0),
	_attr1("Attr1", nullptr, ATTRIBUTE_PREFIXES[1]._x + 3, 14 + ATTRIBUTE_PREFIXES[1]._y, "", "ATTR_CLICK", 1),
	_attr2("Attr2", nullptr, ATTRIBUTE_PREFIXES[2]._x + 3, 14 + ATTRIBUTE_PREFIXES[2]._y, "", "ATTR_CLICK", 2),
	_attr3("Attr3", nullptr, ATTRIBUTE_PREFIXES[3]._x + 3, 14 + ATTRIBUTE_PREFIXES[3]._y, "", "ATTR_CLICK", 3),
	_attr4("Attr4", nullptr, ATTRIBUTE_PREFIXES[4]._x + 3, 14 + ATTRIBUTE_PREFIXES[4]._y, "", "ATTR_CLICK", 4),
	_attr5("Attr5", nullptr, ATTRIBUTE_PREFIXES[5]._x + 3, 14 + ATTRIBUTE_PREFIXES[5]._y, "", "ATTR_CLICK", 5),
	_attr6("Attr6", nullptr, ATTRIBUTE_PREFIXES[6]._x + 3, 14 + ATTRIBUTE_PREFIXES[6]._y, "", "ATTR_CLICK", 6),
	_attr7("Attr7", nullptr, ATTRIBUTE_PREFIXES[7]._x + 5, 14 + ATTRIBUTE_PREFIXES[7]._y, "", "ATTR_CLICK", 7),
	_nameEntry("NameEntry", nullptr, Gfx::Position(4, 21, 0, 4), 12) {

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

	_attributes[0] = &_attr0;
	_attributes[1] = &_attr1;
	_attributes[2] = &_attr2;
	_attributes[3] = &_attr3;
	_attributes[4] = &_attr4;
	_attributes[5] = &_attr5;
	_attributes[6] = &_attr6;
	_attributes[7] = &_attr7;
}

bool EditMember::msgGame(const GameMessage& msg) {
	if (msg._name == "ADD_MEMBER") {
		addMember();
		return true;

	} else if (msg._name == "EDIT_MEMBER") {
		editMember(msg._value);
		return true;

	} else if (msg._name == "ATTR_CLICK") {
		_selectedAttribute = msg._value;
		changeAttribute(_selectedAttribute, msg._stringValue == "LEFT" ? 1 : -1);
		redraw();
		return true;

	} else if (msg._name == "TEXT_CHANGED") {
		Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];
		member._name = msg._stringValue;
		redraw();
		return true;
	}

	return false;
}

void EditMember::setMode(Mode mode) {
	_mode = mode;

	for (int i = _children.size() - 1; i >= 0; --i)
		_children[i]->setParent(nullptr);
	assert(_children.empty());

	switch (mode) {
	case SELECT_PROFESSION:
		hideParty();

		for (int i = 0; i < 5; ++i)
			_professions1[i]->setParent(this);
		break;

	case EDIT_STATS:
		showParty();
		_nameEntry.setParent(this);

		for (int i = 0; i < 5; ++i)
			_professions2[i]->setParent(this);

		for (int i = 0; i < 8; ++i)
			_attributes[i]->setParent(this);
		break;
	}
}

void EditMember::addMember() {
	_selectedPartyMember = g_engine->_disk1._partyCount;
	setMode(SELECT_PROFESSION);
	addView("EditMember");
}

void EditMember::editMember(int rosterNum) {
	_selectedPartyMember = rosterNum;
	addView("EditMember");
}

void EditMember::draw() {
	MenuView::draw();

	switch (_mode) {
	case SELECT_PROFESSION: {
		writePortraitText("Welcome");

		Surface main = getSurface(_mainArea);
		main.writeCenteredString("Choose a Profession:", 1);
		break;
	}

	case EDIT_STATS: {
		const Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];
		_nameEntry.setText(member._name);
		_nameEntry.setPosition(Gfx::Position(4, 21 + _selectedPartyMember, 0, 4));

		writePortraitText(member.getProfession()._name);
		writeSkills();
		writeAttributes();
		break;
	}

	default:
		break;
	}
}

bool EditMember::msgKeypress(const KeypressMessage &msg) {
	if (msg.keycode == Common::KEYCODE_ESCAPE) {
		close();
		return true;
	}

	switch (_mode) {
	case SELECT_PROFESSION:
		if (msg.keycode >= Common::KEYCODE_1 && msg.keycode <= Common::KEYCODE_5) {
			g_engine->_disk1._partyCount++;

			createNewMember(msg.keycode - Common::KEYCODE_1);
			Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];
			member._name = "Ojnab Bob";

			setMode(EDIT_STATS);
			redraw();
			return true;
		}
		break;

	case EDIT_STATS:
		switch (msg.keycode) {
		case Common::KEYCODE_F1:
		case Common::KEYCODE_F2:
		case Common::KEYCODE_F3: {
			int partyNum = msg.keycode - Common::KEYCODE_F1;
			if (partyNum < g_engine->_disk1._partyCount) {
				_selectedPartyMember = partyNum;
				redraw();
			}
			return true;
		}

		case Common::KEYCODE_1:
		case Common::KEYCODE_2:
		case Common::KEYCODE_3:
		case Common::KEYCODE_4:
		case Common::KEYCODE_5:
			// Switch to another profession
			createNewMember(msg.keycode - Common::KEYCODE_1);
			redraw();
			return true;

		case Common::KEYCODE_UP:
			_selectedAttribute = (_selectedAttribute == 0) ? 7 : _selectedAttribute - 1;
			redraw();
			return true;

		case Common::KEYCODE_DOWN:
			_selectedAttribute = (_selectedAttribute + 1) % 8;
			redraw();
			break;

		case Common::KEYCODE_LEFT:
			changeAttribute(_selectedAttribute, -1);
			redraw();
			break;

		case Common::KEYCODE_RIGHT:
			changeAttribute(_selectedAttribute, 1);
			redraw();
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	return MenuView::msgKeypress(msg);
}

void EditMember::createNewMember(int profession) {
	Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];
	member._profession = profession;
	const Data::Profession &prof = member.getProfession();

	member._unknown1 = 0;

	// Copy over the base attributes and skills for the profession
	Common::copy(prof._attributes, prof._attributes + ATTRIBUTES_COUNT, member._attributes);
	member._attributesRemaining = prof._attributePoints;
	Common::copy(prof._activeSkills, prof._activeSkills + SKILLS_COUNT, member._activeSkills);
	Common::copy(prof._passiveSkills, prof._passiveSkills + SKILLS_COUNT, member._passiveSkills);
	Common::fill(member._activeSkills2, member._activeSkills2 + SKILLS_COUNT, 0);
	Common::fill(member._passiveSkills2, member._passiveSkills2 + SKILLS_COUNT, 0);

	member._highlighted = 0;
	member._field51 = prof._field7B;
	member._sex = Data::SEX_MALE;
}

void EditMember::setupMemberCon() {
	Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];
	const Data::Profession &prof = member.getProfession();

	member._con = member._conTemp = member._conBase =
		g_engine->getRandomNumber(prof._unkMin, prof._unkMax);
}

void EditMember::writeSkills() {
	Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];
	Data::Profession &prof = g_engine->_archetypes._professions[member._profession];

	Surface skillsArea = getSurface(Gfx::Window(14, 1, 38, 12));
	skillsArea.writeCenteredString("Skills", 0);
	skillsArea.writeCenteredString("Active      Passive", 1);

	int skillLevel, activeCount = 0, passiveCount = 0;

	// List active skills
	for (int skillNum = 0; skillNum < SKILLS_COUNT; ++skillNum) {
		skillLevel = prof._activeSkills[skillNum];
		if (skillLevel) {
			skillsArea.writeString(Common::String::format("%1d %-10.10s",
				skillLevel, g_engine->_hds._activeSkillNames[skillNum].c_str()),
				0, activeCount + 3);
			++activeCount;
		}

		skillLevel = prof._passiveSkills[skillNum];
		if (skillLevel) {
			skillsArea.writeString(Common::String::format("%1d %-10.10s",
				skillLevel, g_engine->_hds._passiveSkillNames[skillNum].c_str()),
				13, passiveCount + 3);
			++passiveCount;
		}
	}
}

void EditMember::writeAttributes() {
	Surface s = getSurface(Gfx::Window(0, 14, 39, 19));
	const Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];

	s.writeString(Common::String::format("Attribute Pts: <-%d->",
		member._attributesRemaining).c_str(), 19, 0);

	// Write out the attribute headers. Note that we don't need to write the values,
	// since we represent them as clickable text child components
	for (int i = 0; i < 8; ++i) {
		const AttributePrefix &pr = ATTRIBUTE_PREFIXES[i];
		s.writeString(pr._text, pr._x, pr._y);
	}

	// Update the text of the attributes, in preparation for when they're later drawn
	for (int i = 0; i < 7; ++i) {
		_attributes[i]->setText(Common::String::format("%d", member._attributes[i]));
		_attributes[i]->setInverseColor(i == _selectedAttribute);
	}

	_attr7.setText(SEX_NAMES[member._sex]);
	_attr7.setInverseColor(_selectedAttribute == 7);
}

void EditMember::changeAttribute(int attrNum, int delta) {
	Data::PartyMember &member = g_engine->_disk1._party[_selectedPartyMember];
	const Data::Profession &prof = member.getProfession();

	if (attrNum == 7) {
		// Cycle through available sexes. As an enhancement for ScummVM,
		// I've enabled the selection of the originally suppressed 'Other' sex
		member._sex = (member._sex == Data::SEX_OTHER && delta == -1) ? Data::SEX_FEMALE :
			(member._sex + delta) % 3;

	} else if (delta < 0 && member._attributes[attrNum] > prof._attributes[attrNum]) {
		// Only allow decreasing attributes above the minimum for profession
		delta = -delta;
		member._attributes[attrNum] -= delta;
		member._attributesRemaining += delta;

	} else if (delta > 0 && member._attributesRemaining > 0 && member._attributes[attrNum] < 20) {
		member._attributes[attrNum] += delta;
		member._attributesRemaining -= delta;			
	}

	redraw();
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
