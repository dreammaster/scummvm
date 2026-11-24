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

#include "ultima/ultima5/logic/death_logic.h"
#include "common/file.h"
#include "ultima/ultima5/logic/inventory_logic.h"
#include "ultima/ultima5/logic/location_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int KARMA_TEXT_COUNT = 5;
constexpr int KARMA_PER_TEXT = 20;
constexpr int MAX_TEXT = 2000;
constexpr int CASTLE_MAP = 0x11;
constexpr int CASTLE_LEVEL = 1;
constexpr int CASTLE_X = 10;
constexpr int CASTLE_Y = 10;
constexpr int REVIVE_MINUTES = 9;
constexpr int REVIVE_HOUR = 6;
constexpr int MINIMUM_KARMA = 0x4B;
constexpr int MINIMUM_FOOD = 0x3F;

static const uint16 KARMA_OFFSETS[KARMA_TEXT_COUNT] = { 0, 0x84, 0x10D, 0x19A, 0x222 };

static Data::Savegame &save() {
	return _G(savegame);
}

// What Lord British says, which depends on how virtuous the party has been
static Common::String karmaText() {
	Common::String result;
	Common::File f;

	if (!f.open("karma.dat"))
		return result;

	const int which = MIN<int>(save()._karma / KARMA_PER_TEXT, KARMA_TEXT_COUNT - 1);
	if (!f.seek(KARMA_OFFSETS[which]))
		return result;

	for (int i = 0; i < MAX_TEXT && !f.eos(); ++i) {
		const byte ch = f.readByte();
		if (ch == 0)
			break;

		result += (char)ch;
	}

	return result;
}

bool DeathLogic::partyDead() {
	const Data::Savegame &sg = save();

	for (int i = 0; i < sg._partySize; ++i) {
		const byte status = sg._party[i]._status;

		if (status == Data::STATUS_GOOD || status == Data::STATUS_POISONED || status == Data::STATUS_SLEEP)
			return false;
	}

	return sg._partySize != 0;
}

void DeathLogic::begin() {
	_log.print("\nAn unending darkness engulfs thee...");
	_log.print("\n\nThou hast found refuge.");
	_log.print("\n\nNo evil lives here, only peace and darkness.");
	_log.print("\n\nBut thy slumber is disturbed!");
	_log.print("\n\nSomeone shouts\n\n\"FORTIS FORTUNA\nAVENTARI\"");
	_log.print("\n\nThere is a peal of thunder!\n");
	g_engine->shake();
	_log.print("\n\"");
	_log.print(karmaText());
	_log.print("\"");
}

void DeathLogic::finish() {
	Data::Savegame &sg = save();
	const int from = sg._mapId;

	_log.print("\n\nStrange words are intoned.");

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_DEAD)
			InventoryLogic::resurrect(sg, i);

		sg._party[i]._hp = sg._party[i]._maxHp;
	}

	_log.print("\n\nVertigo...\n");

	if (sg._karma < MINIMUM_KARMA)
		sg._karma = MINIMUM_KARMA;

	// The party wakes in the castle of Lord British, after the night is over
	if (from == 0)
		sg.storeWorldObjects(sg._mapLevel != 0);
	sg.clearObjects();

	sg._mapId = CASTLE_MAP;
	sg._mapLevel = CASTLE_LEVEL;
	sg._mapX = sg._mapY = CASTLE_X;
	sg._partyTile = TILE_AVATAR;
	sg._icon = sg._protection = 0;
	sg._doorOpen = 0;

	while (sg._hour != REVIVE_HOUR)
		TimeLogic::advance(sg, REVIVE_MINUTES);

	sg._unk0300 = sg._unk0301 = 0;

	if (sg._food == 0)
		sg._food = MINIMUM_FOOD;

	LocationLogic::arrive();
}

} // namespace Ultima5
} // namespace Ultima
