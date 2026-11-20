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

#include "ultima/ultima5/logic/inventory_logic.h"
#include "ultima/ultima5/data/inventory_data.h"
#include "ultima/ultima5/data/shop_data.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int ITEM_MAX = 99;
constexpr int SLOT_COUNT = 6;
constexpr int COLUMNS = 13;

// The kinds of things that can be put on, in what's said for them
constexpr byte WORN_HELM = 0x80;
constexpr byte WORN_ARMOUR = 0x40;
constexpr byte HELD_ONE_HAND = 0x20;
constexpr byte HELD_TWO_HANDS = 0x30;
constexpr byte WORN_AMULET = 4;
constexpr byte WORN_RING = 2;

constexpr int ITEM_ARROWS = 0x1B;
constexpr int ITEM_QUARRELS = 0x1D;
constexpr int ITEM_SLING = 0x1A;
constexpr int ITEM_CROSSBOW = 0x24;
constexpr int ITEM_BOW = 0x1C;
constexpr int RING_VANISH_ODDS = 0xF;
constexpr int RING_A = 0x2A;
constexpr int RING_B = 0x2C;

// Where the small pictures are in the rune font
static const char SCROLL_PREFIX[] = "\x1C + ";
static const char POTION_PREFIX[] = "\x1D + ";

bool InventoryLogic::isReady(const Data::Character &who, int item) {
	for (int i = 0; i < SLOT_COUNT; ++i) {
		if (who._equipment[i] == item)
			return true;
	}

	return false;
}

bool InventoryLogic::unready(Data::Savegame &sg, int who, int item) {
	Data::Character &c = sg._party[who];

	for (int i = 0; i < SLOT_COUNT; ++i) {
		if (item == c._equipment[i]) {
			c._equipment[i] = NOTHING;
			return true;
		}
	}

	return false;
}

int InventoryLogic::freeHands(const Data::Character &who) {
	const byte first = who._equipment[SLOT_HAND];
	const byte second = who._equipment[SLOT_OFF_HAND];

	if (first == NOTHING && second == NOTHING)
		return 2;
	if (first == NOTHING)
		return 0;
	if (second == NOTHING && Data::EQUIPMENT_SLOTS[first] != HELD_TWO_HANDS)
		return 1;

	return NOTHING;
}

InventoryLogic::EquipResult InventoryLogic::equip(Data::Savegame &sg, int who, int item) {
	EquipResult result;
	Data::Character &c = sg._party[who];

	// A reason the item can't be put on is said before the list of things comes back
	auto refuse = [&result](const char *why) {
		result._message = Common::String("\n\n") + why + "\n\nItem: ";
		return result;
	};

	if (item == ITEM_ARROWS || item == ITEM_QUARRELS)
		return result;

	if (isReady(c, item)) {
		unready(sg, who, item);
		if (sg._equipment[item] < ITEM_MAX)
			++sg._equipment[item];
		return result;
	}

	if (((item == ITEM_SLING || item == ITEM_CROSSBOW) && sg._equipment[ITEM_ARROWS] == 0) ||
			(item == ITEM_BOW && sg._equipment[ITEM_QUARRELS] == 0))
		return refuse("Thou hast no ammunition for that weapon!");

	// Whether they're strong enough for everything they'd have on
	int carried = 0;
	for (int i = 0; i < SLOT_COUNT; ++i) {
		if (c._equipment[i] != NOTHING)
			carried += Data::EQUIPMENT_WEIGHTS[c._equipment[i]];
	}

	const bool strong = Data::EQUIPMENT_WEIGHTS[item] + carried <= c._str;
	byte *slot = nullptr;
	int hands;

	switch (Data::EQUIPMENT_SLOTS[item]) {
	case WORN_HELM:
		if (c._equipment[SLOT_HELM] != NOTHING)
			return refuse("Remove first thy present helm!");
		slot = &c._equipment[SLOT_HELM];
		break;

	case WORN_ARMOUR:
		if (c._equipment[SLOT_ARMOUR] != NOTHING)
			return refuse("Thou must first remove thine other armour!");
		slot = &c._equipment[SLOT_ARMOUR];
		break;

	case HELD_ONE_HAND:
		hands = freeHands(c);
		if (hands == NOTHING)
			return refuse("Thou must free one of thy hands first!");
		if (hands == 2)
			hands = 0;
		slot = &c._equipment[SLOT_HAND] + hands;
		break;

	case HELD_TWO_HANDS:
		if (freeHands(c) != 2)
			return refuse("Both hands must be free before thou canst wield that!");
		slot = &c._equipment[SLOT_HAND];
		break;

	case WORN_AMULET:
		if (c._equipment[SLOT_AMULET] != NOTHING)
			return refuse("Thou must remove thine other amulet!");
		slot = &c._equipment[SLOT_AMULET];
		break;

	case WORN_RING:
		if (c._equipment[SLOT_RING] != NOTHING)
			return refuse("Only one magic ring may be worn at a time!");
		slot = &c._equipment[SLOT_RING];
		break;

	default:
		return result;
	}

	if (!strong)
		return refuse("Thou art not strong enough!");

	*slot = item;
	--sg._equipment[item];

	// Rings can wear out
	if ((sg._equipment[item] == RING_A || sg._equipment[item] == RING_B) &&
			_G(gameRandom).range(0, RING_VANISH_ODDS) == 0) {
		result._message = "\n\nRing vanishes!\n";
		c._equipment[SLOT_RING] = NOTHING;
		result._ringVanished = true;
	}

	return result;
}

void InventoryLogic::specialItems(const Data::Savegame &sg, byte counts[SPECIAL_ITEMS]) {
	for (int i = 0; i < 8; ++i) {
		counts[i] = sg._scrolls[i];
		counts[8 + i] = sg._potions[i];
		counts[0x15 + i] = sg._moonstoneState[i] == 0xFF ? 0xFF : 0;
	}

	counts[0x10] = sg._carpets;
	counts[0x11] = sg._skullKeys;
	counts[0x12] = sg._amulet;
	counts[0x13] = sg._crown;
	counts[0x14] = sg._sceptre;

	for (int i = 0; i < 3; ++i)
		counts[0x1D + i] = sg._shards[i];

	counts[0x20] = sg._spyGlass;
	counts[0x21] = sg._capePlans != 0 ? 0xFF : 0;
	counts[0x22] = sg._sextant;
	counts[0x23] = sg._pocketWatch;
	counts[0x24] = sg._blackBadge;
	counts[0x25] = sg._sandalwoodBox;
}

int InventoryLogic::prevOwned(int from, const byte *counts, int who) {
	const Data::Savegame &sg = _G(savegame);

	while (--from >= 0) {
		if (counts[from] == 0) {
			if (who == NOTHING)
				continue;
			if (!isReady(sg._party[who], from))
				continue;
		}

		return from;
	}

	return -1;
}

int InventoryLogic::nextOwned(int from, int count, const byte *counts, int who) {
	const Data::Savegame &sg = _G(savegame);

	while (++from < count) {
		if (counts[from] == 0) {
			if (who == NOTHING)
				continue;
			if (!isReady(sg._party[who], from))
				continue;
		}

		return from;
	}

	return -1;
}

void InventoryLogic::printRow(TextGrid &grid, ListKind kind, int index, const byte *counts, byte marker) {
	const int8 amount = (int8)counts[index];

	if (amount != -1) {
		if (amount != 0)
			grid.printNumber((byte)amount, 2, ' ');
		else
			grid.print("--");

		if (marker < 0x20)
			grid.setRune(true);

		grid.printChar(marker);
		grid.setRune(false);
	}

	const char *name = "";
	switch (kind) {
	case LIST_REAGENTS:
		name = Data::REAGENT_LIST_NAMES[index];
		break;
	case LIST_SPELLS:
		name = Data::SPELL_NAMES[index];
		break;
	case LIST_ITEMS:
		name = Data::SPECIAL_ITEM_NAMES[index];
		break;
	case LIST_ARMAMENTS:
		name = Data::ITEM_SHORT_NAMES[index];
		break;
	}

	if (name[0] == '*') {
		// A scroll is a picture, then its words in runes
		grid.setRune(true);
		grid.print(SCROLL_PREFIX);
		grid.print(name + 1);
		grid.setRune(false);
	} else if (name[0] == '!') {
		grid.setRune(true);
		grid.print(POTION_PREFIX);
		grid.setRune(false);
		grid.print(Data::ITEM_SHORT_NAMES[40 + index]);
	} else if (name[0] == '(') {
		grid.print("Moonstone ");
		grid.setRune(true);
		grid.printChar(name[1]);
		grid.setRune(false);
	} else {
		grid.print(name);
	}

	// The names are lined up
	for (int x = grid.column(); x < COLUMNS + 1; ++x)
		grid.printChar(' ');

	grid.printChar(10);
}

constexpr int KEY_LEFT = 1;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;
constexpr int KEY_HOME = 0xD3;
constexpr int KEY_END = 0xD4;
constexpr int KEY_PGUP = 0xD5;
constexpr int KEY_PGDN = 0xD6;
constexpr int PAGE = 7;
constexpr int PICK_FIRST_MIDDLE = 4;
constexpr int PICK_LAST_ROW = 7;
constexpr int NEXT_ROW_LIMIT = 8;
constexpr int FRAME_BOTTOM = 8;

void ItemList::start(InventoryLogic::ListKind kind, const byte *counts, int count, int who) {
	_kind = kind;
	_counts = counts;
	_count = count;
	_who = who;
	_marker = '-';
	_first = InventoryLogic::nextOwned(-1, _count, _counts, _who);
	_last = -1;
	_row = 1;
	_shown = 0;
	_item = -1;
	_arrows = 0;

	_grid.clear();
	_grid.drawFrame(FRAME_BOTTOM);

	if (_first == -1) {
		_grid.setPosition(1, 4);
		_grid.print("(None owned!)");
	} else {
		redraw();
	}
}

bool ItemList::empty() const {
	return _first == -1 && InventoryLogic::nextOwned(-1, _count, _counts, _who) == -1;
}

void ItemList::redraw() {
	const Data::Savegame &sg = _G(savegame);
	const bool pick = _who != InventoryLogic::NOTHING;

	_grid.setPosition(1, 1);
	_shown = 0;

	int item = _first;
	while (item != -1) {
		++_shown;

		byte marker = _marker;
		if (pick) {
			if (_grid.row() == _row) {
				_grid.toggleInverse();
				_item = item;
			}

			marker = InventoryLogic::isReady(sg._party[_who], item) ? Data::EQUIPMENT_MARKERS[item] : ' ';
		}

		InventoryLogic::printRow(_grid, _kind, item, _counts, marker);
		_grid.setPosition(1, _grid.row());

		if (pick && _grid.row() - 1 == _row)
			_grid.toggleInverse();

		if (_grid.row() == NEXT_ROW_LIMIT)
			break;

		item = InventoryLogic::nextOwned(item, _count, _counts, _who);
	}

	_last = item;

	_arrows = 0;
	if (InventoryLogic::prevOwned(_first, _counts, _who) != -1)
		_arrows = 2;
	if (item != -1 && InventoryLogic::nextOwned(item, _count, _counts, _who) != -1)
		++_arrows;
}

bool ItemList::scroll(int key) {
	const bool pick = _who != InventoryLogic::NOTHING;

	switch (key) {
	case KEY_LEFT:
	case KEY_UP:
	case KEY_PGUP:
	case KEY_RIGHT:
	case KEY_DOWN:
	case KEY_PGDN:
		break;

	case KEY_HOME:
		_first = InventoryLogic::nextOwned(-1, _count, _counts, _who);
		_row = 1;
		return true;

	case KEY_END:
		if (pick) {
			_first = InventoryLogic::prevOwned(_count, _counts, _who);

			for (_row = 1; _row < PAGE; ++_row) {
				const int before = InventoryLogic::prevOwned(_first, _counts, _who);
				if (before == -1)
					break;

				_first = before;
			}
		} else {
			// The last page, without the row that picks
			_first = InventoryLogic::prevOwned(_count, _counts, _who);

			for (int i = 0; i < PAGE - 1; ++i) {
				const int before = InventoryLogic::prevOwned(_first, _counts, _who);
				if (before == -1)
					break;

				_first = before;
			}
		}
		return true;

	default:
		return false;
	}

	const bool up = key == KEY_LEFT || key == KEY_UP || key == KEY_PGUP;
	const bool page = key == KEY_PGUP || key == KEY_PGDN;

	// Browsing just scrolls
	if (!pick) {
		if (key == KEY_LEFT || key == KEY_RIGHT)
			return false;

		for (int i = 0; i < (page ? PAGE : 1); ++i) {
			if (up) {
				const int before = InventoryLogic::prevOwned(_first, _counts, _who);
				if (before != -1)
					_first = before;
			} else if (_last != -1 && InventoryLogic::nextOwned(_last, _count, _counts, _who) != -1) {
				_first = InventoryLogic::nextOwned(_first, _count, _counts, _who);
				_last = InventoryLogic::nextOwned(_last, _count, _counts, _who);
			}
		}

		return true;
	}

	for (int i = 0; i < (page ? PAGE : 1); ++i) {
		if (up) {
			if (_row == PICK_FIRST_MIDDLE || _row == 1) {
				const int before = InventoryLogic::prevOwned(_first, _counts, _who);

				if (before != -1)
					_first = before;
				else if (_row == PICK_FIRST_MIDDLE)
					--_row;
			} else {
				--_row;
			}
		} else {
			if (_row == PICK_FIRST_MIDDLE || _row == PICK_LAST_ROW || _row >= _shown) {
				if (_last != -1 && InventoryLogic::nextOwned(_last, _count, _counts, _who) != -1) {
					_first = InventoryLogic::nextOwned(_first, _count, _counts, _who);
					_last = InventoryLogic::nextOwned(_last, _count, _counts, _who);
				} else if (_row == PICK_FIRST_MIDDLE && _shown > PICK_FIRST_MIDDLE) {
					++_row;
				}
			} else {
				++_row;
			}
		}
	}

	return true;
}

} // namespace Ultima5
} // namespace Ultima
