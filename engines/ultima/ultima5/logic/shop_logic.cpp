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

#include "common/file.h"
#include "common/util.h"
#include "ultima/ultima5/logic/shop_logic.h"
#include "ultima/ultima5/logic/inventory_logic.h"
#include "ultima/ultima5/data/talk_data.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int FIRST_SHOP_DIALOG = 0x81;
constexpr int SHOP_FILE_SIZE = 0x2800;
constexpr int TEXT_LIMIT = 1500;
constexpr byte FIRST_WORD = 0x80;

constexpr byte TILE_RIDING_HORSE = 0x12;
constexpr byte ACTOR_HORSE = 0x10;
constexpr byte TILE_STABLE_A = 0x44;
constexpr byte TILE_STABLE_B = 0x45;
constexpr byte TILE_GRASS = 5;
constexpr int FEMALE = 12;
constexpr int ITEM_MAX = 99;
constexpr int GOLD_MAX = 9999;
constexpr int GUILD_ITEMS = 3;
constexpr int PROVISIONS = 8;
constexpr int STEAL_MAX = 0x40;
constexpr int NO_ONE = -1;
constexpr int FREE_MAP = 5;
constexpr int SOMETIMES_FREE_MAP = 7;
constexpr int FREE_PRICE_LIMIT = 100;
constexpr int KEY_LEFT = 1;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;
constexpr int KEY_BACKSPACE = 8;
constexpr int KEY_HOME = 0xD3;
constexpr int KEY_END = 0xD4;
constexpr int KEY_PGUP = 0xD5;
constexpr int KEY_PGDN = 0xD6;
constexpr int KEY_ESCAPE = 27;
constexpr int KEY_ENTER = 13;
constexpr int KEY_SPACE = ' ';
constexpr int AFTERNOON_HOUR = 0xC;
constexpr int EVENING_HOUR = 0x12;

// What it costs to get a charge at the guild
constexpr int GUILD_KEYS_PER_BUY = 3;
constexpr int GUILD_GEMS_PER_BUY = 4;
constexpr int GUILD_TORCHES_PER_BUY = 5;

// The offsets in the shop file of things said in more than one place
constexpr int TEXT_SHORT_OF_GOLD_GUILD = 0x21E6;
constexpr int TEXT_GUILD_EXTRA = 8725;
constexpr int TEXT_SHORT_OF_GOLD_PROVISIONER = 0x1A67 + 0x4C4;
constexpr int TEXT_STABLES_PITCH = 5699;
constexpr int TEXT_HEALER_CANT_PAY = 9131;

static const int8 STABLE_DX[4] = { 0, 0, 1, -1 };
static const int8 STABLE_DY[4] = { 1, -1, 0, 0 };

ShopLogic::ShopLogic(Views::MessageLog &log) : _log(log) {
	_text = new byte[SHOP_FILE_SIZE];
	memset(_text, 0, SHOP_FILE_SIZE);

	Common::File f;
	if (f.open("shoppe.dat"))
		_textSize = f.read(_text, SHOP_FILE_SIZE - 1);
}

ShopLogic::~ShopLogic() {
	delete[] _text;
}

bool ShopLogic::isSupported(int dialog) {
	return dialog >= FIRST_SHOP_DIALOG && dialog < FIRST_SHOP_DIALOG + Data::SHOP_KINDS;
}

void ShopLogic::sayText(const byte *text) {
	Common::String out;
	const byte *start = text;

	for (const byte *p = text; *p != 0; ++p) {
		const byte ch = *p;
		const bool special = ch == '#' || ch == '$' || ch == '@' || ch == '%' || ch == '^' || ch == '&' ||
			ch == '*' || ch >= FIRST_WORD;

		if (special) {
			out += Common::String((const char *)start, p - start);
			start = p + 1;
		}

		if (ch >= FIRST_WORD) {
			out += ' ';
			out += Data::TALK_WORDS[ch - FIRST_WORD];
			if (p[1] != 0 && p[1] < FIRST_WORD)
				out += ' ';
			continue;
		}

		switch (ch) {
		case '#':
			out += _shopName;
			break;
		case '$':
			out += _keeperName;
			break;
		case '@':
			if (_G(savegame)._hour < AFTERNOON_HOUR)
				out += "morning";
			else if (_G(savegame)._hour < EVENING_HOUR)
				out += "afternoon";
			else
				out += "evening";
			break;
		case '&':
			out += _item1;
			break;
		case '*':
			out += _item2;
			break;
		case '%':
			out += Common::String::format("%d", _price);
			break;
		case '^':
			out += Common::String::format("%d", _amount);
			break;
		default:
			break;
		}
	}

	out += (const char *)start;
	_log.print(out);
}

void ShopLogic::sayAt(int offset) {
	byte buffer[TEXT_LIMIT + 1];
	int length = 0;

	for (; length < TEXT_LIMIT && offset + length < SHOP_FILE_SIZE && _text[offset + length] != 0; ++length)
		buffer[length] = _text[offset + length];

	buffer[length] = 0;
	sayText(buffer);
}

void ShopLogic::say(const char *text) {
	sayText((const byte *)text);
}

int ShopLogic::discounted(int price) const {
	const int intel = _G(savegame)._party[_member]._int;
	const int32 base = (int16)price;
	const uint32 change = (uint32)(base * -((int32)(intel * 3) - 100)) / 100U;

	return (int16)((uint32)base + change);
}

void ShopLogic::stealGold() {
	Data::Savegame &sg = _G(savegame);

	if (sg._unk03B2 == 0) {
		const int amount = _G(gameRandom).range(1, STEAL_MAX);
		sg._gold = sg._gold > amount ? sg._gold - amount : 0;
	}
}

bool ShopLogic::isFemale() const {
	return _G(savegame)._party[_member]._gender == FEMALE;
}

int ShopLogic::upperKey(const Graphics::Views::KeypressMessage &msg) {
	switch (msg.keycode) {
	case Common::KEYCODE_LEFT:
		return KEY_LEFT;
	case Common::KEYCODE_RIGHT:
		return KEY_RIGHT;
	case Common::KEYCODE_UP:
		return KEY_UP;
	case Common::KEYCODE_DOWN:
		return KEY_DOWN;
	case Common::KEYCODE_HOME:
		return KEY_HOME;
	case Common::KEYCODE_END:
		return KEY_END;
	case Common::KEYCODE_PAGEUP:
		return KEY_PGUP;
	case Common::KEYCODE_PAGEDOWN:
		return KEY_PGDN;
	case Common::KEYCODE_BACKSPACE:
		return KEY_BACKSPACE;
	case Common::KEYCODE_ESCAPE:
		return KEY_ESCAPE;
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
		return KEY_ENTER;
	case Common::KEYCODE_SPACE:
		return KEY_SPACE;
	default:
		return toupper(msg.ascii);
	}
}

void ShopLogic::waitKey(Step next) {
	_afterKey = next;
	_step = STEP_ANY_KEY;
}

void ShopLogic::greet() {
	_log.printChar('"');
	sayAt(Data::SHOP_GREETINGS[_kind][_G(gameRandom).range(0, 3)]);

	if (_log.column() > 0xB)
		_log.print("\n\n:");
	else if (_log.column() == 0)
		_log.print("\n:");
	else
		_log.printChar(' ');
}

void ShopLogic::finish(int code) {
	if (code == 0) {
		_log.print("\n\n\"");
		sayAt(Data::SHOP_GOODBYES[_kind][_G(gameRandom).range(0, 3)]);
	} else if (code == 1) {
		_log.print("\n\n\"");
		sayAt(Data::SHOP_THANKS[_kind][_G(gameRandom).range(0, 3)]);
	}

	if (code == 0 || code == 1) {
		if (_log.column() != 0)
			_log.printChar('\n');

		say("says $.\n");
	}

	_step = STEP_DONE;
}

bool ShopLogic::start(int dialog) {
	Data::Savegame &sg = _G(savegame);
	_kind = dialog - FIRST_SHOP_DIALOG;

	if ((sg._partyTile & 0xFE) == TILE_RIDING_HORSE && dialog != FIRST_SHOP_DIALOG + KIND_STABLES) {
		_log.print("A merchant says:\n\"GET THAT HORSE OUT OF HERE!\"\n");
		return true;
	}

	// Whoever is doing the buying
	_member = 0;
	if (sg._activeChar != Data::NO_ACTIVE_CHARACTER) {
		_member = sg._activeChar;
	} else {
		for (int i = 0; i < sg._partySize; ++i) {
			if (sg._party[i]._status == Data::STATUS_GOOD || sg._party[i]._status == Data::STATUS_POISONED) {
				_member = i;
				break;
			}
		}
	}

	_index = 0;
	while (_index < Data::SHOPS_PER_KIND && Data::SHOP_MAPS[_kind][_index] != sg._mapId)
		++_index;
	_index = MIN(_index, Data::SHOPS_PER_KIND - 1);

	_shopName = Data::SHOP_NAMES[_kind][_index];
	_keeperName = Data::SHOP_KEEPERS[_kind][_index];
	_code = 0;
	_price = _amount = 0;

	switch (_kind) {
	case KIND_GUILD:
		greet();
		_step = STEP_GUILD_OFFER;
		break;

	case KIND_PROVISIONER:
		greet();
		_step = STEP_PROVISIONER_OFFER;
		break;

	case KIND_STABLES:
		if (!findStall()) {
			_log.print("The stables are closed.\n");
			return true;
		}

		_price = discounted(Data::STABLE_PRICES[_index]);
		greet();
		_step = STEP_STABLES_OFFER;
		break;

	case KIND_HEALER:
		greet();
		_step = STEP_HEALER_OFFER;
		break;

	case KIND_WEAPONS:
		weaponsWelcome();
		break;

	case KIND_TAVERN:
		_style = Data::TAVERN_STYLES[_index];
		_bought = 0;
		_drinks = 0;
		greet();
		_step = STEP_TAVERN_OFFER;
		break;

	case KIND_SHIPWRIGHT:
		_shipFailed = false;
		_shipBought = false;
		greet();
		_step = STEP_SHIP_OFFER;
		break;

	case KIND_INN:
		_innCode = 0;
		greet();
		_step = STEP_INN_OFFER;
		break;

	default:
		return true;
	}

	return false;
}

bool ShopLogic::keypress(const Graphics::Views::KeypressMessage &msg) {
	const int key = upperKey(msg);
	const int raw = msg.ascii;

	switch (_step) {
	case STEP_ANY_KEY:
		_step = _afterKey;
		if (_step == STEP_PROVISIONER_MENU)
			provisionerMenu();
		else if (_step == STEP_WEAPONS_SOLD)
			weaponsAfterBuy();
		break;

	case STEP_WEAPONS_WELCOME:
		say("\n$ says,\n\"");
		_log.print(Data::WEAPON_GREETINGS[_G(gameRandom).range(0, 1)]);
		_log.print("\" ");
		_step = STEP_WEAPONS_CHOOSE;
		break;

	case STEP_WEAPONS_CHOOSE:
		weaponsChoose(key);
		break;

	case STEP_WEAPONS_MENU:
		if (key >= 'A' && key <= 'H')
			weaponsItem(key - 'A');
		else if (key == KEY_ESCAPE || key == KEY_SPACE)
			finish(_code);
		break;

	case STEP_WEAPONS_CONFIRM:
		if (key == 'Y' || key == 'N')
			weaponsAnswer(key);
		break;

	case STEP_WEAPONS_SELL:
		weaponsSellKey(key);
		break;

	case STEP_WEAPONS_SELL_CONFIRM:
		if (key == 'Y' || key == 'N')
			weaponsSellAnswer(key);
		break;

	case STEP_TAVERN_OFFER:
		tavernOffer(key);
		break;

	case STEP_TAVERN_ORDER:
		tavernOrder(key);
		break;

	case STEP_TAVERN_ENOUGH:
		if (key == 'Y') {
			_log.print("Yes\n\n");
			tavernMore();
		} else if (key == 'N') {
			_log.print("No!");
			_G(savegame)._drunkTurns = 0x19;
			if (_G(savegame)._karma > 0)
				--_G(savegame)._karma;
			tavernDrinkBought();
		}
		break;

	case STEP_TAVERN_WINE:
		tavernWine(key);
		break;

	case STEP_TAVERN_COUNT:
		if (editNumber(key, 2))
			tavernRationsCount();
		break;

	case STEP_TAVERN_LORE:
		if (editLine(raw ? raw : key, 15))
			tavernLoreEntered();
		break;

	case STEP_TAVERN_LORE_CONFIRM:
		if (key == 'Y' || key == 'N')
			tavernLoreAnswer(key);
		break;

	case STEP_TAVERN_AGAIN:
		if (key == 'Y' || key == 'N')
			tavernAgainAnswer(key);
		break;

	case STEP_SHIP_OFFER:
		shipOffer(key);
		break;

	case STEP_SHIP_MENU:
		shipKey(key);
		break;

	case STEP_SHIP_OWNED:
		if (key == 'Y' || key == 'N')
			shipOwned(key);
		break;

	case STEP_SHIP_FRIGATE:
		if (key == 'Y' || key == 'N')
			shipFrigate(key);
		break;

	case STEP_SHIP_SKIFF:
		if (key == 'Y' || key == 'N')
			shipSkiff(key);
		break;

	case STEP_SHIP_AGAIN:
		if (key == 'Y' || key == 'N')
			shipAgain(key);
		break;

	case STEP_INN_OFFER:
		innOffer(key == KEY_SPACE ? 'N' : key);
		break;

	case STEP_INN_CHOOSE:
		innChoose(key);
		break;

	case STEP_INN_REST_CONFIRM:
		if (key == 'Y' || key == 'N')
			innRestAnswer(key);
		break;

	case STEP_INN_LEAVE_CONFIRM:
		if (key == 'Y' || key == 'N')
			innLeaveAnswer(key);
		break;

	case STEP_INN_GUEST:
		innGuestKey(key);
		break;

	case STEP_GUILD_OFFER:
		if (key == 'Y') {
			_log.print("Yes\n\n\"We sell:\n\n");
			guildMenu();
		} else if (key == KEY_SPACE || key == 'N') {
			_log.print("No");
			finish(_code);
		}
		break;

	case STEP_GUILD_MENU:
		if (key >= 'A' && key <= 'C') {
			_log.printChar(key | 0x20);
			guildItem(key - 'A');
		} else if (key == KEY_ESCAPE || key == KEY_SPACE) {
			finish(_code);
		} else if (key == 'D') {
			sayAt(TEXT_GUILD_EXTRA);
			guildMenu();
		}
		break;

	case STEP_GUILD_CONFIRM:
		if (key == 'Y' || key == 'N')
			guildAnswer(key);
		break;

	case STEP_PROVISIONER_OFFER:
		if (key == 'Y') {
			_log.print("Yes\n\n\"Fine! We sell:\n\n");
			provisionerMenu();
		} else if (key == KEY_SPACE || key == 'N') {
			_log.print("No");
			finish(_code);
		}
		break;

	case STEP_PROVISIONER_MENU:
		if (key >= 'A' && key <= 'E')
			provisionerItem(key - 'A');
		else if (key == KEY_ENTER || key == KEY_ESCAPE || key == KEY_SPACE)
			finish(_code);
		break;

	case STEP_PROVISIONER_CONFIRM:
		if (key == 'Y' || key == 'N')
			provisionerAnswer(key);
		break;

	case STEP_STABLES_OFFER:
		if (key == 'Y') {
			_log.print("Yes\n\n\"");
			sayAt(TEXT_STABLES_PITCH);
			_log.print("\n\nDeal?\" ");
			_step = STEP_STABLES_DEAL;
		} else if (key == KEY_SPACE || key == 'N') {
			_log.print("No");
			finish(_code);
		}
		break;

	case STEP_STABLES_DEAL:
		if (key == 'Y' || key == 'N')
			stablesDeal(key);
		break;

	case STEP_HEALER_OFFER:
		healerOffer(key);
		break;

	case STEP_HEALER_NEED:
		healerNeed(key);
		break;

	case STEP_HEALER_PAY:
		if (key == 'Y' || key == 'N')
			healerPay(key);
		break;

	default:
		break;
	}

	return _step == STEP_DONE;
}

// ---- The guild

void ShopLogic::guildMenu() {
	_log.print("a.........Keys\n");
	_log.print("b.........Gems\n");
	_log.print("c......Torches\n\n");
	_log.print("Thy concern?\" ");
	_step = STEP_GUILD_MENU;
}

void ShopLogic::guildItem(int item) {
	_item = item;
	_price = discounted(Data::GUILD_PRICES[_index][item]);

	_log.print("\n\n\"");
	sayAt(Data::GUILD_TEXT[item]);
	_log.print("\n\nInterested?\" ");
	_step = STEP_GUILD_CONFIRM;
}

void ShopLogic::guildAnswer(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No\n\n\"What else, then?\n\n");
		guildMenu();
		return;
	}

	_log.print("Yes\n");

	if (sg._gold < _price) {
		sayAt(TEXT_SHORT_OF_GOLD_GUILD);
		finish(-1);
		return;
	}

	sg._gold -= _price;
	stealGold();

	switch (_item) {
	case 0:
		sg._keys = MIN(sg._keys + GUILD_KEYS_PER_BUY, ITEM_MAX);
		break;
	case 1:
		sg._gems = MIN(sg._gems + GUILD_GEMS_PER_BUY, ITEM_MAX);
		break;
	default:
		sg._torches = MIN(sg._torches + GUILD_TORCHES_PER_BUY, ITEM_MAX);
		break;
	}

	_code = 1;
	say("\n\"Sold!\"\nsays $.\n\n\"What else, \n");
	_log.print(isFemale() ? "m'lady" : "m'lord");
	_log.print("?\n\n");
	guildMenu();
}

// ---- The provisioner

void ShopLogic::provisionerMenu() {
	char letter = 'A';

	for (int i = 0; i < PROVISIONS; ++i) {
		if (Data::PROVISION_PRICES[_index][i] != 0) {
			_log.printChar(letter++);
			_log.print("...");
			_log.print(Data::REAGENT_NAMES[i]);
			_log.printChar('\n');
		}
	}

	_log.print("\nThy interest?\" ");
	_step = STEP_PROVISIONER_MENU;
}

void ShopLogic::provisionerItem(int choice) {
	Data::Savegame &sg = _G(savegame);

	// The choice is of the things sold, not of everything
	int item = 0;
	int counted = 0;
	for (; item < PROVISIONS; ++item) {
		if (Data::PROVISION_PRICES[_index][item] != 0) {
			if (counted == choice)
				break;
			++counted;
		}
	}

	if (item == PROVISIONS)
		return;

	_item = item;

	if (sg._reagents[item] == ITEM_MAX) {
		_log.print("\n\n\"Thou canst not carry any more!\"\n\n");
		waitKey(STEP_PROVISIONER_MENU);
		return;
	}

	_price = discounted(Data::PROVISION_PRICES[_index][item]);
	_amount = Data::PROVISION_AMOUNTS[_index][item];

	_log.print("\n\n\"");
	sayAt(Data::PROVISION_TEXT[item]);
	_log.print(" Is this thy need?\" ");

	if (_log.column() > 0xC)
		_log.print("\n:");

	_step = STEP_PROVISIONER_CONFIRM;
}

void ShopLogic::provisionerAnswer(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No\n\n\"What else?\n\n");
		provisionerMenu();
		return;
	}

	_log.print("Yes\n");

	if (sg._gold < _price) {
		sayAt(TEXT_SHORT_OF_GOLD_PROVISIONER);
		finish(-1);
		return;
	}

	sg._gold -= _price;
	stealGold();
	sg._reagents[_item] = MIN(sg._reagents[_item] + _amount, ITEM_MAX);
	_code = 1;

	say("\n\"I thank thee!\"\nsays $.\n");
	_log.print("\"Anything else?\n\n");
	provisionerMenu();
}

// ---- The stables

bool ShopLogic::findStall() {
	const Data::Savegame &sg = _G(savegame);

	for (_slot = 1; _slot < Data::ACTOR_COUNT; ++_slot) {
		if (sg.getActor(_slot)._tile == 0)
			break;
	}

	int side;
	for (side = 0; side < 4; ++side) {
		_horseX = STABLE_DX[side] + sg._mapX;
		_horseY = STABLE_DY[side] + sg._mapY;
		const byte tile = g_engine->getMapTile(_horseX, _horseY);

		bool occupied = false;
		for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
			const Data::ActorRecord actor = sg.getActor(i);
			if (actor._x == _horseX && actor._y == _horseY && actor._z == sg._mapLevel) {
				occupied = actor._tile != 0;
				break;
			}
		}

		if (!occupied && (tile == TILE_STABLE_A || tile == TILE_STABLE_B || tile == TILE_GRASS))
			break;
	}

	return side != 4 && _slot != Data::ACTOR_COUNT;
}

void ShopLogic::stablesDeal(int answer) {
	Data::Savegame &sg = _G(savegame);

	if (answer == 'N') {
		_log.print("No");
		finish(_code);
		return;
	}

	_log.print("Yes!");

	if (sg._gold < _price) {
		_log.print("\n\n\"Thou couldst not afford to ");
		say("feed it!\"\nyells $.\n");
		finish(-1);
		return;
	}

	sg._gold -= _price;
	stealGold();

	Data::ActorRecord horse;
	horse._tile = horse._animTile = ACTOR_HORSE;
	horse._x = _horseX;
	horse._y = _horseY;
	horse._z = sg._mapLevel;
	sg.setActor(_slot, horse);

	finish(1);
}

// ---- The healers

void ShopLogic::healerOffer(int key) {
	if (key == 'N') {
		_log.print("No");
		finish(1);
	} else if (key == 'Y') {
		_log.print("Yes\n\n");
		_log.print("\"We have powers to Cure, Heal, or Resurrect.\"\n");
		say("says $.\n\n\"What is the nature of thy need?\" ");
		_step = STEP_HEALER_NEED;
	}
}

void ShopLogic::healerNeed(int key) {
	switch (key) {
	case 'C':
		_log.print("Curing");
		_need = NEED_CURE;
		break;
	case 'H':
		_log.print("Healing");
		_need = NEED_HEAL;
		break;
	case 'R':
		_log.print("Resurrect");
		_need = NEED_RESURRECT;
		break;
	case KEY_ENTER:
	case KEY_SPACE:
		_log.print("Nothing");
		finish(1);
		return;
	default:
		return;
	}

	if (_G(savegame)._partySize == 1) {
		_target = 0;
		healerService();
	} else {
		_log.print("\n\n\"Who needs my aid?\" ");
		_step = STEP_HEALER_MEMBER;
	}
}

bool ShopLogic::memberChosen(int member) {
	if (_step == STEP_INN_LEAVE_MEMBER) {
		innLeaveMember(member);
		return _step == STEP_DONE;
	}

	if (member == NO_ONE) {
		_log.print("No one");
		healerAgain();
	} else {
		_target = member;
		healerService();
	}

	return _step == STEP_DONE;
}

void ShopLogic::healerAgain() {
	_log.print("\n\n\"Is there any other way in which I may\n");
	_log.print("aid thee?\" ");
	_step = STEP_HEALER_OFFER;
}

void ShopLogic::healerService() {
	Data::Savegame &sg = _G(savegame);
	const Data::Character &who = sg._party[_target];

	bool needed = false;
	switch (_need) {
	case NEED_CURE:
		needed = who._status == Data::STATUS_POISONED;
		break;
	case NEED_HEAL:
		needed = who._status != Data::STATUS_DEAD && who._hp != who._maxHp;
		break;
	case NEED_RESURRECT:
		needed = who._status == Data::STATUS_DEAD;
		break;
	}

	if (!needed) {
		say("\n\n\"Thou hast no need of this art!\"\nsays $.");
		healerAgain();
		return;
	}

	_log.print("\n\n\"");

	if (_need != NEED_RESURRECT && sg._mapId == FREE_MAP) {
		_log.print("Receive now the Light!\"");
		healerPay('F');
		return;
	}

	switch (_need) {
	case NEED_CURE:
		_log.print("I can cure thy poisoned body ");
		_price = Data::CURE_PRICES[_index];
		break;
	case NEED_HEAL:
		_log.print("I can heal thee ");
		_price = Data::HEAL_PRICES[_index];
		break;
	case NEED_RESURRECT:
		_log.print("I can raise this unfortunate person from ");
		_log.print("the dead ");
		_price = Data::RESURRECT_PRICES[_index];
		break;
	}

	say("for % gold.\n\nWilt thou\npay?\" ");
	_step = STEP_HEALER_PAY;
}

void ShopLogic::healerPay(int answer) {
	Data::Savegame &sg = _G(savegame);
	bool refused = false;

	// The light of the shrines is free
	if (answer != 'F') {
		_log.print(answer == 'Y' ? "Yes" : "No");
		refused = answer == 'N';

		if (!refused && sg._gold < _price && (_price > FREE_PRICE_LIMIT || sg._mapId != SOMETIMES_FREE_MAP)) {
			sayAt(TEXT_HEALER_CANT_PAY);
			refused = true;
		}

		if (!refused && sg._gold >= _price) {
			sg._gold -= _price;
			stealGold();
		}
	}

	if (!refused) {
		Data::Character &who = sg._party[_target];

		switch (_need) {
		case NEED_CURE:
			who._status = Data::STATUS_GOOD;
			break;

		case NEED_HEAL:
			who._hp = who._maxHp;
			break;

		case NEED_RESURRECT:
			InventoryLogic::resurrect(sg, _target);
			who._hp = who._maxHp;
			break;
		}
	}

	healerAgain();
}

} // namespace Ultima5
} // namespace Ultima
