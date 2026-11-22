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

#include "ultima/ultima5/logic/capture_logic.h"
#include "common/file.h"
#include "ultima/ultima5/data/shrine_data.h"
#include "ultima/ultima5/logic/location_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int MAX_MESSAGE = 400;
constexpr int SHRINE_COUNT = 8;
constexpr int LAST_QUESTION = 3;
constexpr int QUESTION_OFFSETS[LAST_QUESTION + 1] = { 0x00, 0x2D, 0x52, 0xAC };
constexpr int KARMA_LOSS = 5;
constexpr byte SHRINE_TAKEN = 0xFF;
constexpr byte LEFT_BEHIND_MAP = 0x7F;
constexpr int PALACE_MAP = 0x12;
constexpr int PALACE_X = 10;
constexpr int PALACE_Y = 7;

const byte CaptureLogic::SCRIPT_RELEASE[] = {
	0x13, 0x06, 0x44, 0x00, 0x04, 0x07, 0x11, 0x11, 0x02, 0x02, 0x01, 0x13, 0x1F, 0x12, 0x11, 0x11,
	0x10, 0x01, 0x13, 0x1F, 0x01, 0x13, 0x1C, 0x01, 0x10, 0x1C, 0x1C, 0x12, 0x06, 0xBB, 0x00, 0x04,
	0x08, 0x19, 0x02, 0x04, 0x01, 0x1A, 0x12, 0x1A, 0x11, 0x09, 0x08, 0x02, 0x03, 0x11, 0x12, 0x09,
	0x06, 0x02, 0x03, 0x17, 0x16, 0x09, 0x07, 0x02, 0x06, 0x08, 0x00, 0x00
};

const byte CaptureLogic::SCRIPT_THREAT[] = {
	0x05, 0x16, 0x12, 0x02, 0x07, 0x11, 0x22, 0x02, 0x02, 0x01, 0x13, 0x23, 0x22, 0x05, 0x03, 0x06,
	0x82, 0x05, 0x07, 0x09, 0x01, 0x07, 0x02, 0x05, 0x13, 0x10, 0x05, 0x0C, 0x02, 0x03, 0x17, 0x06,
	0xE9, 0x05, 0x09, 0x02, 0x03, 0x15, 0x00
};

const byte CaptureLogic::SCRIPT_ARRIVE[] = { 0x08, 0x01, 0x10, 0x14, 0x02, 0x03, 0x01, 0x13, 0x15, 0x05, 0x08, 0x00 };
const byte CaptureLogic::SCRIPT_ORDER[] = { 0x05, 0x0B, 0x13, 0x02, 0x04, 0x10, 0x11, 0x00 };
const byte CaptureLogic::SCRIPT_LEAVE[] = { 0x05, 0x04, 0x19, 0x02, 0x05, 0x1A, 0x09, 0x08, 0x07, 0x00 };

const byte CaptureLogic::CHAIN_ORDER[48] = {
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x05, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x05, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00
};

const byte CaptureLogic::CHAIN_X[6] = { 0x00, 0x01, 0x09, 0x0A, 0x03, 0x07 };
const byte CaptureLogic::CHAIN_Y[6] = { 0x01, 0x01, 0x01, 0x01, 0x05, 0x05 };

static Data::Savegame &save() {
	return _G(savegame);
}

Common::String CaptureLogic::message(int offset) {
	Common::String result;
	Common::File f;

	if (!f.open("miscmsg.dat") || !f.seek(offset))
		return result;

	for (int i = 0; i < MAX_MESSAGE && !f.eos(); ++i) {
		const byte ch = f.readByte();
		if (ch == 0)
			break;

		result += (char)ch;
	}

	return result;
}

int CaptureLogic::shrineToTake() {
	const Data::Savegame &sg = save();

	for (int i = 0; i < SHRINE_COUNT; ++i) {
		if (sg._unk0332[i] == 0)
			return i;
	}

	return -1;
}

Common::String CaptureLogic::question(int number, int shrine) {
	Common::String text = message(QUESTION_OFFSETS[number]);

	if (number < LAST_QUESTION) {
		text += Data::SHRINE_VIRTUES[shrine];
		text += "?\"";
	}

	return text;
}

bool CaptureLogic::isMantra(const Common::String &answer, int shrine) {
	Common::String mantra = Data::SHRINE_MANTRAS[shrine];
	Common::String said = answer;
	mantra.toUppercase();
	said.toUppercase();

	return said.contains(mantra);
}

void CaptureLogic::takeShrine(int shrine) {
	Data::Savegame &sg = save();

	sg._unk0332[shrine] = SHRINE_TAKEN;
	sg._karma = sg._karma > KARMA_LOSS ? sg._karma - KARMA_LOSS : 0;
}

int CaptureLogic::aliveCount() {
	const Data::Savegame &sg = save();
	int count = 0;

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status != Data::STATUS_DEAD)
			++count;
	}

	return count;
}

Common::String CaptureLogic::sacrifice() {
	Data::Savegame &sg = save();
	int index = 0;
	int alive = 0;

	for (; index < sg._partySize; ++index) {
		if (sg._party[index]._status != Data::STATUS_DEAD && ++alive == 2)
			break;
	}

	if (index >= sg._partySize)
		return Common::String();

	const Data::Character gone = sg._party[index];
	for (int i = index; i < Data::PARTY_MAX - 1; ++i)
		sg._party[i] = sg._party[i + 1];

	sg._party[Data::PARTY_MAX - 1] = gone;
	sg._party[Data::PARTY_MAX - 1]._mapId = LEFT_BEHIND_MAP;
	--sg._partySize;

	return gone._name;
}

void CaptureLogic::release() {
	Data::Savegame &sg = save();

	sg.clearObjects();
	sg._mapId = PALACE_MAP;
	sg._mapX = PALACE_X;
	sg._mapY = PALACE_Y;
	sg._keys = 0;

	LocationLogic::arrive();
}

} // namespace Ultima5
} // namespace Ultima
