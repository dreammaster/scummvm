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

#ifndef WASTELAND_FOD_VIEWS_EDIT_MEMBER_H
#define WASTELAND_FOD_VIEWS_EDIT_MEMBER_H

#include "wasteland/fod/views/main_menu/menu_view.h"
#include "wasteland/gfx/clickable_text.h"
#include "wasteland/gfx/text_entry.h"

namespace Wasteland {
namespace FOD {
namespace Views {

class EditMember : public MenuView {
	enum Mode { SELECT_PROFESSION, EDIT_STATS};
private:
	Shared::Gfx::ClickableText _profession11;
	Shared::Gfx::ClickableText _profession12;
	Shared::Gfx::ClickableText _profession13;
	Shared::Gfx::ClickableText _profession14;
	Shared::Gfx::ClickableText _profession15;
	Shared::Gfx::ClickableText *_professions1[5];
	Shared::Gfx::ClickableText _profession21;
	Shared::Gfx::ClickableText _profession22;
	Shared::Gfx::ClickableText _profession23;
	Shared::Gfx::ClickableText _profession24;
	Shared::Gfx::ClickableText _profession25;
	Shared::Gfx::ClickableText *_professions2[5];
	Shared::Gfx::ClickableText _attr0;
	Shared::Gfx::ClickableText _attr1;
	Shared::Gfx::ClickableText _attr2;
	Shared::Gfx::ClickableText _attr3;
	Shared::Gfx::ClickableText _attr4;
	Shared::Gfx::ClickableText _attr5;
	Shared::Gfx::ClickableText _attr6;
	Shared::Gfx::ClickableText _attr7;
	Shared::Gfx::ClickableText *_attributes[8];
	Shared::Gfx::TextEntry _nameEntry;

	Mode _mode = SELECT_PROFESSION;
	Window _mainArea;
	int _selectedAttribute = 0;

	void setMode(Mode mode);
	void addMember();
	void createNewMember(int profession);
	void editMember(int rosterNum);
	void setupMemberCon();
	void writeSkills();
	void writeAttributes();
	void changeAttribute(int attrNum, int delta);

public:
	EditMember();
	virtual ~EditMember() {}

	bool msgGame(const GameMessage &msg) override;
	bool msgKeypress(const KeypressMessage &msg) override;
	void draw() override;
};

} // namespace Views
} // namespace FOD
} // namespace Wasteland

#endif
