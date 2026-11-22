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

#include "ultima/ultima5/logic/endgame_logic.h"
#include "common/file.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int MAX_MESSAGE = 1000;
constexpr int START_YEAR = 139;
constexpr int START_MONTH = 4;
constexpr int START_DAY = 5;
constexpr int DAYS_PER_MONTH = 28;
constexpr int MONTHS_PER_YEAR = 13;

static const char *const ONES[19] = {
	"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve",
	"Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"
};

static const char *const TENS[8] = { "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };

static const char *const ORDINALS[12] = {
	"First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eighth", "Ninth", "Tenth", "Eleventh", "Twelfth"
};

// A number said in words
static Common::String cardinal(int n) {
	if (n <= 0)
		return Common::String();

	if (n < 20)
		return ONES[n - 1];

	Common::String result = TENS[n / 10 - 2];

	if (n % 10 != 0)
		result += Common::String("-") + ONES[n % 10 - 1];

	return result;
}

// A number said as a place in order
static Common::String ordinal(int n) {
	if (n < 13)
		return ORDINALS[n - 1];

	if (n < 20)
		return cardinal(n) + "th";

	if (n == 20)
		return "Twentieth";

	return Common::String("Twenty-") + ORDINALS[n - 21];
}

Common::String EndgameLogic::message(int offset) {
	Common::String result;
	Common::File f;

	if (!f.open("endmsg.dat") || !f.seek(offset))
		return result;

	for (int i = 0; i < MAX_MESSAGE && !f.eos(); ++i) {
		const byte ch = f.readByte();
		if (ch == 0)
			break;

		result += (char)ch;
	}

	return result;
}

int EndgameLogic::memberTile(char cls) {
	switch (cls) {
	case 'M':
		return 0x40;
	case 'B':
		return 0x44;
	case 'F':
		return 0x48;
	default:
		return 0x4C;
	}
}

Common::Array<EndgameLogic::Line> EndgameLogic::certificate() {
	const Data::Savegame &sg = _G(savegame);
	Common::Array<Line> lines;

	auto add = [&](const Common::String &text, bool runes = false, bool inverse = false) {
		Line line;
		line._text = text;
		line._runes = runes;
		line._inverse = inverse;
		lines.push_back(line);
	};

	add("Be it known that on", false, true);
	add("the " + ordinal(sg._day) + " Day of", false, true);
	add("the " + ordinal(sg._month) + " Month", false, true);
	add("of the Year", false, true);
	add(cardinal(sg._year / 100) + " Hundred", false, true);
	add(cardinal(sg._year % 100), false, true);
	add("", false, true);
	add(Common::String(sg._party[0]._name) + " the Avatar", false, true);
	add("", false, true);
	add("saved the life", false, true);
	add("of our sovereign", false, true);
	add("Lord British, thereby", false, true);
	add("saving our people", false, true);
	add("and our land.", false, true);
	add("", false, true);
	add("[E@QUE_@OF@[E@AVATAR", true, true);
	add("IS@FOREVER", true, true);
	add("", false, true);
	add("", false, true);
	add("", false, true);
	add("Report now, thy Quest compleat in");

	// How long it took
	int years = sg._year - START_YEAR;
	int months = sg._month - START_MONTH;
	int days = sg._day - START_DAY;

	if (days < 0) {
		days += DAYS_PER_MONTH;
		--months;
	}

	if (months < 0) {
		months += MONTHS_PER_YEAR;
		--years;
	}

	Common::String took;
	if (years != 0) {
		took += Common::String::format("%d year%s", years, years > 1 ? "s" : "");
		if (months != 0 || days != 0)
			took += ", ";
	}

	if (months != 0) {
		took += Common::String::format("%d month%s", months, months > 1 ? "s" : "");
		if (days != 0)
			took += ", ";
	}

	if (days != 0)
		took += Common::String::format("%d day%s", days, days > 1 ? "s" : "");

	add(took);
	add("to Lord British at Origin Systems!");
	return lines;
}

} // namespace Ultima5
} // namespace Ultima
