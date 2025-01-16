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

#ifndef WASTELAND_WASTELAND1_DATA_SAVED_H
#define WASTELAND_WASTELAND1_DATA_SAVED_H

#include "wasteland/core/serializer.h"
#include "wasteland/wasteland1/core/array1.h"
#include "wasteland/wasteland1/data/party_member.h"
#include "wasteland/wasteland1/data/party.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

/**
 * Sub-section of the saved data. In the original,
 * this gets separately copied
 */
struct Saved2 {
	byte _field0 = 0;
	byte _field1 = 0;
	byte _field2 = 0;
	byte _field3 = 0;
	byte _field4 = 0;
	byte _membersInGroup = 0;
	byte _activePartyGroup = 0;
	byte _field7 = 0;
	byte _field8 = 0;
	byte _totalPartyGroups = 0;
	byte _fieldA = 0;
	byte _fieldB = 0;
	byte _fieldC = 0;
	byte _fieldD = 0;

	void synchronize(Serializer &s);
};

/**
 * Returns party, roster, and other saved data
 */
struct Saved {
	Common::Array<Party> _parties;
	Array1<PartyMember> _roster;
	Common::String _saveLocationName;
	Saved2 _saved2;
	PartyMember *_currentCharacter = nullptr;

	Saved();
	void synchronize(Serializer &s);

	/**
	 * Loads the data from the original files
	 */
	void load();

	/**
	 * Sets the currently active/in-use character
	 */
	void setCurrentCharacter(int partyNum);
};

struct CurrentParty {
private:
	Saved &_saved;
public:
	CurrentParty(Saved &saved) : _saved(saved) {}

	size_t size() const;
	PartyMember *operator[](uint memberNum);
};

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland

#endif
