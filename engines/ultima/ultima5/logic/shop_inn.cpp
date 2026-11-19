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
#include "ultima/ultima5/logic/location_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int KEY_SPACE_I = ' ';
constexpr int KEY_ESCAPE_I = 27;
constexpr int KEY_ENTER_I = 13;
constexpr int KEY_LEFT_I = 1;
constexpr int KEY_RIGHT_I = 2;
constexpr int KEY_UP_I = 3;
constexpr int KEY_DOWN_I = 4;
constexpr int FEMALE_GENDER = 0xC;
constexpr int ROSTER_SIZE = 16;
constexpr int PARTY_FULL = 6;
constexpr int MONTH_MULTIPLIER = 10;
constexpr int NIGHT_STEPS = 12;
constexpr int NIGHT_STEP_MINUTES = 5;
constexpr int MORNING_STEP_MINUTES = 9;
constexpr int MORNING_HOUR = 6;
constexpr int DUSK_HOUR = 0x14;
constexpr int DAWN_HOUR = 5;
constexpr int REGENERATION_RING = 0x2C;
constexpr int REGENERATION_ODDS = 7;
constexpr int SHIP_OWNED_MASK = 0xC0;
constexpr int SHIP_FRIGATE_BIT = 0x80;
constexpr int SHIP_SKIFF_BIT = 0x40;
constexpr int SHIP_FRIGATE_STATE = 0x82;
constexpr int SHIP_PRICE_TOO_HIGH = 10000;

constexpr int TEXT_SHIP_MENU = 0x18EB;
constexpr int TEXT_SHIP_OWNED = 0x19F2;
constexpr int TEXT_SHIP_FRIGATE = 0x183E;
constexpr int TEXT_SHIP_SKIFF = 0x188C;
constexpr int TEXT_SHIP_ASK = 0x1A50;
constexpr int TEXT_SHIP_NO_MONEY = 0x198E;
constexpr int TEXT_SHIP_DELIVERED = 0x19AB;
constexpr int TEXT_SHIP_ASK_MORE = 0x19DA;
constexpr int TEXT_SHIP_SECOND_SKIFF = 0x193B;
constexpr int TEXT_SHIP_THIRD_SKIFF = 0x195F;
constexpr int TEXT_INN_ALONE = 0x26E8;
constexpr int TEXT_INN_DEAD = 0x2723;
constexpr int TEXT_INN_NO_MONEY = 0x275A;

// ---- The shipwright

bool ShopLogic::shipAfford(int price) {
	if (price > _G(savegame)._gold) {
		sayAt(TEXT_SHIP_NO_MONEY);
		say("yells $.\n");
		_shipFailed = true;
		return false;
	}

	return true;
}

void ShopLogic::shipEnd() {
	if (_shipFailed)
		_step = STEP_DONE;
	else
		finish(_shipBought ? 1 : 0);
}

void ShopLogic::shipOffer(int key) {
	if (key == 'Y') {
		_log.print("Yes");
		shipMenu();
	} else if (key == KEY_SPACE_I || key == 'N') {
		_log.print("No");
		shipEnd();
	}
}

void ShopLogic::shipMenu() {
	sayAt(TEXT_SHIP_MENU);
	_step = STEP_SHIP_MENU;
}

void ShopLogic::shipKey(int key) {
	Data::Savegame &sg = _G(savegame);

	switch (key) {
	case 'F':
		_log.print("F\n\n");

		if ((sg._unk105F & SHIP_OWNED_MASK) != 0) {
			sayAt(TEXT_SHIP_OWNED);
			_step = STEP_SHIP_OWNED;
		} else {
			_price = discounted(Data::FRIGATE_PRICES[_index]);
			sayAt(TEXT_SHIP_FRIGATE);
			sayAt(TEXT_SHIP_ASK);
			_step = STEP_SHIP_FRIGATE;
		}
		break;

	case 'S':
		_log.print("S\n\n");
		_price = discounted(Data::SKIFF_PRICES[_index]);
		sayAt(TEXT_SHIP_SKIFF);
		sayAt(TEXT_SHIP_ASK);
		_step = STEP_SHIP_SKIFF;
		break;

	case KEY_ESCAPE_I:
	case KEY_SPACE_I:
		shipEnd();
		break;

	default:
		// Anything else, and the keeper says it again
		shipMenu();
		break;
	}
}

void ShopLogic::shipOwned(int answer) {
	if (answer == 'Y') {
		_log.print("Yes");
		shipAfford(SHIP_PRICE_TOO_HIGH);
		shipEnd();
	} else {
		_log.print("No");
		shipMenu();
	}
}

void ShopLogic::shipFrigate(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No");
		shipEnd();
		return;
	}

	_log.print("Yes\n\n");
	if (!shipAfford(_price)) {
		shipEnd();
		return;
	}

	sg._unk105F = SHIP_FRIGATE_STATE;
	shipDeal(true, true);
}

void ShopLogic::shipSkiff(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No");
		shipEnd();
		return;
	}

	_log.print("Yes\n\n");
	if (!shipAfford(_price)) {
		shipEnd();
		return;
	}

	if ((sg._unk105F & SHIP_OWNED_MASK) == 0) {
		sg._unk105F = SHIP_SKIFF_BIT;
		shipDeal(true, true);
	} else if ((sg._unk105F & SHIP_FRIGATE_BIT) != 0) {
		sayAt(TEXT_SHIP_SECOND_SKIFF);
		++sg._unk105F;
		shipDeal(false, true);
	} else {
		sayAt(TEXT_SHIP_THIRD_SKIFF);
		shipDeal(false, false);
	}
}

// Where the ship is put, what's paid, and whether anything else is wanted
void ShopLogic::shipDeal(bool place, bool pay) {
	Data::Savegame &sg = _G(savegame);

	if (place) {
		sayAt(TEXT_SHIP_DELIVERED);
		sg._shipX = Data::SHIP_X[_index];
		sg._shipY = Data::SHIP_Y[_index];
	}

	if (pay) {
		sg._gold -= _price;
		_shipBought = true;
		stealGold();
	}

	sayAt(TEXT_SHIP_ASK_MORE);

	// Whoever's buying is always spoken to as a man
	_log.print("sir");
	_log.print("?\" ");
	_step = STEP_SHIP_AGAIN;
}

void ShopLogic::shipAgain(int answer) {
	if (answer == 'Y') {
		_log.print("Yes");
		shipMenu();
	} else {
		_log.print("No");
		shipEnd();
	}
}

// ---- The inn

int ShopLogic::innGuests() const {
	const Data::Savegame &sg = _G(savegame);
	int count = 0;

	for (int i = 0; i < ROSTER_SIZE; ++i) {
		if (sg._party[i]._mapId == sg._mapId)
			++count;
	}

	return count;
}

int ShopLogic::innPrevGuest(int from) const {
	const Data::Savegame &sg = _G(savegame);

	while (--from) {
		if (sg._party[from]._mapId == sg._mapId)
			break;
	}

	return from;
}

int ShopLogic::innNextGuest(int from) const {
	const Data::Savegame &sg = _G(savegame);

	while (++from < ROSTER_SIZE) {
		if (sg._party[from]._mapId == sg._mapId)
			break;
	}

	return from == ROSTER_SIZE ? 0 : from;
}

// What the rooms cost, for each of the people staying
int ShopLogic::innPrice(int multiplier) const {
	const int intel = _G(savegame)._party[_member]._int;
	int32 price = (int16)(Data::INN_PRICES[_index] * multiplier);

	price += price * -((int32)(intel * 3) - 100) / 100;
	return (int16)price;
}

bool ShopLogic::innRoom() {
	_log.print("\n\n");

	if (innGuests() < Data::INN_ROOMS[_index])
		return true;

	_log.print("\"I am sorry,\n");
	_log.print(_G(savegame)._party[_member]._gender == FEMALE_GENDER ? "milady" : "sir");
	_log.print(", but we\nhave no room\navailable.\"\n\n");
	return false;
}

void ShopLogic::innOffer(int key) {
	if (key == 'N') {
		_log.print("No");
		finish(_innCode);
	} else if (key == 'Y') {
		_log.print("Yes");
		say("\n\n$ asks,\n\"Art thou here\nto Pick up or\n");
		_log.print("Leave a\ncompanion, or\nto Rest for the\nnight?\" ");
		_step = STEP_INN_CHOOSE;
	}
}

// What follows a thing done at the inn: either the business is over or the keeper asks if there is more
void ShopLogic::innAfter(bool done) {
	if (done) {
		finish(_innCode);
		return;
	}

	_log.print("\"Is there\nanything more\nI can do for\nthee?\" ");
	_step = STEP_INN_OFFER;
}

void ShopLogic::innChoose(int key) {
	switch (key) {
	case 'R':
		innRest();
		break;

	case KEY_SPACE_I:
		innAfter(true);
		break;

	case 'L':
		innLeave();
		break;

	case 'P':
		innPickUp();
		break;

	default:
		break;
	}
}

void ShopLogic::innRest() {
	Data::Savegame &sg = _G(savegame);
	(void)sg;

	_log.printChar('R');
	if (!innRoom()) {
		innAfter(false);
		return;
	}

	_log.printChar('"');
	_price = innPrice(_G(savegame)._partySize);
	sayAt(Data::INN_TEXT[_index]);
	_log.print("\nWilt thou take\nit?\" ");
	_step = STEP_INN_REST_CONFIRM;
}

void ShopLogic::innRestAnswer(int answer) {
	Data::Savegame &sg = _G(savegame);

	_log.print(answer == 'Y' ? "Yes" : "No");
	_log.print("\n\n");

	if (answer == 'N') {
		innAfter(true);
		return;
	}

	if (sg._gold < _price) {
		_log.print("\"Highwaymen!\nCheap, at that!\nOUT!\" ");
		say("screams\n$.\n");
		innAfter(true);
		return;
	}

	sg._gold -= _price;
	stealGold();

	_log.print("\"Have a pleasant\nnight, ");
	_log.print(sg._party[_member]._gender == FEMALE_GENDER ? "milady" : "sir");
	say("!\"\nsays $.\n\n");

	innSleep();
	_innCode = -1;
	innAfter(true);
}

void ShopLogic::innSleep() {
	Data::Savegame &sg = _G(savegame);

	// The door that was opened is shut again
	if (sg._doorOpen != 0 && sg._mapId != 0 && sg._mapId < 0x21)
		_G(location).setTile(sg._doorX, sg._doorY, sg._doorOpen);

	sg._mapX = Data::INN_BED_X[_index];
	sg._mapY = Data::INN_BED_Y[_index];

	const byte active = sg._activeChar;

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_GOOD)
			sg._party[i]._status = Data::STATUS_SLEEP;
	}

	_log.print("Zzzzzz....\n\n");

	for (int i = 0; i < NIGHT_STEPS; ++i)
		TimeLogic::advance(sg, NIGHT_STEP_MINUTES);

	while (sg._hour != MORNING_HOUR) {
		// Rings of regeneration heal a little as the night goes
		for (int i = 0; i < sg._partySize; ++i) {
			Data::Character &who = sg._party[i];

			if (who._status != Data::STATUS_DEAD && who._equipment[4] == REGENERATION_RING &&
					_G(gameRandom).range(0, REGENERATION_ODDS) == REGENERATION_ODDS) {
				who._hp = who._hp + 1 < who._maxHp ? who._hp + 1 : who._maxHp;
			}
		}

		TimeLogic::advance(sg, MORNING_STEP_MINUTES);

		if (sg._hour == DUSK_HOUR || sg._hour == DAWN_HOUR)
			LocationLogic::updateForHour();
	}

	_log.print("Morning!\n");
	NpcLogic::place(sg);

	for (int i = 0; i < sg._partySize; ++i) {
		Data::Character &who = sg._party[i];
		if (who._status == Data::STATUS_DEAD)
			continue;

		who._hp = who._maxHp;

		if (who._class == 'A' || who._class == 'M')
			who._mag = who._int;
		else if (who._class == 'B')
			who._mag = who._int >> 1;

		if (who._status == Data::STATUS_POISONED) {
			who._status = Data::STATUS_DEAD;
			who._hp = 0;

			_log.printChar('\n');
			_log.print(who._name);
			_log.print(" has\npassed away.\n");
		} else if (who._status == Data::STATUS_SLEEP) {
			who._status = Data::STATUS_GOOD;
		}
	}

	sg._activeChar = active;
	++sg._mapX;
	_G(visibility).invalidate();
}

void ShopLogic::innLeave() {
	Data::Savegame &sg = _G(savegame);

	_log.printChar('L');
	if (!innRoom()) {
		innAfter(_innCode == -1);
		return;
	}

	if (sg._partySize == 1) {
		sayAt(TEXT_INN_ALONE);
		_innCode = -1;
		innAfter(true);
		return;
	}

	innLeaveWho();
}

void ShopLogic::innLeaveWho() {
	say("$ asks,\n\"Who will\nstay?\" ");
	_step = STEP_INN_LEAVE_MEMBER;
}

void ShopLogic::innLeaveMember(int member) {
	Data::Savegame &sg = _G(savegame);

	if (member == -1) {
		_log.print("Nobody\n\n");
		innAfter(_innCode == -1);
		return;
	}

	_log.print("\n\n");

	if (member == 0) {
		_log.print("Thy friend");
		if (sg._partySize > 2)
			_log.printChar('s');
		_log.print(" will not leave thee!\n\n");
		innLeaveWho();
	} else if (sg._party[member]._status == Data::STATUS_DEAD) {
		sayAt(TEXT_INN_DEAD);
		_innCode = -1;
		innAfter(true);
	} else {
		_target = member;
		_price = innPrice(MONTH_MULTIPLIER);

		_log.print("\"The rate for\nour most comfortable room will be ");
		say("% gold per month, due at check-out.");
		_log.print("\nWilt thou take\nit?\" ");
		_step = STEP_INN_LEAVE_CONFIRM;
	}
}

void ShopLogic::innLeaveAnswer(int answer) {
	Data::Savegame &sg = _G(savegame);

	_log.print(answer == 'Y' ? "Yes" : "No");
	_log.print("\n\n");

	if (answer == 'Y') {
		_innCode = 1;
		const int who = _target;

		if (sg._activeChar == who)
			sg._activeChar = Data::NO_ACTIVE_CHARACTER;
		else if (who < sg._activeChar && sg._activeChar != Data::NO_ACTIVE_CHARACTER)
			--sg._activeChar;

		sg._party[who]._mapId = sg._mapId;
		sg._party[who]._unk17 = 0;

		const Data::Character left = sg._party[who];
		for (int i = who; i < ROSTER_SIZE - 1; ++i)
			sg._party[i] = sg._party[i + 1];
		sg._party[ROSTER_SIZE - 1] = left;

		--sg._partySize;
		say("\"I thank thee.\"\nsays $.\n\n");
	}

	innAfter(_innCode == -1);
}

void ShopLogic::innPickUp() {
	Data::Savegame &sg = _G(savegame);
	const int guests = innGuests();

	if (sg._partySize == PARTY_FULL) {
		_log.print("\n\nOne must first be left behind!\n\n");
		innAfter(_innCode == -1);
	} else if (guests == 0) {
		say("\n\n\"No one here is from thy party!\"\nsays $.\n\n");
		innAfter(_innCode == -1);
	} else if (guests > 1) {
		_log.print("\n\n\"Who will\ncheck out?\" ");
		_guest = innNextGuest(0);
		_step = STEP_INN_GUEST;
	} else {
		_guest = innNextGuest(0);
		innCheckOut();
	}
}

void ShopLogic::innGuestKey(int key) {
	switch (key) {
	case KEY_LEFT_I:
	case KEY_UP_I: {
		const int other = innPrevGuest(_guest);
		if (other != 0)
			_guest = other;
		break;
	}

	case KEY_RIGHT_I:
	case KEY_DOWN_I: {
		const int other = innNextGuest(_guest);
		if (other != 0)
			_guest = other;
		break;
	}

	case KEY_ESCAPE_I:
		_log.print("No one\n\n");
		_guest = 0;
		innCheckOut();
		break;

	case KEY_SPACE_I:
	case KEY_ENTER_I:
		innCheckOut();
		break;

	default:
		break;
	}
}

void ShopLogic::innCheckOut() {
	Data::Savegame &sg = _G(savegame);

	if (_guest != 0) {
		int days = sg._party[_guest]._unk17;
		if (days == 0)
			days = 1;

		_price = (int16)(days * innPrice(MONTH_MULTIPLIER));

		say("\n\n\"That will be % gold, please.\"\n\n\"");

		if (sg._gold < _price) {
			sayAt(TEXT_INN_NO_MONEY);
			_innCode = -1;
		} else {
			sg._gold -= _price;
			stealGold();
			_innCode = 1;

			const Data::Character guest = sg._party[_guest];
			for (int i = _guest; i > sg._partySize; --i)
				sg._party[i] = sg._party[i - 1];
			sg._party[sg._partySize] = guest;

			Data::Character &joined = sg._party[sg._partySize++];
			joined._mapId = 0;

			if (joined._status == Data::STATUS_POISONED) {
				joined._status = Data::STATUS_DEAD;
				joined._hp = 0;
				_log.print("Thy friend has died, by the way.\"\n");
			} else {
				joined._hp = joined._maxHp;

				if (joined._class == 'A' || joined._class == 'M')
					joined._mag = joined._int;
				else if (joined._class == 'B')
					joined._mag = joined._int >> 1;

				_log.print("I hope thou hast found thy stay enjoyable,\"\n");
			}

			say("says $.\n\n");
		}
	}

	innAfter(_innCode == -1);
}

bool ShopLogic::guestWindow(Common::Array<Common::String> &names, int &selected) const {
	if (_step != STEP_INN_GUEST)
		return false;

	const Data::Savegame &sg = _G(savegame);
	names.clear();
	selected = -1;

	for (int i = 1; i < ROSTER_SIZE; ++i) {
		if (sg._party[i]._mapId == sg._mapId) {
			if (i == _guest)
				selected = names.size();

			names.push_back(sg._party[i]._name);
		}
	}

	return true;
}

} // namespace Ultima5
} // namespace Ultima
