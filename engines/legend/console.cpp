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

namespace Legend {

Console::Console() : GUI::Debugger() {
	registerCmd("string",   WRAP_METHOD(Console, cmdString));
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

} // End of namespace Legend
