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

#include "ultima/ultima6/script/se/se_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

void SEScripts::initLang() {
	static const LangString ENGLISH[] = {
		{ "PASS", "Pass!\n" },
		{ "YOU_SEE", "you see %s" },
		{ "NOTHING", "nothing!\n" },
		{ "SEARCHING_HERE_YOU_FIND", "Searching here, you find %s" },
		{ "SEARCHING_HERE_YOU_FIND_NOTHING", "Searching here, you find nothing.\n" },
		{ "SEARCH_NEXT_OBJ", ", %s" },
		{ "SEARCH_LAST_OBJ", " and %s" },
		{ "OUT_OF_RANGE", "Out of Range!\n" },
		{ "CARRY_TOO_MUCH", "You are carrying too much already.\n" },
		{ "GOT_CORN", "You got some corn from the plant.\n" },
		{ "USE_CLAY", "You shaped a small, soft pot from the clay.\n" },
		{ "USE_FISHING_POLE_NO_WATER", "You need to stand next to deep water.\n" },
		{ "USE_FISHING_POLE_FAIL", "You didn't get a fish.\n" },
		{ "USE_FISHING_POLE_SUCCESS", "You caught a fish!\n" },
		{ "USE_DIGGING_STICK_NO_WATER", "There is nothing to dig!\n" },
		{ "USE_DIGGING_STICK", "You get some clay.\n" },
		{ "USE_YUCCA_PLANT", "You got some flax from the yucca plant.\n" },
		{ "USE_TREE", "You pulled a branch from the tree.\n" }
	};

	static const LangTable TABLES[] = {
		{ "en", ENGLISH, ARRAYSIZE(ENGLISH) }
	};

	_lang.init(_config, TABLES, ARRAYSIZE(TABLES));
}

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
