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

#include "ultima/ultima5/logic/transfer_logic.h"
#include "common/file.h"
#include "common/util.h"

namespace Ultima {
namespace Ultima5 {

constexpr int STATS_OFFSET = 8;
constexpr int VIRTUES_OFFSET = 0x140;
constexpr int VIRTUES_SKIP = 6;
constexpr int VIRTUE_COUNT = 8;
constexpr int NAME_FIELD = 16;
constexpr int MAX_NAME = 8;
constexpr int MAX_ATTRIBUTE = 0x46;
constexpr int MAX_POINTS = 9999;
constexpr int CLASS_COUNT = 8;
constexpr int MIN_STRENGTH = 0x14;
constexpr int HP_PER_LEVEL = 0x1E;
constexpr int MALE_CODE = 11;

static const char CLASS_LETTERS[CLASS_COUNT + 1] = "MBFDTPRS";
static const char *const CLASS_NAMES[CLASS_COUNT] = {
	"Mage", "Bard", "Fighter", "Druid", "Tinker", "Paladin", "Ranger", "Shepherd"
};

TransferLogic::LoadResult TransferLogic::load() {
	Common::File f;

	if (!f.open("party.sav") || f.size() < VIRTUES_OFFSET + VIRTUES_SKIP + VIRTUE_COUNT * 2)
		return LOAD_MISSING;

	f.seek(STATS_OFFSET);
	const int hp = f.readUint16LE();
	const int maxHp = f.readUint16LE();
	const int experience = f.readUint16LE();
	const int strength = f.readUint16LE();
	const int dexterity = f.readUint16LE();
	const int intellect = f.readUint16LE();
	f.readUint16LE();
	f.skip(6);

	byte name[NAME_FIELD];
	f.read(name, NAME_FIELD);
	const int gender = f.readByte();
	const int cls = f.readByte();

	if (strength > MAX_ATTRIBUTE || dexterity > MAX_ATTRIBUTE || intellect > MAX_ATTRIBUTE || experience > MAX_POINTS ||
			hp > MAX_POINTS || maxHp > MAX_POINTS || cls >= CLASS_COUNT)
		return LOAD_BAD;

	_old = Character();
	for (int i = 0; i < NAME_FIELD && name[i] != 0; ++i) {
		if (name[i] < 0x20)
			return LOAD_BAD;

		if (i < MAX_NAME)
			_old._name += (char)name[i];
	}

	_old._male = gender == MALE_CODE;
	_old._class = CLASS_LETTERS[cls];
	_old._strength = strength;
	_old._dexterity = dexterity;
	_old._intellect = intellect;
	_old._hp = hp;
	_old._maxHp = maxHp;
	_old._experience = experience;
	_old._level = maxHp / 100;

	// Whoever has gained all the virtues has all the eight values nought
	f.seek(VIRTUES_OFFSET + VIRTUES_SKIP);
	_old._avatar = true;
	for (int i = 0; i < VIRTUE_COUNT; ++i) {
		if (f.readUint16LE() != 0)
			_old._avatar = false;
	}

	_new = _old;
	return LOAD_OK;
}

int TransferLogic::convertAttribute(int value) {
	if (value < 10)
		return value;

	return value >= 30 ? (value - 30) / 4 + 20 : (value - 9) / 2 + 10;
}

const char *TransferLogic::className(char cls) {
	if (cls == 'A')
		return "Avatar";

	for (int i = 0; i < CLASS_COUNT; ++i) {
		if (CLASS_LETTERS[i] == cls)
			return CLASS_NAMES[i];
	}

	return "";
}

void TransferLogic::convertClass() {
	if (_old._avatar)
		_new._class = 'A';
}

void TransferLogic::convertExperience() {
	_new._experience = _old._experience / 10;
}

void TransferLogic::convertLevel() {
	_new._level = 1;

	for (int value = _new._experience / 100; value > 0; value >>= 1)
		++_new._level;

	_new._hp = _new._maxHp = _new._level * HP_PER_LEVEL;
}

void TransferLogic::convertStrength() {
	_new._strength = MAX(convertAttribute(_old._strength), MIN_STRENGTH);
}

void TransferLogic::convertDexterity() {
	_new._dexterity = convertAttribute(_old._dexterity);
}

void TransferLogic::convertIntellect() {
	_new._intellect = convertAttribute(_old._intellect);
}

void TransferLogic::apply(Data::Character &character) const {
	Common::strlcpy(character._name, _new._name.c_str(), sizeof(character._name));
	character._gender = _new._male ? Data::GENDER_MALE : Data::GENDER_FEMALE;
	character._class = _new._class;
	character._status = Data::STATUS_GOOD;
	character._str = _new._strength;
	character._dex = _new._dexterity;
	character._int = character._mag = _new._intellect;
	character._hp = _new._hp;
	character._maxHp = _new._maxHp;
	character._exp = _new._experience;
	character._level = _new._level;
}

} // namespace Ultima5
} // namespace Ultima
