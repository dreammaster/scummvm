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

#include "wasteland/fod/logic/map_action.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

void MapAction::doMapAction(const uint16 *idPtr, int action, int charNum) {
	_idPtr = idPtr;
	_actionNum = action;
	_charNum = charNum;
	_partyNum = 0;

	while (_actionNum != -1 && _actionNum != 8) {
		mapActionInner(_idPtr, _actionNum, _partyNum, _charNum, 0);

		if (_redrawFlag && !_actionNum)
			g_engine->findView("Game")->draw();
	}
}

bool MapAction::mapActionInner(const uint16 *idPtr, int action, int partyNum, int charNum, int arg4) {
	const auto &disk = g_engine->_disk;
	uint16 id = *idPtr;
	_actionNum = -1;

	if (id == 0xffff)
		return false;

	// Check whether the script handles the particular action
	const byte *scriptP = &disk._scripts[disk._scriptsOffsets[id]];
	uint flags = READ_LE_UINT16(scriptP);
	if (!(flags & (1 << action)))
		return false;

	_val2 = 1;
	setPartyMember(partyNum);

	if (!_redrawFlag && action == 6)
		error("TODO: action 6");
	if (_redrawFlag)
		partyNum = g_engine->_disk1._party.getMemberByStatus(1);

	int local1 = 0, local2 = 0, local3 = 0;
	const byte *scriptCurrP;

	if (action == 4)
		local1 = arg4;

	scriptP += 4;
	scriptCurrP = scriptP;

	for (;;) {
		error("TODO: main loop");
	}
}

void MapAction::mapActionDone() {

}

void MapAction::setPartyMember(int partyNum) {
	_partyMember = &g_engine->_disk1._party[partyNum];
}


} // namespace Logic
} // namespace FOD
} // namespace Wasteland
