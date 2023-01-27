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
	// TODO
	return nullptr;
}

String LogicStrings::fn311(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn317(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn318(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn433(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn444(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn452(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn463(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn468(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn472(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn477(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn479(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn480(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn481(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn485(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn491(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn492(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn493(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn494(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn495(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn505(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn506(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn509(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn510(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn511(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn512(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn517(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn557(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn558(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn591(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn603(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn606(LogicStringAction action) {
	// TODO
	return nullptr;
}

String LogicStrings::fn727(LogicStringAction action) {
	// TODO
	return nullptr;
}

} // namespace Logic
} // namespace Gateway
} // namespace Early
} // namespace Legend
