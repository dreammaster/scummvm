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

#include "ultima/ultima5/data/shop_data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

const uint16 SHOP_GREETINGS[SHOP_KINDS][4] = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0d6c, 0x0da6, 0x0dd9, 0x0e14,
	0x1491, 0x14e1, 0x1513, 0x1557,
	0x1697, 0x16d8, 0x1710, 0x174d,
	0x1a67, 0x1aaa, 0x1ae5, 0x1b25,
	0x1f62, 0x1f92, 0x1fd1, 0x2026,
	0x223c, 0x2273, 0x22ab, 0x22df,
	0x23e4, 0x241a, 0x245c, 0x2495
};

const uint16 SHOP_GOODBYES[SHOP_KINDS][4] = {
	0x0000, 0x0011, 0x0038, 0x0043,
	0x0e44, 0x0e69, 0x0e85, 0x0ea1,
	0x159a, 0x15b4, 0x15be, 0x15c9,
	0x1788, 0x179b, 0x17c7, 0x17d2,
	0x1b61, 0x1b73, 0x1b90, 0x1ba8,
	0x205f, 0x2078, 0x2086, 0x209b,
	0x231e, 0x233c, 0x235e, 0x2381,
	0x24f2, 0x250c, 0x2520, 0x2539
};

const uint16 SHOP_THANKS[SHOP_KINDS][4] = {
	0x0057, 0x0064, 0x0076, 0x0083,
	0x0ebc, 0x0ece, 0x0ee3, 0x0ef4,
	0x15ea, 0x160b, 0x161b, 0x1637,
	0x17e6, 0x1807, 0x181d, 0x1832,
	0x1bbf, 0x1bda, 0x1bf5, 0x1c0f,
	0x20bb, 0x20da, 0x20ee, 0x210e,
	0x231e, 0x233c, 0x235e, 0x2381,
	0x2569, 0x2584, 0x25a2, 0x25be
};

const uint16 GUILD_PRICES[4][4] = {
	0x00be, 0x00ff, 0x000c, 0x0118,
	0x00a0, 0x00c8, 0x000b, 0x00af,
	0x00b9, 0x00e1, 0x0019, 0x0109,
	0x00aa, 0x0096, 0x0014, 0x012c
};

const uint16 GUILD_TEXT[3] = {
	0x212d, 0x216c, 0x21b9
};

const byte PROVISION_PRICES[5][8] = {
	0x00, 0x14, 0x12, 0x0c, 0x00, 0x00, 0x0c, 0x0d,
	0x0c, 0x10, 0x10, 0x08, 0x14, 0x00, 0x00, 0x00,
	0x0e, 0x10, 0x00, 0x00, 0x1e, 0x12, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x08, 0x08, 0x0a, 0x0f,
	0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x1e, 0x28
};

const byte PROVISION_AMOUNTS[5][8] = {
	0x00, 0x0a, 0x06, 0x04, 0x00, 0x00, 0x01, 0x01,
	0x0c, 0x08, 0x08, 0x02, 0x04, 0x00, 0x00, 0x00,
	0x0e, 0x08, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x01, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x01
};

const uint16 PROVISION_TEXT[8] = {
	0x1c1c, 0x1c7c, 0x1cda, 0x1d3b, 0x1d8d, 0x1dee, 0x1e4e, 0x1ebd
};

const char *const REAGENT_NAMES[8] = {
	"Sulfur Ash",
	"Ginseng",
	"Garlic",
	"Spider Silk",
	"Blood Moss",
	"Black Pearl",
	"Nightshade",
	"Mandrake"
};

const uint16 STABLE_PRICES[4] = {
	0x0064, 0x0082, 0x00a0, 0x00be
};

const byte HEAL_PRICES[8] = {
	0x23, 0x28, 0x2d, 0x32, 0x37, 0x3c, 0x41, 0x46
};

const byte CURE_PRICES[8] = {
	0x14, 0x19, 0x1e, 0x23, 0x28, 0x0f, 0x0a, 0x01
};

const uint16 RESURRECT_PRICES[8] = {
	0x00c8, 0x00d7, 0x00e1, 0x00ed, 0x00f7, 0x00f9, 0x0106, 0x010e
};

const uint16 WEAPON_PRICES[48] = {
	0x000f, 0x0032, 0x0078, 0x0096, 0x0028, 0x0046, 0x0078, 0x07d0,
	0x0000, 0x0014, 0x0032, 0x0064, 0x0096, 0x012c, 0x02bc, 0x0000,
	0x0001, 0x000a, 0x0005, 0x0005, 0x000f, 0x0007, 0x0003, 0x0028,
	0x0032, 0x003c, 0x004b, 0x000a, 0x0096, 0x000f, 0x0046, 0x0055,
	0x0096, 0x00c8, 0x00fa, 0x0000, 0x0320, 0x00fa, 0x03e8, 0x0000,
	0x0000, 0x0000, 0x01c2, 0x01f4, 0x00c8, 0x0384, 0x00f0, 0x0000
};

const byte WEAPON_LISTS[9][8] = {
	0x10, 0x11, 0x1a, 0x1b, 0x1c, 0x1d, 0x24, 0xff,
	0x13, 0x18, 0x2e, 0x16, 0x03, 0x06, 0x19, 0xff,
	0x00, 0x09, 0x0a, 0x12, 0x15, 0x25, 0x26, 0xff,
	0x02, 0x04, 0x0b, 0x17, 0x1e, 0x18, 0x1f, 0xff,
	0x20, 0x21, 0x22, 0x02, 0x05, 0x0c, 0x0e, 0xff,
	0x01, 0x07, 0x0d, 0x0e, 0x1e, 0x25, 0x2b, 0xff,
	0x00, 0x0a, 0x10, 0x14, 0x17, 0x13, 0x2a, 0xff,
	0x07, 0x20, 0x24, 0x1b, 0x1f, 0x2c, 0x2d, 0xff,
	0x01, 0x0d, 0x1c, 0x1d, 0x22, 0x16, 0x19, 0xff
};

const int16 WEAPON_TEXT[47] = {
	0x0095, 0x00d0, 0x011f, 0x0152, 0x0192, 0x01cb, 0x0209, 0x024d,
	0x0000, 0x02b2, 0x030a, 0x0342, 0x0376, 0x03c6, 0x0410, 0x0000,
	0x0465, 0x04a1, 0x04de, 0x0521, 0x0560, 0x05ab, 0x05f6, 0x0638,
	0x066e, 0x06b0, 0x06ee, 0x0721, 0x0742, 0x077c, 0x07a4, 0x07e7,
	0x082f, 0x0872, 0x08bb, 0x0000, 0x090e, 0x0959, 0x09a0, 0x0000,
	0x0000, 0x0000, 0x09f7, 0x0a60, 0x0ac2, 0x0b19, 0x0b78
};

const char *const WEAPON_BUY_PROMPTS[4] = {
	"Wouldst thou buy one?",
	"Wilt thou take it?",
	"Wish ye it?",
	"May I get one for thee?"
};

const char *const WEAPON_CANT_PAY[4] = {
	"Can't pay?! Out with ye, orc-face!",
	"What be ye trying to pull? OUT!",
	"OUT, SLIME!",
	"BEAT IT!"
};

const char *const WEAPON_WHICH[4] = {
	"What may I show thee?",
	"Which wouldst thou like to see?",
	"What is thine interest?",
	"Which would ye see?"
};

const uint16 WEAPON_SELL_TEXT[8] = {
	0x0bc0, 0x0bff, 0x0c2e, 0x0c55, 0x0ca1, 0x0ce8, 0x0d12, 0x0d3d
};

const char *const WEAPON_SELL_NAMES[48] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Cloth suit",
	"Leather suit",
	"Ring Mail suit",
	"Scale suit",
	"Chain suit",
	"Plate suit",
	"Mystic suits",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Two-Handed Hammer",
	"Two-Handed Axe",
	"Two-Handed Sword",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Invisibility Ring",
	"Protection Ring",
	"Regeneration Ring",
	"Turning Amulet",
	"",
	""
};

const char *const WEAPON_SELL_PROMPTS[4] = {
	"Which item wouldst thou like to sell?",
	"What dost thou wish to sell?",
	"Show me what ye got...",
	"What dost thou have for me to buy?"
};

const char *const WEAPON_SELL_MORE[4] = {
	"What else can ye offer me?",
	"What else hath ye to sell?",
	"What else doth thou wish to sell?",
	"What other arms wilt thou sell?"
};

const char *const WEAPON_SELL_BYES[4] = {
	"Good-bye...",
	"Mayhap another time...",
	"Godspeed...",
	"Fare thee well..."
};

const char *const WEAPON_GREETINGS[2] = {
	"Hail, friend! Wouldst thou Buy or Sell?",
	"Greetings, traveller! Wish ye to Buy, or hast thou wares to Sell?"
};

const char *const WEAPON_GOOD[4] = {
	"Very good!\n",
	"Excellent!\n",
	"Fine, fine!\n",
	"But of course!\n"
};

const char *const WEAPON_STOCK[4] = {
	"We have:",
	"We stock:",
	"Thou canst buy:",
	"We've got:"
};

const char *const ITEM_NAMES[48] = {
	"Leather Helm",
	"Chain Coif",
	"Iron Helm",
	"Spiked Helm",
	"Small Shield",
	"Large Shield",
	"Spiked Shield",
	"Magic Shield",
	"Jewel Shield",
	"Cloth Armour",
	"Leather Armour",
	"Ring Mail",
	"Scale Mail",
	"Chain Mail",
	"Plate Mail",
	"Mystic Armour",
	"Dagger",
	"Sling",
	"Club",
	"Flaming Oil",
	"Main Gauche",
	"Spear",
	"Throwing Axe",
	"Short Sword",
	"Mace",
	"Morning Star",
	"Bow",
	"Arrows",
	"Crossbow",
	"Quarrels",
	"Long Sword",
	"2H Hammer",
	"2H Axe",
	"2H Sword",
	"Halberd",
	"Sword of Chaos",
	"Magic Bow",
	"Silver Sword",
	"Magic Axe",
	"Glass Sword",
	"Jeweled Sword",
	"Mystic Sword",
	"Ring of Invisibility",
	"Ring of Protection",
	"Ring of Regeneration",
	"Amulet/Turning",
	"Spiked Collar",
	"Ankh"
};

const char *const ITEM_SHORT_NAMES[56] = {
	"Leath Helm",
	"Chain Coif",
	"Iron Helm",
	"Spkd. Helm",
	"Sm. Shield",
	"Lg. Shield",
	"Spkd. Shld",
	"Mag. Shld",
	"Jewel Shld",
	"Cloth",
	"Leather",
	"Ring Mail",
	"Scale",
	"Chain",
	"Plate",
	"Myst. Armr",
	"Dagger",
	"Sling",
	"Club",
	"Flame Oil",
	"Main Gauch",
	"Spear",
	"Thrwng Axe",
	"Sht. Sword",
	"Mace",
	"Morn. Star",
	"Bow",
	"Arrows",
	"Crossbow",
	"Quarrels",
	"Long Sword",
	"2H Hammer",
	"2H Axe",
	"2H Sword",
	"Halberd",
	"Chaos Swrd",
	"Magic Bow",
	"Silver Swd",
	"Magic Axe",
	"Glass Swrd",
	"Jewel Swrd",
	"Myst. Swrd",
	"Inv. Ring",
	"Prot. Ring",
	"Regen Ring",
	"Am/Turning",
	"Sp. Collar",
	"Ankh",
	"Blue",
	"Yellow",
	"Red",
	"Green",
	"Orange",
	"Purple",
	"Black",
	"White"
};

const char *const TAVERN_FOOD_KEYS[1] = {
	"MCBF"
};

const char *const TAVERN_DRINK_KEYS[1] = {
	"AWRS"
};

const char *const TAVERN_FOOD_COUNT_KEYS[1] = {
	"R  P"
};

const char *const TAVERN_TALK_KEYS[1] = {
	"CTHA"
};

const uint16 TAVERN_MEAL_PRICES[9] = {
	0x0003, 0x0004, 0x0005, 0x0003, 0x0002, 0x0005, 0x0003, 0x0004,
	0x0005
};

const uint16 TAVERN_WINE_PRICES[6] = {
	0x0012, 0x00c0, 0x004f, 0x001e, 0x0113, 0x0062
};

const uint16 TAVERN_RATION_PRICES[9] = {
	0x000a, 0x000f, 0x0014, 0x0019, 0x001e, 0x0019, 0x0014, 0x0019,
	0x001e
};

const uint16 TAVERN_RATION_TEXT[7] = {
	0x1198, 0x11d7, 0x121f, 0x1271, 0x1299, 0x12e3, 0x130c
};

const char *const TAVERN_TOPICS[26] = {
	"hone",
	"comp",
	"valo",
	"just",
	"sacr",
	"hono",
	"spir",
	"humi",
	"dece",
	"desp",
	"dest",
	"wron",
	"cove",
	"sham",
	"hyth",
	"crow",
	"scep",
	"amul",
	"fals",
	"hatr",
	"cowa",
	"astr",
	"oppr",
	"brit",
	"resi",
	"unde"
};

const uint16 TAVERN_TOPIC_PRICES[26] = {
	0x0032, 0x004b, 0x0032, 0x0032, 0x004b, 0x004b, 0x0019, 0x0032,
	0x0064, 0x0096, 0x004b, 0x0096, 0x004b, 0x0064, 0x0064, 0x00c8,
	0x00c8, 0x00c8, 0x00fa, 0x00fa, 0x00fa, 0x0064, 0x0032, 0x0032,
	0x00c8, 0x0064
};

const char *const TAVERN_TOPIC_ITEMS[26] = {
	"Malik",
	"Greyson",
	"Trian",
	"Jeremy",
	"Rew",
	"Gruman",
	"Saul",
	"Shirita",
	"Malifora",
	"Annon",
	"Trian",
	"Felespar",
	"the mother of Rew",
	"Sindar",
	"Kaiko",
	"Terrance",
	"Greymarch",
	"Simon and Tessa",
	"Shalineth",
	"a daemon",
	"Lord Malone",
	"Zachariah",
	"Tactus",
	"a daemon",
	"Terrance",
	"Jotham"
};

const byte TAVERN_TOPIC_PLACES[26] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x01,
	0x03, 0x09, 0x0b, 0x0a, 0x0c, 0x00, 0x04, 0x0a,
	0x01, 0x08
};

const char *const TAVERN_PLACES[13] = {
	"Moonglow",
	"Britain",
	"Jhelom",
	"Yew",
	"Minoc",
	"Trinsic",
	"Skara Brae",
	"New Magincia",
	"a lighthouse south of Britain",
	"a hidden mountain keep",
	"the desert",
	"the Lycaeum",
	"Serpent's Hold"
};

const uint16 TAVERN_ANSWERS[4] = {
	0x13a2, 0x13ae, 0x13d9, 0x13f3
};

const byte TAVERN_STYLES[10] = {
	0x00, 0x00, 0x00, 0x02, 0x03, 0x01, 0x00, 0x02,
	0x00, 0x00
};

const uint16 TAVERN_GREETINGS[4] = {
	0x0f09, 0x0f5a, 0x0f9e, 0x0fdf
};

const uint16 TAVERN_AGAIN[4] = {
	0x1042, 0x109a, 0x10ea, 0x1146
};

const uint16 FRIGATE_PRICES[4] = {
	0x0258, 0x02f1, 0x028a, 0x02bc
};

const uint16 SKIFF_PRICES[4] = {
	0x00c8, 0x00af, 0x007d, 0x0064
};

const byte SHIP_X[4] = {
	0x27, 0x97, 0x4f, 0x8a
};

const byte SHIP_Y[4] = {
	0xdd, 0x15, 0x6d, 0x9f
};

const byte INN_PRICES[8] = {
	0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x0a, 0x0a
};

const byte INN_ROOMS[6] = {
	0x03, 0x04, 0x03, 0x02, 0x02, 0x02
};

const uint16 INN_TEXT[6] = {
	0x25d0, 0x2600, 0x2641, 0x2641, 0x2667, 0x26a7
};

const byte INN_BED_X[6] = {
	0x15, 0x0f, 0x19, 0x14, 0x1b, 0x07
};

const byte INN_BED_Y[6] = {
	0x0a, 0x07, 0x09, 0x01, 0x06, 0x1a
};

const byte SHOP_MAPS[SHOP_KINDS][SHOPS_PER_KIND] = {
	0x02, 0x03, 0x04, 0x05, 0x06, 0x11, 0x18, 0x1a, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x02, 0x03, 0x04, 0x08, 0x13, 0x16, 0x18, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x14, 0x16, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x05, 0x15, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x04, 0x07, 0x17, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x16, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x06, 0x07, 0x15, 0x17, 0x1e, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x03, 0x07, 0x14, 0x16, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const char *const SHOP_NAMES[SHOP_KINDS][SHOPS_PER_KIND] = {
	"Iolo's Bows",
	"Naughty Nomaan's",
	"Arms of Justice",
	"Darkwatch Armoury",
	"The Paladin's Protectorate!",
	"North Star Armoury",
	"Buccaneers Booty",
	"The Shattered Shield",
	"Siege Crafters",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"The Honest Meal",
	"The Wayfarer Tavern",
	"The Sword and Keg",
	"The Slaughtered Lamb",
	"The Humble Palate",
	"The Blue Boar Tavern",
	"The Cat's Lair",
	"The Fallen Virgin",
	"The Folley Tap",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Horse & Rider",
	"The Stablehouse",
	"Wishing Well Horses",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Island Shipwrights",
	"The Crow's Nest",
	"The Oaken Oar",
	"The Rusty Bucket",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"The Herbalist",
	"Healers Herbs",
	"The Alchemist",
	"Mysticism",
	"The Sharper Mage",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"The Den",
	"The Guild",
	"The Nemesis",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"The Healers Mission",
	"Wounds of Honour",
	"The Spirit Healers",
	"Healers' Sanctum",
	"Sanctuary",
	"The Shield of Truth",
	"The Empath",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"The Wayfarer Inn",
	"The Warrior's Stead",
	"The Haunting Inn",
	"Hotel Brittany",
	"The Smugglers' Inn",
	"The King's Ransom Inn",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};

const char *const SHOP_KEEPERS[SHOP_KINDS][SHOPS_PER_KIND] = {
	"Gwenneth",
	"Nomaan",
	"Ronan",
	"Shenstone",
	"Paul",
	"Max",
	"Kitiara",
	"Steve",
	"Thol",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Sam",
	"Tika",
	"Nicole",
	"Duclas",
	"Felicity",
	"Jaymes",
	"Dr. Cat",
	"Nikki",
	"Rob",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Hettar",
	"Theoan",
	"Ferru",
	"Simplon",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Bantral",
	"Captain Blyth",
	"Master Hawkins",
	"Jones",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Nilrem",
	"Madam Pendra",
	"Toama",
	"Enlor",
	"Virden",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Braunam",
	"Danfits",
	"Daem",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Regina",
	"Leila",
	"Temptious",
	"Milan",
	"Jessica",
	"Faye",
	"Jessip",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Donya",
	"Gremnor",
	"Rogi",
	"Terbor",
	"Lorien",
	"Ransack",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
