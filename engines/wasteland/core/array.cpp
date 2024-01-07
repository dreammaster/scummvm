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

#include "wasteland/core/array.h"

namespace Wasteland {

void StringArray::split(const Common::String &src, char separator) {
	clear();

	Common::String str = src;
	size_t idx;
	while ((idx = str.findFirstOf(separator)) != Common::String::npos) {
		push_back(Common::String(str.c_str(), idx));
		str = Common::String(str.c_str() + idx + 1);
	}

	push_back(str);
}

void StringArray::split(const Common::String &src, char separator, size_t maxLen) {
	Common::String str = src;
	size_t newLine, lineEnd;
	clear();

	while (!str.empty()) {
		// Find end of the current line
		newLine = str.findFirstOf('\n');
		lineEnd = (newLine == Common::String::npos) ? str.size() : newLine;

		if (lineEnd > maxLen) {
			// Move backwards from line end to find point to split line at
			while (lineEnd > 0 && !(lineEnd <= maxLen && Common::isSpace(str[lineEnd])))
				--lineEnd;
			assert(lineEnd > 0);
		}

		push_back(Common::String(str.c_str(), str.c_str() + lineEnd));

		if (lineEnd == str.size())
			// Nothing left in string
			str.clear();
		else
			// Remove already processed line
			str = Common::String(str.c_str() + lineEnd + 1);
	}
}

} // namespace Wasteland
