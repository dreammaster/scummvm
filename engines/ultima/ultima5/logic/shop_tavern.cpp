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
#include "ultima/ultima5/logic/talk_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int TOPICS = 0x1A;
constexpr int WINES = 6;
constexpr int FOOD_PER_RATION = 0x19;
constexpr int FOOD_MAX = 9999;
constexpr int DRINKS_BEFORE_ASKING = 3;
constexpr int DRUNK_TURNS = 0x19;
constexpr int TEXT_WINE_REFUSED = 0x1413;
constexpr int TEXT_NO_MONEY_FOR_FOOD = 0x143F;
constexpr int TEXT_LORE_ANSWER = 0x134E;
constexpr int TEXT_LORE_SORRY = 0x146A;
constexpr int LORE_MAX = 15;
constexpr int MALE_GENDER = 0xB;
constexpr int KEY_SPACE_T = ' ';
constexpr int KEY_ESCAPE_T = 27;
constexpr int KEY_ENTER_T = 13;
constexpr int KEY_BACKSPACE_T = 8;
constexpr int KEY_LEFT_T = 1;
constexpr byte TILE_TABLE_FULL = 0x95;
constexpr byte TILE_TABLE_EATEN_A = 0x9B;
constexpr byte TILE_TABLE_EATEN_B = 0x9A;

// The result of what was ordered, when it wasn't just paid for
constexpr int ORDER_TAKEN = 2;

void ShopLogic::printSirOrMilady() {
	Data::Savegame &sg = _G(savegame);

	sg._activeChar = Data::NO_ACTIVE_CHARACTER;
	_log.print(sg._party[0]._gender == MALE_GENDER ? "sir" : "milady");
}

void ShopLogic::printCount() {
	switch (_diners) {
	case 2:
		_log.print("two");
		break;
	case 3:
		_log.print("three");
		break;
	case 4:
		_log.print("four");
		break;
	case 5:
		_log.print("five");
		break;
	case 6:
		_log.print("six");
		break;
	default:
		break;
	}
}

void ShopLogic::startDiners(int price) {
	const Data::Savegame &sg = _G(savegame);

	_diners = _foodCount = 0;
	_price = 0;

	for (int i = 0; i != sg._partySize; ++i) {
		if (sg._party[i]._status != Data::STATUS_DEAD) {
			++_diners;
			++_foodCount;
			_price = (int16)(_price + price);
		}
	}

	_log.printChar('\n');
	_log.printChar('\n');
}

bool ShopLogic::editLine(int key, int maxLength) {
	if ((key == KEY_BACKSPACE_T || key == KEY_LEFT_T) && !_line.empty()) {
		_log.backspace(1);
		_line.deleteLastChar();
	} else if (key == KEY_ESCAPE_T && !_line.empty()) {
		_log.backspace(_line.size());
		_line.clear();
	} else if (key == KEY_ENTER_T) {
		return true;
	} else if (key > 0x1F && key < 0x80 && (int)_line.size() < maxLength) {
		_line += (char)key;
		_log.printChar((char)key);
	}

	return false;
}

bool ShopLogic::editNumber(int key, int maxLength) {
	if (key >= '0' && key <= '9') {
		if ((int)_digits.size() < maxLength) {
			_digits += (char)key;
			_log.printChar((char)key);
		}
	} else if ((key == '-' || key == '+') && _digits.empty()) {
		_digits += (char)key;
		_log.printChar((char)key);
	} else if ((key == KEY_BACKSPACE_T || key == KEY_LEFT_T) && !_digits.empty()) {
		_digits.deleteLastChar();
		_log.backspace(1);
	} else if (key == KEY_ESCAPE_T && !_digits.empty()) {
		_log.backspace(_digits.size());
		_digits.clear();
	} else if (key == KEY_ENTER_T) {
		return true;
	}

	return false;
}

void ShopLogic::startLine(Step step) {
	_line.clear();
	_step = step;
}

void ShopLogic::startNumber(Step step) {
	_digits.clear();
	_step = step;
}

void ShopLogic::tavernOffer(int key) {
	if (key == 'N' || key == KEY_SPACE_T) {
		_log.print("No");
		finish(_bought);
	} else if (key == 'Y') {
		_log.print("Yes\n\n\"");
		sayAt(Data::TAVERN_GREETINGS[_style]);
		_log.printChar('"');
		_log.printChar(' ');

		if (_log.column() > 0xE)
			_log.printChar('\n');

		_step = STEP_TAVERN_ORDER;
	}
}

void ShopLogic::tavernMore() {
	if (_orderResult != ORDER_TAKEN)
		_bought = 1;

	_log.print("\"Anything else\nfor thee?\" ");
	_step = STEP_TAVERN_AGAIN;
}

void ShopLogic::tavernAgainAnswer(int answer) {
	if (answer == 'N') {
		_log.print("No");
		finish(_bought);
	} else {
		_log.print("Yes\n\n\"");
		sayAt(Data::TAVERN_AGAIN[_style]);
		_log.printChar('"');
		_log.printChar(' ');
		_step = STEP_TAVERN_ORDER;
	}
}

void ShopLogic::tavernOrder(int key) {
	if (key == KEY_SPACE_T || key == KEY_ESCAPE_T || key == KEY_ENTER_T) {
		finish(_bought);
		return;
	}

	const int foodKey = Data::TAVERN_FOOD_KEYS[0][_style];
	const int drinkKey = Data::TAVERN_DRINK_KEYS[0][_style];
	const int rationKey = Data::TAVERN_FOOD_COUNT_KEYS[0][_style];
	const int loreKey = Data::TAVERN_TALK_KEYS[0][_style];

	_orderResult = 0;

	if (key == foodKey) {
		_log.printChar(foodKey);
		startDiners(Data::TAVERN_MEAL_PRICES[_index]);

		if (tavernPayBill())
			_step = STEP_DONE;
		else
			tavernMore();
	} else if (key == drinkKey) {
		tavernDrink();
	} else if (key == rationKey) {
		tavernRations();
	} else if (key == loreKey) {
		// Nobody tells their secrets to someone who hasn't bought anything
		if (_bought != 0)
			tavernLore();
	}
}

// Pays for what everyone had, returning true if there's no money for it and the tavern is over
bool ShopLogic::tavernPayBill() {
	Data::Savegame &sg = _G(savegame);

	_log.printChar('"');
	_log.print("That will be ");
	_log.print(Common::String::format("%d", _price));
	_log.print(" gold for the ");
	printCount();
	_log.print(" of ye,\n");
	printSirOrMilady();
	_log.printChar('.');

	if (_price > sg._gold) {
		_log.print("\"\n\n\"CAN'T PAY?\nBeat it!\"\nyells ");
		_log.print(_keeperName);
		_log.printChar('.');
		_log.printChar('\n');
		return true;
	}

	sg._gold -= _price;
	stealGold();

	if (_foodCount != 0) {
		sg._food = MIN(sg._food + _foodCount, FOOD_MAX);

		// The dish is taken from the table
		const int x = sg._mapX;
		const int y = sg._mapY;

		if (g_engine->getMapTile(x, y - 1) == TILE_TABLE_FULL)
			_G(location).setTile(x, y - 1, TILE_TABLE_EATEN_A);
		else if (g_engine->getMapTile(x, y + 1) == TILE_TABLE_FULL)
			_G(location).setTile(x, y + 1, TILE_TABLE_EATEN_B);
	} else {
		++_drinks;
	}

	_log.print("\nEnjoy!\"\n\n");
	return false;
}

void ShopLogic::tavernDrink() {
	_log.printChar(Data::TAVERN_DRINK_KEYS[0][_style]);

	if (_drinks == DRINKS_BEFORE_ASKING) {
		_log.print("\n\n\"I beg thy\npardon, ");
		printSirOrMilady();
		_log.print(",\"\nsays ");
		_log.print(_keeperName);
		_log.print(".\n\"But haven't\nye had enough\nto drink?\" ");
		_step = STEP_TAVERN_ENOUGH;
		return;
	}

	tavernDrinkBought();
}

void ShopLogic::tavernDrinkBought() {
	startDiners(1);

	if (Data::TAVERN_DRINK_KEYS[0][_style] == 'W') {
		_log.print("\"Our wine list,\n");
		printSirOrMilady();
		_log.print(".\n\n");
		_log.print("a) Rose.......18\n");
		_log.print("b) Claret....192\n");
		_log.print("c) Sauterne...79\n");
		_log.print("d) Muscatel...30\n");
		_log.print("e) Moselle...275\n");
		_log.print("f) Chablis....98\n\n");
		_log.print("Thy choice?\" ");
		_step = STEP_TAVERN_WINE;
		return;
	}

	_foodCount = 0;
	if (tavernPayBill())
		_step = STEP_DONE;
	else
		tavernMore();
}

void ShopLogic::tavernWine(int key) {
	Data::Savegame &sg = _G(savegame);

	if (key == KEY_SPACE_T) {
		_log.printChar('\n');
		_log.printChar('\n');
		sayAt(TEXT_WINE_REFUSED);
		_orderResult = ORDER_TAKEN;
		tavernMore();
		return;
	}

	if (key < 'A' || key > 'A' + WINES - 1)
		return;

	_log.printChar((char)key);
	_log.print("\n\n\"Ah, a fine\nchoice, ");
	printSirOrMilady();
	_log.printChar('.');

	const int price = (int16)Data::TAVERN_WINE_PRICES[key - 'A'];
	if (sg._gold < price) {
		_log.print("\"\n\n\"CAN'T PAY?\nBeat it!\"\nyells ");
		_log.print(_keeperName);
		_log.printChar('.');
		_log.printChar('\n');
		_step = STEP_DONE;
		return;
	}

	sg._gold -= price;
	stealGold();
	++_drinks;
	_log.print("\nEnjoy!\"");
	_log.printChar('\n');
	_log.printChar('\n');
	tavernMore();
}

void ShopLogic::tavernRations() {
	_log.printChar(Data::TAVERN_FOOD_COUNT_KEYS[0][_style]);
	_log.printChar('\n');
	_log.printChar('\n');

	_price = discounted(Data::TAVERN_RATION_PRICES[_index]);

	_log.printChar('"');
	sayAt(Data::TAVERN_RATION_TEXT[_G(gameRandom).range(0, 6)]);
	_log.print("\n\nHow many wouldst\nthou like?\" ");
	startNumber(STEP_TAVERN_COUNT);
}

void ShopLogic::tavernRationsCount() {
	Data::Savegame &sg = _G(savegame);

	// The number typed, which can have a sign
	int wanted = 0;
	for (uint i = 0; i < _digits.size(); ++i) {
		if (_digits[i] >= '0' && _digits[i] <= '9')
			wanted = wanted * 10 + (_digits[i] - '0');
	}

	if (!_digits.empty() && _digits[0] == '-')
		wanted = -wanted;

	if (wanted == 0) {
		_log.print("\n\n\"Hrumph.\"");
		_orderResult = ORDER_TAKEN;
		tavernMore();
		return;
	}

	for (int remaining = wanted; remaining != 0; --remaining) {
		if (sg._gold >= _price) {
			sg._gold = sg._gold > _price ? sg._gold - _price : 0;
			sg._food = sg._food + FOOD_PER_RATION < FOOD_MAX ? sg._food + FOOD_PER_RATION : FOOD_MAX;

			if (sg._food == FOOD_MAX)
				break;
		} else {
			const int got = wanted - remaining;
			_log.print("\n\n");

			if (got == 0) {
				if (sg._food < 3) {
					sg._food += _G(gameRandom).range(0, 1) + 1;
					sayAt(TEXT_NO_MONEY_FOR_FOOD);
				} else {
					_log.print("\"Thou hast\nneither gold nor\nneed! Out!\"\n");
					_log.print("yells ");
					_log.print(_keeperName);
					_log.print(".\n");
				}

				_step = STEP_DONE;
			} else {
				_log.print("\"Thou canst\nafford only ");
				_log.print(Common::String::format("%d", got));
				_log.print("!\"\n\n");
				tavernMore();
			}

			return;
		}
	}

	_log.print("\n\n");
	stealGold();
	tavernMore();
}

void ShopLogic::tavernLore() {
	_log.printChar(Data::TAVERN_TALK_KEYS[0][_style]);
	_log.print("\n\n\"");
	tavernLorePrompt();
}

void ShopLogic::tavernLorePrompt() {
	_log.print("Of what wouldst\nthou hear my\nlore, ");
	printSirOrMilady();
	_log.print("?\"\n\nYou respond:\n");
	startLine(STEP_TAVERN_LORE);
}

void ShopLogic::tavernLoreEntered() {
	_log.print("\n\n");

	if (_line.empty()) {
		tavernMore();
		return;
	}

	int topic;
	for (topic = 0; topic < TOPICS; ++topic) {
		const int pos = TalkLogic::findWord((const byte *)Data::TAVERN_TOPICS[topic], _line);
		if (pos == 0 || (pos > 0 && _line[pos - 1] == ' '))
			break;
	}

	if (topic == TOPICS) {
		_log.print("\"That, I cannot help thee with.\n\n");
		tavernLorePrompt();
		return;
	}

	_topic = topic;
	_price = (int16)Data::TAVERN_TOPIC_PRICES[topic];
	sayAt(TEXT_LORE_ANSWER);
	_log.print("\n\nFair 'nuff?\" ");
	_step = STEP_TAVERN_LORE_CONFIRM;
}

void ShopLogic::tavernLoreAnswer(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No\n\n");
		tavernMore();
		return;
	}

	_log.print("Yes\n\n");

	if (_price > sg._gold) {
		_log.print("\"Sorry, ");
		printSirOrMilady();
		sayAt(TEXT_LORE_SORRY);
		_step = STEP_DONE;
		return;
	}

	sg._gold -= _price;

	_item1 = Data::TAVERN_TOPIC_ITEMS[_topic];
	_item2 = Data::TAVERN_PLACES[Data::TAVERN_TOPIC_PLACES[_topic]];

	sayAt(Data::TAVERN_ANSWERS[_G(gameRandom).range(0, 3)]);
	_log.print("\nsays ");
	_log.print(_keeperName);
	_log.print(".\n\n");
	tavernMore();
}

} // namespace Ultima5
} // namespace Ultima
