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

#include "common/util.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr char ICON_TIME_STOPPED = 'T';
constexpr char ICON_QUICKNESS = 'Q';
constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY = 24;
constexpr int DAYS_PER_MONTH = 28;
constexpr int MONTHS_PER_YEAR = 13;
constexpr int PARTY_COUNTER_MAX = 0x19;
constexpr int PLACE_COUNT = 8;
constexpr byte FIXED_PLACE_FLAG = 0x80;

// The light at the start and end of the day, in steps of 10 minutes
static const byte DAWN_LIGHT[] = { 0x02, 0x05, 0x0A, 0x14, 0x22, 0x31 };

constexpr int LIGHT_MAX = 0x32;
constexpr int LIGHT_SKIP_THRESHOLD = 0x33;
constexpr int LIGHT_NIGHT = 2;
constexpr int FIRST_DAYLIGHT_HOUR = 5;
constexpr int LAST_DAYLIGHT_HOUR = 19;
constexpr int LIGHT_FROM_TORCH = 0x12;
constexpr int LIGHT_FROM_SPELL = 10;
constexpr byte SHRINE_MAP = 0x19;

static void decreaseByte(byte &value, int amount) {
	value = value > amount ? value - amount : 0;
}

void TimeLogic::advance(Data::Savegame &sg, int minutes) {
	if (minutes != 0) {
		if (sg._icon == ICON_QUICKNESS) {
			minutes >>= 1;
			if (minutes == 0)
				minutes = 1;
		}

		sg._hourCopy = sg._hour;
		if (sg._icon != ICON_TIME_STOPPED) {
			sg._minute += minutes;
			decreaseByte(sg._unk0301, minutes);
			decreaseByte(sg._unk0300, minutes);
		}

		if (sg._minute >= MINUTES_PER_HOUR) {
			sg._minute -= MINUTES_PER_HOUR;
			decreaseByte(sg._unk02E6, 1);
			++sg._hour;

			if (sg._hour >= HOURS_PER_DAY) {
				sg._hour = 0;

				// Each day, the three places that change are picked again, unless they are fixed
				for (int i = 0; i < 3; ++i) {
					if (sg._unk0322[i] >= FIXED_PLACE_FLAG)
						continue;

					byte place;
					do {
						place = 1 + g_engine->getRandomNumber(PLACE_COUNT - 1);
						if (sg._mapId == place)
							place = 0;
						for (int j = 0; j < 3; ++j) {
							if (sg._unk0322[j] == place)
								place = 0;
						}
					} while (place == 0);

					sg._unk0322[i] = place;
				}

				++sg._day;
				if (sg._day > DAYS_PER_MONTH) {
					memset(sg._harvestDays, 0, sizeof(sg._harvestDays));
					sg._skullKeysDay = 0;
					sg._day = 1;
					sg._unk03B3 = 0;

					for (int i = 0; i < Data::PARTY_MAX; ++i) {
						if (sg._party[i]._unk17 < PARTY_COUNTER_MAX)
							++sg._party[i]._unk17;
					}

					++sg._month;
					if (sg._month > MONTHS_PER_YEAR) {
						sg._month = 1;
						++sg._year;
					}
				}
			}
		}
	}

	updateLight(sg);

	// The hour on a 12 hour clock
	if (sg._hour != sg._hourCopy)
		sg._unk02DE = sg._hour == 0 ? 12 : (sg._hour > 12 ? sg._hour - 12 : sg._hour);
}

void TimeLogic::updateLight(Data::Savegame &sg) {
	if (sg._light >= LIGHT_SKIP_THRESHOLD)
		return;

	if (sg._mapId == SHRINE_MAP || sg._mapLevel >= Data::MAP_LEVEL_UNDERGROUND || sg._hour < FIRST_DAYLIGHT_HOUR ||
			sg._hour > LAST_DAYLIGHT_HOUR) {
		sg._light = LIGHT_NIGHT;
	} else if (sg._hour == FIRST_DAYLIGHT_HOUR) {
		sg._light = DAWN_LIGHT[sg._minute / 10];
	} else if (sg._hour == LAST_DAYLIGHT_HOUR) {
		sg._light = DAWN_LIGHT[(MINUTES_PER_HOUR - 1 - sg._minute) / 10];
	} else {
		sg._light = LIGHT_MAX;
	}

	if (sg._unk0300 != 0 && sg._light < LIGHT_FROM_TORCH)
		sg._light = LIGHT_FROM_TORCH;
	if (sg._unk0301 != 0 && sg._light < LIGHT_FROM_SPELL)
		sg._light = LIGHT_FROM_SPELL;
}

} // namespace Ultima5
} // namespace Ultima
