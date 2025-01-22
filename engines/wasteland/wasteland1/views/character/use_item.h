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

#ifndef WASTELAND_WASTELAND1_VIEWS_CHARACTER_USE_ITEM_H
#define WASTELAND_WASTELAND1_VIEWS_CHARACTER_USE_ITEM_H

#include "wasteland/wasteland1/views/gfx/pane.h"
#include "wasteland/wasteland1/views/gfx/text_button.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

class UseItem : public Gfx::Pane {
	enum Mode {
		INITIAL, RELOAD, NO_TRADE, TRADE
	};
private:
	Gfx::TextButton _esc;
	Gfx::TextButton _enter;
	int _selectedItem = 0;
	Mode _mode = INITIAL;

	void drop();
	void trade();
	void equip();

public:
	UseItem();
	virtual ~UseItem() {}

	void draw() override;
	bool msgGame(const GameMessage &msg) override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
