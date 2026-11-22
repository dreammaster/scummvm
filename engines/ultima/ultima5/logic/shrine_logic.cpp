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

#include "ultima/ultima5/logic/shrine_logic.h"
#include "common/file.h"
#include "ultima/ultima5/data/dungeon_data.h"
#include "ultima/ultima5/data/shrine_data.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int MESSAGES_START = 0x3AB;
constexpr int MAX_MESSAGE = 2000;
constexpr int KARMA_LIMIT = 99;
constexpr int STAT_LIMIT = 0x1E;
constexpr int GOLD_PER_HUNDRED = 100;
constexpr int REWARD_KARMA = 3;
constexpr int LAST_SHRINE = 7;
constexpr int DEFAULT_SHRINE = 6;
constexpr byte LEFT_BEHIND_MAP = 0x7F;
constexpr byte SEALED_TILE = 0xDF;
constexpr byte RUINS_TILE = 0x1A;
constexpr byte SHRINE_TILE = 0x19;

static Data::Savegame &save() {
	return _G(savegame);
}

Common::String ShrineLogic::message(int offset) {
	Common::String result;
	Common::File f;

	if (!f.open("miscmsg.dat") || !f.seek(MESSAGES_START + offset))
		return result;

	for (int i = 0; i < MAX_MESSAGE && !f.eos(); ++i) {
		const byte ch = f.readByte();
		if (ch == 0)
			break;

		result += (char)ch;
	}

	return result;
}

int ShrineLogic::find(const char *needle, const char *typed) {
	const int needleLength = strlen(needle);
	const int typedLength = strlen(typed);

	if (needleLength > typedLength)
		return -1;

	const int lastStart = typedLength - needleLength;
	int pos = 0;

	do {
		int k;

		for (k = 0; k < needleLength; ++k) {
			int a = needle[k] & 0x7F;
			int b = (byte)typed[pos + k] & 0x7F;

			if (b > 0x60)
				b &= 0x5F;
			if (a > 0x60)
				a &= 0x5F;

			if (a != b)
				break;
		}

		if (k == needleLength)
			return pos;

		pos += k + 1;
	} while (pos <= lastStart);

	return -1;
}

int ShrineLogic::shrineAt(int x, int y) {
	for (int i = 0; i < Data::SHRINE_COUNT; ++i) {
		if (Data::SHRINE_X[i] == x && Data::SHRINE_Y[i] == y)
			return i;
	}

	return DEFAULT_SHRINE;
}

ShrineLogic::Stage ShrineLogic::stage(int shrine) {
	const Data::Savegame &sg = save();
	const int bit = 1 << shrine;

	if ((sg._shrineDone & bit) == 0)
		return STAGE_ORDAIN;

	return (sg._shrineActive & bit) == 0 ? STAGE_OFFER : STAGE_REWARD;
}

void ShrineLogic::ordain(int shrine) {
	save()._shrineActive |= 1 << shrine;
}

bool ShrineLogic::offer(int shrine, int hundreds) {
	Data::Savegame &sg = save();
	(void)shrine;

	if (sg._gold < hundreds * GOLD_PER_HUNDRED)
		return false;

	sg._gold -= hundreds * GOLD_PER_HUNDRED;
	sg._karma = MIN<int>(sg._karma + hundreds, KARMA_LIMIT);
	return true;
}

void ShrineLogic::reward(int shrine, Views::MessageLog &log) {
	Data::Savegame &sg = save();
	Data::Character &leader = sg._party[0];

	sg._shrineActive &= ~(1 << shrine);
	sg._karma = MIN<int>(sg._karma + REWARD_KARMA, KARMA_LIMIT);

	if (Data::SHRINE_STRENGTH[shrine] != 0) {
		leader._str = MIN<int>(leader._str + 1, STAT_LIMIT);
		log.print("Strength +1\n");
	}

	if (Data::SHRINE_DEXTERITY[shrine] != 0) {
		leader._dex = MIN<int>(leader._dex + 1, STAT_LIMIT);
		log.print("Dexterity +1\n");
	}

	if (Data::SHRINE_INTELLIGENCE[shrine] != 0) {
		leader._int = MIN<int>(leader._int + 1, STAT_LIMIT);
		log.print("Intelligence +1\n");
	}

	if (shrine == LAST_SHRINE)
		sg._karma = MIN<int>(sg._karma + REWARD_KARMA, KARMA_LIMIT);
}

ShrineLogic::Spoken ShrineLogic::speak(const char *typed, Views::MessageLog &log, int &shrine, int &x, int &y) {
	static const int8 DX[4] = { -1, 0, 1, 0 };
	static const int8 DY[4] = { 0, 1, 0, -1 };
	Data::Savegame &sg = save();

	for (int word = 0; word < Data::SHRINE_COUNT; ++word) {
		if (find(Data::SHRINE_WORDS[word], typed) == -1)
			continue;

		log.print("\nA word of power is uttered\n");

		for (int side = 0; side < 4; ++side) {
			const int tx = sg._mapX + DX[side];
			const int ty = sg._mapY + DY[side];
			const byte tile = _G(world).getTile(tx, ty);

			if (tile != Data::SHRINE_GATE_TILES[word] && tile != SEALED_TILE && tile != RUINS_TILE)
				continue;

			if (tile == RUINS_TILE) {
				shrine = word;
				x = tx;
				y = ty;
				return SPOKEN_RUINS;
			}

			if (tx == Data::DUNGEON_ENTRANCE_X[Data::SHRINE_COUNT * 4 + word] &&
					ty == Data::DUNGEON_ENTRANCE_Y[Data::SHRINE_COUNT * 4 + word]) {
				sg._unk032A[word] ^= 0x80;
				_G(world).setTile(tx, ty, tile ^ Data::SHRINE_GATE_TILES[word] ^ SEALED_TILE);
				_G(visibility).invalidate();
			}

			return SPOKEN_EFFECT;
		}

		break;
	}

	log.print("\nNo effect!\n");
	return SPOKEN_NOTHING;
}

void ShrineLogic::restore(int shrine, int x, int y) {
	save()._unk0332[shrine] &= 0x7F;
	_G(world).setTile(x, y, SHRINE_TILE);
	_G(visibility).invalidate();
}

int ShrineLogic::urnCount() {
	const Data::Savegame &sg = save();
	int count = 0;

	for (int i = 1; i < Data::PARTY_MAX; ++i) {
		if (sg._party[i]._mapId == LEFT_BEHIND_MAP)
			++count;
	}

	return count;
}

Common::String ShrineLogic::urnNames() {
	const Data::Savegame &sg = save();
	Common::String result;

	for (int i = 1; i < Data::PARTY_MAX; ++i) {
		if (sg._party[i]._mapId != LEFT_BEHIND_MAP)
			continue;

		Common::String name(sg._party[i]._name);
		name.toUppercase();
		result += name + "\n";
	}

	return result;
}

int ShrineLogic::codexVirtue() {
	const Data::Savegame &sg = save();

	for (int i = 0; i < Data::SHRINE_COUNT; ++i) {
		if ((sg._shrineActive & (1 << i)) != 0)
			return i;
	}

	return -1;
}

} // namespace Ultima5
} // namespace Ultima
