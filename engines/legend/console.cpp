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

#include "legend/console.h"
#include "legend/utils/string.h"
#include "legend/early/engine.h"

namespace Legend {

Console::Console() : GUI::Debugger() {
	registerCmd("string",   WRAP_METHOD(Console, cmdString));
	registerCmd("vocab", WRAP_METHOD(Console, cmdVocab));
}

Console::~Console() {
}

bool Console::cmdString(int argc, const char **argv) {
	if (argc == 2) {
		uint val = strtol(argv[1], nullptr, 16);
		if (val <= 0xffff)
			val += 0xf0000000;

		String msg(val);
		debugPrintf("%s\n", msg.c_str());

	} else {
		debugPrintf("string <hexadecimal>\n");
	}

	return true;

	debugPrintf("Test\n");
	return true;
}

bool Console::cmdVocab(int argc, const char **argv) {
	if (Legend::g_engine->isLater()) {
		debugPrintf("Not supported for games\n");

	} else if (argc != 2) {
		debugPrintf("vocab <id>\n");

	} else {
		int vocabId = atoi(argv[1]);

		if (vocabId < 1 || vocabId >= (int)Early::g_engine->_vocab->size()) {
			debugPrintf("Invalid vocab Id\n");

		} else {
			debugPrintf("%s\n", (*Early::g_engine->_vocab)[vocabId].c_str());
		}
	}

	return true;
}

} // namespace Legend
