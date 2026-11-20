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

#include "ultima/ultima5/views/interactions/ztats.h"
#include "ultima/ultima5/data/inventory_data.h"
#include "ultima/ultima5/data/shop_data.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int WINDOW_LEFT = 24;
constexpr int WINDOW_TOP = 1;
constexpr int WINDOW_ROWS = 9;
constexpr int WINDOW_COLUMNS = 16;
constexpr int TEXT_WIDTH = 15;
constexpr int INDICATOR_ROW = 10;

constexpr int KEY_LEFT = 1;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;
constexpr int KEY_ESCAPE = 27;
constexpr int KEY_SPACE = ' ';

static const char CLASS_LIST[] = "AMBFDTPRS";
static const char STATUS_LIST[] = "GPDSC";

static int indexIn(const char *list, int ch) {
	int i = 0;
	while (list[i] != 0 && list[i] != ch)
		++i;
	return i;
}

// Puts text into the window over the party, from a place in it
static void put(View &view, int x, int y, const Common::String &text, byte fg = COLOR_WHITE, byte bg = COLOR_BLACK) {
	view.print(WINDOW_LEFT + x, WINDOW_TOP + y, text, fg, bg);
}

static void clearWindow(View &view) {
	auto s = view.getSurface();
	s.fillRect(Common::Rect(WINDOW_LEFT * 8, WINDOW_TOP * 8, (WINDOW_LEFT + WINDOW_COLUMNS) * 8,
		(WINDOW_TOP + WINDOW_ROWS) * 8), COLOR_BLACK);
}

static int centered(const Common::String &text) {
	return (TEXT_WIDTH - (int)text.size()) / 2;
}

Ztats::Ztats(MessageLog &log) : Interaction(log) {
	memset(_items, 0, sizeof(_items));
	_chooser.allowAll();
	_log.print("Player: ");
}

void Ztats::enterPage() {
	const Data::Savegame &sg = _G(savegame);

	switch (_page) {
	case PAGE_REAGENTS:
		_list.start(InventoryLogic::LIST_REAGENTS, sg._reagents, 8);
		break;
	case PAGE_SPELLS:
		_list.start(InventoryLogic::LIST_SPELLS, sg._mixtures, InventoryLogic::ARMAMENTS);
		break;
	case PAGE_ITEMS:
		_list.start(InventoryLogic::LIST_ITEMS, _items, InventoryLogic::SPECIAL_ITEMS);
		break;
	case PAGE_ARMAMENTS:
		_list.start(InventoryLogic::LIST_ARMAMENTS, sg._equipment, InventoryLogic::ARMAMENTS);
		break;
	default:
		return;
	}

	_empty = _list.empty();
}

bool Ztats::turnPage(int key) {
	const int partySize = _G(savegame)._partySize;

	if (key == KEY_SPACE || key == KEY_ESCAPE) {
		_log.print("Done\n");
		return true;
	}

	switch (key) {
	case KEY_UP:
	case KEY_LEFT:
		if (_page == PAGE_EQUIPMENT)
			_page = partySize * 2 - 1;
		else if (_page > 0)
			--_page;
		else
			_page = PAGE_ARMAMENTS;
		break;

	case KEY_RIGHT:
	case KEY_DOWN:
		if (partySize * 2 - 1 == _page)
			_page = PAGE_EQUIPMENT;
		else if (_page < PAGE_ARMAMENTS)
			++_page;
		else
			_page = 0;
		break;

	default:
		if (key >= '1' && key <= '6' && key - '1' < partySize)
			_page = (key - '1') * 2;
		break;
	}

	enterPage();
	return false;
}

bool Ztats::keypress(const KeypressMessage &msg) {
	Data::Savegame &sg = _G(savegame);

	if (_choosing) {
		const PartyMemberChooser::Result result = _chooser.handleKey(msg);
		if (result == PartyMemberChooser::PENDING)
			return false;

		if (result == PartyMemberChooser::CANCELLED) {
			_log.print("None!\n");
			return true;
		}

		if (result == PartyMemberChooser::ALL) {
			_log.printChar('\n');
			_page = PAGE_EQUIPMENT;
		} else {
			_log.print(sg._party[_chooser.index()]._name);

			if (_log.column() != 0)
				_log.printChar('\n');

			_page = _chooser.index() * 2;
		}

		_log.print("\nStatus: ");
		InventoryLogic::specialItems(sg, _items);
		_choosing = false;
		enterPage();
		return false;
	}

	const int key = originalKey(msg);

	if (_page >= PAGE_REAGENTS) {
		if (_empty)
			return turnPage(key);

		if (!_list.scroll(key) && (key == KEY_LEFT || key == KEY_RIGHT || key == KEY_ESCAPE || key == KEY_SPACE ||
				(key >= '0' && key <= '6')))
			return turnPage(key);

		_list.redraw();
		return false;
	}

	return turnPage(key);
}

void Ztats::drawMemberPage(View &view, bool arms) {
	const Data::Character &who = _G(savegame)._party[_page >> 1];

	clearWindow(view);
	view.drawLabel(who._name);

	if (arms) {
		const Common::String title = "Arms";
		put(view, centered(title), 0, title);
		view.getSurface().drawLine((WINDOW_LEFT + centered(title)) * 8, (WINDOW_TOP) * 8 + 7,
			(WINDOW_LEFT + centered(title) + (int)title.size()) * 8 - 1, WINDOW_TOP * 8 + 7, COLOR_WHITE);

		int shown = 0;
		for (int i = 0; i < 6; ++i) {
			if (who._equipment[i] != InventoryLogic::NOTHING) {
				put(view, 0, 2 + shown, Common::String(" ") + Data::ITEM_SHORT_NAMES[who._equipment[i]]);
				++shown;
			}
		}

		if (shown == 0) {
			const Common::String none = "(None ready)";
			put(view, centered(none), 4, none);
		}

		return;
	}

	const int classIndex = indexIn(CLASS_LIST, who._class);
	const int statusIndex = indexIn(STATUS_LIST, who._status);

	Common::String header(Data::CLASS_NAME_PADDING[classIndex], ' ');
	header += (char)who._gender;
	header += Common::String::format(" Lv-%d %s", who._level, Data::CLASS_NAMES[classIndex]);
	put(view, 0, 0, header);

	const Common::String status = Data::STATUS_NAMES[statusIndex];
	put(view, centered(status), 1, status);

	put(view, 0, 3, Common::String::format("Str=%02d  HP:%4d", who._str, who._hp));
	put(view, 0, 4, Common::String::format("Int=%02d  HM:%4d", who._int, who._maxHp));
	put(view, 0, 5, Common::String::format("Dex=%02d  Ex:%4d", who._dex, who._exp));
	put(view, 0, 7, Common::String::format("    Magic:%2d", who._mag));
}

void Ztats::drawEquipmentPage(View &view) {
	const Data::Savegame &sg = _G(savegame);

	clearWindow(view);
	view.drawLabel("Equipment");

	put(view, 0, 1, Common::String::format(" Food: %4d", sg._food));
	put(view, 0, 2, Common::String::format(" Gold: %4d", sg._gold));
	put(view, 0, 4, Common::String::format(" Keys.......%2d", sg._keys));
	put(view, 0, 5, Common::String::format(" Gems.......%2d", sg._gems));
	put(view, 0, 6, Common::String::format(" Torches....%2d", sg._torches));

	if (sg._grapple != 0)
		put(view, 0, 7, " Grapple");
}

void Ztats::drawListPage(View &view) {
	static const char *const TITLES[] = { "Reagents", "Spells", "Items", "Armaments" };

	view.drawLabel(TITLES[_page - PAGE_REAGENTS]);
	view.drawTextGrid(_list.grid(), WINDOW_TOP);

	if (!_empty)
		view.drawScrollArrows(INDICATOR_ROW, _list.arrows());
}

void Ztats::draw(View &view) {
	if (_choosing) {
		_chooser.draw(view);
		return;
	}

	if (_page == PAGE_EQUIPMENT)
		drawEquipmentPage(view);
	else if (_page > PAGE_EQUIPMENT)
		drawListPage(view);
	else
		drawMemberPage(view, (_page & 1) != 0);
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
