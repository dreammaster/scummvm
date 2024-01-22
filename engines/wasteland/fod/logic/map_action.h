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

#ifndef WASTELAND_FOD_LOGIC_ACTION_H
#define WASTELAND_FOD_LOGIC_ACTION_H

#include "wasteland/fod/data/party.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

class MapAction {
private:
	const uint16 *_idPtr = nullptr;
	int _actionNum = 0;
	int _charNum = 0;
	int _partyNum = 0;
	bool _redrawFlag = false;
	int _val2 = 0;
	Data::PartyMember *_partyMember = nullptr;

	bool mapActionInner(const uint16 *idPtr, int action, int partyNum, int charNum, int arg4);
	void mapActionDone();
	void setPartyMember(int partyNum);

protected:
	void doMapAction(const uint16 *idPtr, int action, int arg2);
};

} // namespace Logic
} // namespace FOD
} // namespace Wasteland

#endif
