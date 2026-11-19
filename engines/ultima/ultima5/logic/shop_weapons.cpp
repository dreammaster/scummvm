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

#include "common/util.h"
#include "ultima/ultima5/logic/shop_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int EQUIPMENT_COUNT = 0x30;
constexpr int NO_LIST_END = 0xFF;
constexpr int SHOP_LIST_MAX = 8;
constexpr int SELL_ROWS = 4;
constexpr int NAME_FITS = 0xD;
constexpr int SHORT_NAME_WIDTH = 10;
constexpr byte ITEM_ARROWS = 0x1B;
constexpr byte ITEM_QUARRELS = 0x1D;
constexpr int KEY_ESCAPE_CODE = 27;
constexpr int KEY_SPACE_CODE = ' ';
constexpr int KEY_ENTER_CODE = 13;
constexpr int KEY_LEFT_CODE = 1;
constexpr int KEY_RIGHT_CODE = 2;
constexpr int KEY_UP_CODE = 3;
constexpr int KEY_DOWN_CODE = 4;
constexpr int KEY_HOME_CODE = 0xD3;
constexpr int KEY_END_CODE = 0xD4;
constexpr int KEY_PGUP_CODE = 0xD5;
constexpr int KEY_PGDN_CODE = 0xD6;
constexpr int PAGE = 4;

void ShopLogic::weaponsWelcome() {
	say("\"Good @, and welcome to #!\"\n");
	_step = STEP_WEAPONS_WELCOME;
}

void ShopLogic::weaponsChoose(int key) {
	switch (key) {
	case 'B':
		_log.print("Buy\n\n\"");
		_log.print(Data::WEAPON_GOOD[_G(gameRandom).range(0, 3)]);
		_log.print(Data::WEAPON_STOCK[_G(gameRandom).range(0, 3)]);
		_code = 0;
		weaponsMenu();
		break;

	case 'S':
		_log.print("Sell\n\n\"");
		weaponsSell();
		break;

	case KEY_SPACE_CODE:
		_log.print("No");
		finish(_code);
		break;

	default:
		break;
	}
}

void ShopLogic::weaponsMenu() {
	_log.print("\n\n");

	int i;
	for (i = 0; i < SHOP_LIST_MAX; ++i) {
		const int item = Data::WEAPON_LISTS[_index][i];
		if (item == NO_LIST_END)
			break;

		_log.printChar('a' + i);
		_log.print("...");

		const char *name = Data::ITEM_NAMES[item];
		_log.print(strlen(name) < NAME_FITS ? name : Data::ITEM_SHORT_NAMES[item]);

		if (_log.column() != 0)
			_log.printChar('\n');
	}

	_count = i;
	_log.printChar('\n');
	_log.print(Data::WEAPON_WHICH[_G(gameRandom).range(0, 3)]);
	_log.print("\" ");
	_step = STEP_WEAPONS_MENU;
}

void ShopLogic::weaponsItem(int choice) {
	if (choice >= _count)
		return;

	_log.printChar('a' + choice);

	_item = Data::WEAPON_LISTS[_index][choice];
	_price = discounted(Data::WEAPON_PRICES[_item]);

	_log.print("\n\n\"");
	sayAt(Data::WEAPON_TEXT[_item]);
	_log.print("\n\n");
	_log.print(Data::WEAPON_BUY_PROMPTS[_G(gameRandom).range(0, 3)]);
	_log.print("\" ");
	_step = STEP_WEAPONS_CONFIRM;
}

void ShopLogic::weaponsAnswer(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No\n\n");
		weaponsAfterBuy();
		return;
	}

	_log.print("Yes\n");

	if (sg._equipment[_item] == 99) {
		_log.print("\n\"Thou canst not carry any more!\"\n");
		say("says $.\n\n");
		waitKey(STEP_WEAPONS_SOLD);
		return;
	}

	if (sg._gold < _price) {
		_log.print("\n\"");
		_log.print(Data::WEAPON_CANT_PAY[_G(gameRandom).range(0, 3)]);
		say("\"\nyells $.\n");
		_code = -1;
	} else {
		sg._gold -= _price;
		stealGold();

		if (_item == ITEM_ARROWS || _item == ITEM_QUARRELS)
			sg._equipment[_item] = 99;
		else
			sg._equipment[_item] = MIN(sg._equipment[_item] + 1, 99);

		say("\nSold!\n");
		_code = 1;
	}

	weaponsAfterBuy();
}

void ShopLogic::weaponsAfterBuy() {
	if (_code != -1) {
		_log.print("\"Anything else,\n");

		if (_code != 0)
			_log.print(isFemale() ? "milady?" : "sir?");
		else
			_log.print("then?");

		weaponsMenu();
	} else {
		finish(-1);
	}
}

// ---- Selling

bool ShopLogic::anyEquipment() const {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < EQUIPMENT_COUNT; ++i) {
		if (sg._equipment[i] != 0)
			return true;
	}

	return false;
}

int ShopLogic::prevOwned(int from) const {
	const Data::Savegame &sg = _G(savegame);

	while (--from >= 0) {
		if (sg._equipment[from] != 0)
			return from;
	}

	return -1;
}

int ShopLogic::nextOwned(int from) const {
	const Data::Savegame &sg = _G(savegame);

	while (++from < EQUIPMENT_COUNT) {
		if (sg._equipment[from] != 0)
			return from;
	}

	return -1;
}

void ShopLogic::settleSellList() {
	const Data::Savegame &sg = _G(savegame);

	// What's picked may have been sold
	if (sg._equipment[_sellSelected] == 0) {
		if (_sellTop == _sellSelected) {
			_sellSelected = prevOwned(_sellSelected);
			if (_sellSelected == -1)
				_sellSelected = nextOwned(-1);

			_sellTop = _sellSelected;
		} else {
			_sellSelected = nextOwned(_sellSelected);

			if (_sellSelected == -1) {
				_sellSelected = prevOwned(EQUIPMENT_COUNT);

				if (prevOwned(_sellTop) > -1)
					_sellTop = prevOwned(_sellTop);
			}
		}
	}

	// Which row of the window it's in
	_sellRow = 0;
	int row = 1;
	for (int item = _sellTop; item != -1; item = nextOwned(item)) {
		if (item == _sellSelected)
			_sellRow = row;

		if (++row == SELL_ROWS + 1)
			break;
	}
}

bool ShopLogic::sellWindow(SellWindow &window) const {
	if (_step != STEP_WEAPONS_SELL && _step != STEP_WEAPONS_SELL_CONFIRM)
		return false;

	const Data::Savegame &sg = _G(savegame);
	int item = _sellTop;
	int row = 0;

	while (item != -1) {
		const char *name = Data::ITEM_SHORT_NAMES[item];
		window.rows[row] = Common::String::format("%2d-%-*s", sg._equipment[item], SHORT_NAME_WIDTH, name);

		if (item == _sellSelected)
			window.selected = row;

		if (++row == SELL_ROWS)
			break;

		item = nextOwned(item);
	}

	for (int i = row; i < SELL_ROWS; ++i)
		window.rows[i].clear();

	// Arrows show which ways there is more to see
	window.arrows = 0;
	if (prevOwned(_sellTop) != -1)
		window.arrows = 2;
	if (item != -1 && nextOwned(item) != -1)
		++window.arrows;

	return true;
}

void ShopLogic::weaponsSell() {
	if (!anyEquipment()) {
		say("Thou hast nothing to sell!\"\ngrowls $.\n");
		finish(-1);
		return;
	}

	_log.print(Data::WEAPON_SELL_PROMPTS[_G(gameRandom).range(0, 3)]);
	_log.print("\" ");

	_sellTop = _sellSelected = nextOwned(-1);
	settleSellList();
	_step = STEP_WEAPONS_SELL;
}

void ShopLogic::weaponsSellDone(bool refused) {
	if (!refused) {
		_log.print("\n\n\"");
		_log.print(Data::WEAPON_SELL_BYES[_G(gameRandom).range(0, 3)]);
		_log.print("\"\n");

		if (anyEquipment())
			say("says $.\n");
	}

	finish(-1);
}

void ShopLogic::weaponsSellKey(int key) {
	Data::Savegame &sg = _G(savegame);

	switch (key) {
	case KEY_LEFT_CODE:
	case KEY_UP_CODE:
	case KEY_PGUP_CODE:
		for (int n = key == KEY_PGUP_CODE ? PAGE : 1; n > 0; --n) {
			const int before = prevOwned(_sellSelected);

			if (_sellSelected == _sellTop) {
				if (before > -1)
					_sellTop = _sellSelected = before;
			} else {
				_sellSelected = before;
			}
		}

		settleSellList();
		break;

	case KEY_RIGHT_CODE:
	case KEY_DOWN_CODE:
	case KEY_PGDN_CODE:
		for (int n = key == KEY_PGDN_CODE ? PAGE : 1; n > 0; --n) {
			const int after = nextOwned(_sellSelected);

			if (after > -1) {
				_sellSelected = after;

				if (++_sellRow > SELL_ROWS)
					_sellTop = nextOwned(_sellTop);
			}
		}

		settleSellList();
		break;

	case KEY_HOME_CODE:
		_sellSelected = _sellTop = nextOwned(-1);
		settleSellList();
		break;

	case KEY_END_CODE:
		_sellSelected = _sellTop = prevOwned(EQUIPMENT_COUNT);

		for (int i = 1; i < SELL_ROWS; ++i) {
			const int above = prevOwned(_sellTop);
			if (above == -1)
				break;

			_sellTop = above;
		}

		settleSellList();
		break;

	case KEY_ENTER_CODE:
	case KEY_SPACE_CODE:
		_item = _sellSelected;

		if (_item == ITEM_ARROWS || _item == ITEM_QUARRELS) {
			say("\n\n\"We don't deal in used ammunition!\"\ngrowls $.\n");
			weaponsSellDone(true);
			return;
		}

		_log.print("\n\n\"");

		if (Data::WEAPON_PRICES[_item] != 0) {
			const uint32 intel = sg._party[_member]._int;
			_price = (int16)((intel * Data::WEAPON_PRICES[_item] * 3U) / 100U + 1);

			const char *name = Data::WEAPON_SELL_NAMES[_item];
			_item1 = *name ? name : Data::ITEM_NAMES[_item];

			sayAt(Data::WEAPON_SELL_TEXT[_G(gameRandom).range(0, 7)]);
			_log.print("\n\nDeal?\" ");
			_step = STEP_WEAPONS_SELL_CONFIRM;
		} else {
			say("That, I cannot buy from thee.\"\nsays $.");
			weaponsSellContinue();
		}
		break;

	case KEY_ESCAPE_CODE:
		weaponsSellDone(false);
		break;

	default:
		break;
	}
}

void ShopLogic::weaponsSellAnswer(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No");
	} else {
		say("Yes\n\n\"Done!\"\nsays $.");
		sg._gold = MIN(sg._gold + _price, 9999);

		if (sg._equipment[_item] > 1)
			--sg._equipment[_item];
		else
			sg._equipment[_item] = 0;
	}

	weaponsSellContinue();
}

void ShopLogic::weaponsSellContinue() {
	if (anyEquipment()) {
		settleSellList();
		_log.print("\n\n\"");
		_log.print(Data::WEAPON_SELL_MORE[_G(gameRandom).range(0, 3)]);
		_log.print("\" ");
		_step = STEP_WEAPONS_SELL;
	} else {
		weaponsSellDone(false);
	}
}

} // namespace Ultima5
} // namespace Ultima
