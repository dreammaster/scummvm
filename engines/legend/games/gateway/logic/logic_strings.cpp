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

#include "legend/games/gateway/logic/logic_strings.h"
#include "legend/games/gateway/engine.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Logic {

LogicStrings::LogicStrings() {
	(*this)[36] = fn36;
	(*this)[41] = fn41;
	(*this)[43] = fn43;
	(*this)[44] = fn44;
	(*this)[115] = fn115;
	(*this)[162] = fn162;
	(*this)[171] = fn171;
	(*this)[190] = fn190;
	(*this)[222] = fn222;
	(*this)[224] = fn224;
	(*this)[301] = fn301;
	(*this)[310] = fn310;
	(*this)[311] = fn311;
	(*this)[317] = fn317;
	(*this)[318] = fn318;
	(*this)[433] = fn433;
	(*this)[444] = fn444;
	(*this)[452] = fn452;
	(*this)[463] = fn463;
	(*this)[468] = fn468;
	(*this)[472] = fn472;
	(*this)[477] = fn477;
	(*this)[479] = fn479;
	(*this)[480] = fn480;
	(*this)[481] = fn481;
	(*this)[485] = fn485;
	(*this)[491] = fn491;
	(*this)[492] = fn492;
	(*this)[493] = fn493;
	(*this)[494] = fn494;
	(*this)[495] = fn495;
	(*this)[505] = fn505;
	(*this)[506] = fn506;
	(*this)[509] = fn509;
	(*this)[510] = fn510;
	(*this)[511] = fn511;
	(*this)[512] = fn512;
	(*this)[517] = fn517;
	(*this)[557] = fn557;
	(*this)[558] = fn558;
	(*this)[591] = fn591;
	(*this)[603] = fn603;
	(*this)[606] = fn606;
	(*this)[727] = fn727;
}

String LogicStrings::fn36(LogicStringAction action)  {
	if (action >= LSA_16 && action <= LSA_17) {
		return g_engine->_val95 ? "Demon Chamber" : "Empty Chamber";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn41(LogicStringAction action)  {
	if (action == LSA_17) {
		return g_engine->_logics->getBit(41, 9) ? "shiny sword" : "rusty sword";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn43(LogicStringAction action)  {
	if (action == LSA_18 && g_engine->_val9)
		return String(0x40bb);
	return nullptr;
}

String LogicStrings::fn44(LogicStringAction action)  {
	if (action == LSA_16) {
		return g_engine->_val24 ? "magic ring" : "ring";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn115(LogicStringAction action) {
	if (action == LSA_17) {
		return g_engine->_roomLogicNum == 73 ? "slots" : "slot";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn162(LogicStringAction action) {
	switch (action) {
	case LSA_16:
		return String(g_engine->_val177 ? 0x4c3f : 0x4c80);
	case LSA_17:
		return g_engine->_val177 ? "crystal" : "shard";
	case LSA_19:
		if (!g_engine->_val177)
			return String(0x4c3e);
		break;
	default:
		break;
	}

	return nullptr;
}

String LogicStrings::fn171(LogicStringAction action) {
	String line;

	switch (action) {
	case LSA_16:
	case LSA_17:
		return g_engine->_roomLogicNum == 143 ? "berry bushes" : "bushes";
	case LSA_18:
	case LSA_19:
		line = String(0x4848);
		if (g_engine->_roomLogicNum == 143)
			line += String(0x4849);
		return line;
	default:
		break;
	}

	return nullptr;
}

String LogicStrings::fn190(LogicStringAction action) {
	switch (action) {
	case LSA_16:
		if (g_engine->_val177)
			return "crystal garden";
		return String(0x4842);
	case LSA_17:
		return g_engine->_val177 ? "crystal garden" : "outcropping";
	case LSA_18:
	case LSA_19:
		return g_engine->_val177 ?
			"There is a small garden of crystals growing" :
			"There is a mud - spattered outcropping";
	default:
		break;
	}

	return nullptr;
}

String LogicStrings::fn222(LogicStringAction action) {
	if (action >= LSA_16 && action <= LSA_17) {
		return g_engine->_logics->getBit(g_engine->_roomLogicNum, 39) ?
			"sky" : "ceiling";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn224(LogicStringAction action) {
	if (action >= LSA_16 && action <= LSA_17) {
		return g_engine->_logics->getBit(g_engine->_roomLogicNum, 39) ?
			"ground" : "floor";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn301(LogicStringAction action) {
	if (action >= LSA_16 && action <= LSA_17) {
		if (g_engine->_val226)
			return "hologram";
		if (g_engine->_logics->getBit(301, 9))
			return "metal disk";
		return "medallion";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn310(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		if (g_engine->_logics->isPrehandler1(211, 310)) {
			g_engine->_logics->setBit(310, 4);
			g_engine->_logics->clearBit(310, 5);
			return action == LSA_17 ? "my seat" : "your seat";

		} else {
			g_engine->_logics->clearBit(310, 4);
			g_engine->_logics->setBit(310, 5);
			return "empty seat";
		}
	}

	return nullptr;
}

String LogicStrings::fn311(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		if (g_engine->_logics->isPrehandler1(256, 311))
			return "Thom's seat";
		if (g_engine->_logics->isPrehandler1(267, 311))
			return "Nubar's seat";
		if (g_engine->_logics->isPrehandler1(266, 311) ||
			g_engine->_logics->isPrehandler1(309, 311))
			return "Terri's seat";

		return "other seat";
	}

	return nullptr;
}

String LogicStrings::fn317(LogicStringAction action) {
	if (action == LSA_18 && g_engine->_logics->isPrehandler1(317, 316))
		return String(0x183c);

	return nullptr;
}

String LogicStrings::fn318(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		if (g_engine->_logics->isPrehandler1(318, 256))
			return "Thom's drink";
		if (g_engine->_logics->isPrehandler1(318, 267))
			return "Nubar's drink";
		if (g_engine->_logics->isPrehandler1(318, 266))
			return "Terri's drink";

		return "other drink";
	}

	return nullptr;
}

String LogicStrings::fn433(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		return g_engine->_roomLogicNum == 433 ? "Ship Cabin" : "ship";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn444(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		return g_engine->_val157 ? "Abandoned Mine" : "Deep Shaft";
	} else {
		return nullptr;
	}
}

String LogicStrings::fn452(LogicStringAction action) {
	static const char *const TREES[3] = { "distant tree", "huge tree", "Tree House" };

	if (action == LSA_16 || action == LSA_17) {
		int idx;
		switch (g_engine->_roomLogicNum) {
		case 445:
		case 449:
			idx = 0;
			break;
		case 446:
		case 448:
		case 450:
		case 453:
			idx = 1;
			break;
		default:
			idx = 2;
			break;
		}

		return TREES[idx];
	}

	return nullptr;
}

String LogicStrings::fn463(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(463);
	return nullptr;
}

String LogicStrings::fn468(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		if (g_engine->_val142)
			return "metal cane";
		if (g_engine->_val159)
			return "metal strut";
		return "metallic glint";
	}

	return nullptr;
}

String LogicStrings::fn472(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(472);
	return nullptr;
}

String LogicStrings::fn477(LogicStringAction action) {
	int idx;
	for (idx = 0; idx < 3 && g_engine->_val150[idx][0] != g_engine->_roomLogicNum; ++idx) {
	}

	assert(idx < 3);
	const byte *row = &g_engine->_val150[idx][0];

	switch (action) {
	case LSA_16:
	case LSA_17:
		return row[2] ? "cairn" : "pile of rocks";
	case LSA_18:
	case LSA_19:
		return String::format("There is a %s on the ground.",
			row[2] ? "cairn" : "pile of rocks");
	default:
		break;
	}

	return nullptr;
}

String LogicStrings::fn479(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(479);
	return nullptr;
}

String LogicStrings::fn480(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		String line = getThingName(479);
		line += " egg";
		return line;
	}

	return nullptr;
}

String LogicStrings::fn481(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(481);
	return nullptr;
}

String LogicStrings::fn485(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(485);
	return nullptr;
}

String LogicStrings::fn491(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return g_engine->_roomLogicNum == 448 ? "piles of dirt" : "piles of rocks";
	return nullptr;
}

String LogicStrings::fn492(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(492);
	return nullptr;
}

String LogicStrings::fn493(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		String line = "dead ";
		line += getThingName(492);
		return line;
	}

	return nullptr;
}

String LogicStrings::fn494(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return g_engine->_logics->isPrehandler1(494, 489) ? "actuator cell" : "robbed cell";
	return nullptr;
}

String LogicStrings::fn495(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(495);
	return nullptr;
}

String LogicStrings::fn505(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(505);
	return nullptr;
}

String LogicStrings::fn506(LogicStringAction action) {
	switch (action) {
	case LSA_16: {
		String line = getThingName(505);
		line += " leaves";
		return line;
	}
	case LSA_17:
		return "red leaves";
	default:
		break;
	}

	return nullptr;
}

String LogicStrings::fn509(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(509);
	return nullptr;
}

String LogicStrings::fn510(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		String line = getThingName(509);
		line += " limb";
		return line;
	}

	return nullptr;
}

String LogicStrings::fn511(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17) {
		String line = getThingName(509);
		line += " seeds";
		return line;
	}

	return nullptr;
}

String LogicStrings::fn512(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return g_engine->_logics->isPrehandler1(512, 459) ?
			"piece of glass" : "focal lens";

	return nullptr;
}

String LogicStrings::fn517(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return g_engine->_logics->isPrehandler1(517, 510) ?
			"long rope" : "coil of rope";

	return nullptr;
}

String LogicStrings::fn557(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(557);
	return nullptr;
}

String LogicStrings::fn558(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return getThingName(558);
	return nullptr;
}

String LogicStrings::fn591(LogicStringAction action) {
	if (action == LSA_18 || action == LSA_19) {
		String line(0x943c);
		line += g_engine->_logics->printObjLower(g_engine->_roomLogicNum);
		line += '.';
		return line;
	}

	return nullptr;
}

String LogicStrings::fn603(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return g_engine->_logics->getBit(603, 21) ? "spider" : "dead spider";
	return nullptr;
}

String LogicStrings::fn606(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return g_engine->_logics->getBit(606, 21) ? "tentacle" : "octopus";
	return nullptr;
}

String LogicStrings::fn727(LogicStringAction action) {
	if (action == LSA_16 || action == LSA_17)
		return g_engine->_val37 > 2 ? "chasm" : "crack";
	return nullptr;
}

String LogicStrings::getThingName(int logicNum) {
	struct Thing {
		uint16 _id;
		const char *_str1;
		const char *_str2;
		const char *_str3;
	};
	static const Thing THINGS[11] = {
		{ 479, "weird bird", "pterodactyl", "gyranthymus" },
		{ 472, "cactus", "saguaro cactus", "geraldo cactus" },
		{ 463, "strange bush", "creosote bush", "mendobrillium" },
		{ 481, "dinosaur", "Tyrannosaurus", "gopheria" },
		{ 505, "plant", "palm", "jubifruitus" },
		{ 509, "elm", "elm", "winnerpeller" },
		{ 492, "animal", "horse", "canterlope" },
		{ 558, "horned insect", "scorpion", "phylangomorph" },
		{ 557, "small rodent", "prairie dog", "rodenta" },
		{ 495, "ore", "purple ore", "vermaculite" },
		{ 485, "parrot", "singing parrot", "parroo" }
	};

	for (int idx = 0; idx < 11; ++idx) {
		if (THINGS[idx]._id == logicNum) {
			if (!g_engine->_logics->getBit(logicNum, 9))
				return THINGS[idx]._str1;
			if (!g_engine->_val152[idx])
				return THINGS[idx]._str2;
			return THINGS[idx]._str3;
		}
	}

	return "unknown thing";
}

} // namespace Logic
} // namespace Gateway
} // namespace Early
} // namespace Legend
