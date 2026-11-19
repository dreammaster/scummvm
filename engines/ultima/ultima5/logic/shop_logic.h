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

#ifndef ULTIMA5_LOGIC_SHOP_LOGIC_H
#define ULTIMA5_LOGIC_SHOP_LOGIC_H

#include "common/array.h"
#include "common/str.h"
#include "ultima/ultima5/data/shop_data.h"
#include "ultima/ultima5/views/message_log.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Doing business with a shopkeeper. The shopkeepers and what they say come from
 * the SHOPPE.DAT file, with the names of the shop and the shopkeeper, the
 * time of day and the price and amount worked into it
 */
class ShopLogic {
private:
	// The kinds of shop, in the order the people who keep them are numbered
	enum Kind {
		KIND_WEAPONS = 0,
		KIND_TAVERN = 1,
		KIND_STABLES = 2,
		KIND_SHIPWRIGHT = 3,
		KIND_PROVISIONER = 4,
		KIND_GUILD = 5,
		KIND_HEALER = 6,
		KIND_INN = 7
	};

	enum Step {
		STEP_DONE,
		STEP_ANY_KEY,
		STEP_GUILD_OFFER,
		STEP_GUILD_MENU,
		STEP_GUILD_CONFIRM,
		STEP_PROVISIONER_OFFER,
		STEP_PROVISIONER_MENU,
		STEP_PROVISIONER_CONFIRM,
		STEP_STABLES_OFFER,
		STEP_STABLES_DEAL,
		STEP_HEALER_OFFER,
		STEP_HEALER_NEED,
		STEP_HEALER_MEMBER,
		STEP_HEALER_PAY,
		STEP_WEAPONS_WELCOME,
		STEP_WEAPONS_CHOOSE,
		STEP_WEAPONS_MENU,
		STEP_WEAPONS_CONFIRM,
		STEP_WEAPONS_SOLD,
		STEP_WEAPONS_SELL,
		STEP_WEAPONS_SELL_CONFIRM,
		STEP_TAVERN_OFFER,
		STEP_TAVERN_ORDER,
		STEP_TAVERN_ENOUGH,
		STEP_TAVERN_WINE,
		STEP_TAVERN_COUNT,
		STEP_TAVERN_LORE,
		STEP_TAVERN_LORE_CONFIRM,
		STEP_TAVERN_AGAIN,
		STEP_SHIP_OFFER,
		STEP_SHIP_MENU,
		STEP_SHIP_OWNED,
		STEP_SHIP_FRIGATE,
		STEP_SHIP_SKIFF,
		STEP_SHIP_AGAIN,
		STEP_INN_OFFER,
		STEP_INN_CHOOSE,
		STEP_INN_REST_CONFIRM,
		STEP_INN_LEAVE_MEMBER,
		STEP_INN_LEAVE_CONFIRM,
		STEP_INN_GUEST
	};

	// What was asked of the healer
	enum Need {
		NEED_CURE,
		NEED_HEAL,
		NEED_RESURRECT
	};

	Views::MessageLog &_log;
	byte *_text = nullptr;
	int _textSize = 0;

	int _kind = 0;
	int _index = 0;
	int _member = 0;
	int _price = 0;
	int _amount = 0;
	Common::String _shopName;
	Common::String _keeperName;

	Step _step = STEP_DONE;
	Step _afterKey = STEP_DONE;
	int _code = 0;
	int _item = 0;
	int _slot = 0;
	int _horseX = 0;
	int _horseY = 0;
	Need _need = NEED_CURE;
	int _target = 0;

	// The weapons shop's window of things to sell
	int _sellTop = 0;
	int _sellSelected = 0;
	int _sellRow = 0;

	// The tavern
	int _style = 0;
	int _bought = 0;
	int _diners = 0;
	int _foodCount = 0;
	int _drinks = 0;
	int _topic = 0;
	int _count = 0;
	int _orderResult = 0;
	Common::String _line;
	Common::String _digits;
	bool _noMoney = false;
	bool _shipFailed = false;
	bool _shipBought = false;
	Common::String _item1;
	Common::String _item2;

	// The inn
	int _guest = 0;
	int _innCode = 0;

	/**
	 * Says what's at a place in the shop text file, or a string that has the
	 * same codes in it
	 */
	void sayText(const byte *text);
	void sayAt(int offset);
	void say(const char *text);

	/**
	 * What shopkeepers do whatever they sell
	 */
	int discounted(int price) const;
	void stealGold();
	void greet();
	void finish(int code);
	bool paid(int price);
	void waitKey(Step next);
	static int upperKey(const Graphics::Views::KeypressMessage &msg);
	bool isFemale() const;

	/**
	 * The guild's keys, gems and torches
	 */
	void guildMenu();
	void guildItem(int item);
	void guildAnswer(int answer);

	/**
	 * The provisioner's reagents
	 */
	void provisionerMenu();
	void provisionerItem(int choice);
	void provisionerAnswer(int answer);

	/**
	 * The stables
	 */
	bool findStall();
	void stablesDeal(int answer);

	/**
	 * The healers
	 */
	void healerOffer(int answer);
	void healerNeed(int key);
	void healerService();
	void healerPay(int answer);
	void healerAgain();

	/**
	 * The shops that sell weapons and armour
	 */
	void weaponsWelcome();
	void weaponsChoose(int key);
	void weaponsMenu();
	void weaponsItem(int choice);
	void weaponsAnswer(int answer);
	void weaponsAfterBuy();
	void weaponsSell();
	void weaponsSellKey(int key);
	void weaponsSellAnswer(int answer);
	void weaponsSellContinue();
	void weaponsSellDone(bool refused);
	bool anyEquipment() const;
	int prevOwned(int from) const;
	int nextOwned(int from) const;
	void settleSellList();

	/**
	 * The taverns
	 */
	void tavernOffer(int key);
	void tavernOrder(int key);
	void tavernMore();
	void tavernAgainAnswer(int answer);
	bool tavernPayBill();
	void tavernDrink();
	void tavernDrinkBought();
	void tavernWine(int key);
	void tavernRations();
	void tavernRationsCount();
	void tavernLore();
	void tavernLorePrompt();
	void tavernLoreEntered();
	void tavernLoreAnswer(int answer);
	void startDiners(int price);
	void printCount();
	void printSirOrMilady();
	void startLine(Step step);
	void startNumber(Step step);
	bool editLine(int key, int maxLength);
	bool editNumber(int key, int maxLength);

	/**
	 * The shipwrights
	 */
	void shipOffer(int key);
	void shipMenu();
	void shipKey(int key);
	void shipOwned(int answer);
	void shipFrigate(int answer);
	void shipSkiff(int answer);
	void shipAgain(int answer);
	void shipDeal(bool place, bool pay);
	void shipEnd();
	bool shipAfford(int price);

	/**
	 * The inns
	 */
	void innOffer(int key);
	void innChoose(int key);
	void innAfter(bool done);
	void innRest();
	void innRestAnswer(int answer);
	void innSleep();
	void innLeave();
	void innLeaveWho();
	void innLeaveMember(int member);
	void innLeaveAnswer(int answer);
	void innPickUp();
	void innGuestKey(int key);
	void innCheckOut();
	bool innRoom();
	int innGuests() const;
	int innNextGuest(int from) const;
	int innPrevGuest(int from) const;
	int innPrice(int multiplier) const;

public:
	ShopLogic(Views::MessageLog &log);
	~ShopLogic();

	/**
	 * Returns true if the shopkeepers a person can be are ones that can be done business with
	 */
	static bool isSupported(int dialog);

	/**
	 * Starts doing business with a shopkeeper
	 * @param dialog	What the person says, from 0x81 for each kind of shop
	 * @returns			True if that was all there was to it
	 */
	bool start(int dialog);

	/**
	 * Handles a keypress
	 * @returns		True once the business is done
	 */
	bool keypress(const Graphics::Views::KeypressMessage &msg);

	/**
	 * Returns true if a member of the party has to be picked
	 */
	bool needsMember() const {
		return _step == STEP_HEALER_MEMBER || _step == STEP_INN_LEAVE_MEMBER;
	}

	/**
	 * Gives the member picked, or -1 if none was
	 * @returns		True once the business is done
	 */
	bool memberChosen(int member);

	/**
	 * What's in the window of things to sell
	 */
	struct SellWindow {
		Common::String rows[4];
		int selected = -1;
		int arrows = 0;
	};

	/**
	 * Returns true if the window of things to sell is up, filling in what's in it
	 */
	bool sellWindow(SellWindow &window) const;

	/**
	 * Returns true if the inn's register of guests is up, with their names
	 * and which is picked
	 */
	bool guestWindow(Common::Array<Common::String> &names, int &selected) const;
};

} // namespace Ultima5
} // namespace Ultima

#endif
