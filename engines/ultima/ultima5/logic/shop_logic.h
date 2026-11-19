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
		STEP_HEALER_PAY
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
		return _step == STEP_HEALER_MEMBER;
	}

	/**
	 * Gives the member picked, or -1 if none was
	 * @returns		True once the business is done
	 */
	bool memberChosen(int member);
};

} // namespace Ultima5
} // namespace Ultima

#endif
