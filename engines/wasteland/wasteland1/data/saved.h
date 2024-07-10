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
 * Returns party, roster, and other saved data
 */
struct Saved {
	Array1<Party> _parties;
	Array1<PartyMember> _roster;
	Common::String _saveLocationName;

	Saved();
	void synchronize(Serializer &s);

	/**
	 * Loads the data from the original files
	 */
	void load();
};

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland

#endif
