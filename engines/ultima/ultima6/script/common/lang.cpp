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

#include "ultima/ultima6/conf/configuration.h"
#include "ultima/ultima6/misc/u6_misc.h"
#include "ultima/ultima6/script/common/lang.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

const char *Lang::find(const LangString *strings, uint count, const char *code) {
	for (uint i = 0; i < count; ++i) {
		if (!strcmp(strings[i]._code, code))
			return strings[i]._text;
	}

	return nullptr;
}

void Lang::init(const Configuration *config, const LangTable *tables, uint count) {
	_english = tables[0]._strings;
	_englishCount = tables[0]._count;
	_selected = _english;
	_selectedCount = _englishCount;

	Common::String language;
	config->value(config_get_game_key(config) + "/language", language, "en");

	for (uint i = 1; i < count; ++i) {
		if (language.equalsIgnoreCase(tables[i]._language)) {
			_selected = tables[i]._strings;
			_selectedCount = tables[i]._count;
			break;
		}
	}
}

const char *Lang::get(const char *code) const {
	const char *str = find(_selected, _selectedCount, code);
	if (!str)
		str = find(_english, _englishCount, code);

	return str ? str : code;
}

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
