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

#ifndef NUVIE_SCRIPT_COMMON_LANG_H
#define NUVIE_SCRIPT_COMMON_LANG_H

#include "common/str.h"

namespace Ultima {
namespace Ultima6 {

class Configuration;

namespace Scripts {

struct LangString {
	const char *_code;
	const char *_text;
};

struct LangTable {
	const char *_language;
	const LangString *_strings;
	uint _count;
};

/**
 * Looks up script text by code. Strings missing from the selected language
 * fall back to English, then to the code itself.
 */
class Lang {
private:
	const LangString *_english = nullptr;
	uint _englishCount = 0;
	const LangString *_selected = nullptr;
	uint _selectedCount = 0;

	static const char *find(const LangString *strings, uint count, const char *code);

public:
	/** The first table must be English */
	void init(const Configuration *config, const LangTable *tables, uint count);

	const char *get(const char *code) const;
};

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
