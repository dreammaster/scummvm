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

#ifndef WASTELAND_FOD_VIEWS_MENU_VIEW_H
#define WASTELAND_FOD_VIEWS_MENU_VIEW_H

#include "wasteland/fod/views/core/base_view.h"
#include "wasteland/gfx/clickable_text.h"

namespace Wasteland {
namespace FOD {
namespace Views {

class MenuView : public BaseView {
private:
	Shared::Gfx::ClickableText _f1;
	Shared::Gfx::ClickableText _f2;
	Shared::Gfx::ClickableText _f3;

protected:
	int _selectedPartyMember = -1;

	/**
	 * Writes the text under the portrait image
	 */
	void writePortraitText(const Common::String &str);

	/**
	 * Enables the display of the party at the bottom of the screen
	 */
	void showParty();

	/**
	 * Removes the display of the party at the bottom of the screen
	 */
	void hideParty();

public:
	MenuView(const Common::String &name);
	virtual ~MenuView() {}

	void draw() override;
};

} // namespace Views
} // namespace FOD
} // namespace Wasteland

#endif
