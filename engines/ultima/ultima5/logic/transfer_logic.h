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

#ifndef ULTIMA5_LOGIC_TRANSFER_LOGIC_H
#define ULTIMA5_LOGIC_TRANSFER_LOGIC_H

#include "common/scummsys.h"
#include "common/str.h"
#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Converts the character of an Ultima IV game into the Avatar of this one
 */
class TransferLogic {
public:
	enum LoadResult {
		LOAD_OK,
		LOAD_MISSING,
		LOAD_BAD
	};

	struct Character {
		Common::String _name;
		bool _male = true;
		char _class = 'F';
		int _strength = 0;
		int _dexterity = 0;
		int _intellect = 0;
		int _hp = 0;
		int _maxHp = 0;
		int _experience = 0;
		int _level = 0;
		bool _avatar = false;
	};

	// The character as it was in Ultima IV, and as it is becoming
	Character _old;
	Character _new;

	/**
	 * Reads the character from the Ultima IV party file that's in the game's folder
	 */
	LoadResult load();

	/**
	 * Gives how an attribute is changed from the scale of Ultima IV to that of this game
	 */
	static int convertAttribute(int value);

	/**
	 * Gives the name of a class
	 */
	static const char *className(char cls);

	// The steps of the conversion, each working out a part of the new character
	void convertClass();
	void convertExperience();
	void convertLevel();
	void convertStrength();
	void convertDexterity();
	void convertIntellect();

	/**
	 * Gives the new character to the party of a new game
	 */
	void apply(Data::Character &character) const;
};

} // namespace Ultima5
} // namespace Ultima

#endif
