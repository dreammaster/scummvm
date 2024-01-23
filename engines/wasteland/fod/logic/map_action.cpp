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
#include "wasteland/fod/data/map.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

enum { BYTE = 1, WORD = 2};
static const byte PARAMS1[] = { 0 };
static const byte PARAMS2[] = { BYTE, 0 };
static const byte PARAMS3[] = { BYTE, BYTE, 0 };
static const byte PARAMS4[] = { WORD, 0 };
static const byte PARAMS5[] = { BYTE, BYTE, BYTE, 0 };
static const byte PARAMS6[] = { BYTE, WORD, 0 };
static const byte PARAMS7[] = { BYTE, BYTE, BYTE, BYTE, 0 };
static const byte PARAMS8[] = { BYTE, BYTE, WORD, 0 };
static const byte PARAMS9[] = { WORD, WORD, 0 };
static const byte PARAMS10[] = { BYTE, BYTE, BYTE, WORD, 0 };
static const byte PARAMS11[] = { BYTE, BYTE, BYTE, WORD, WORD, 0 };
static const byte *const PARAMS[11] = {
	PARAMS1, PARAMS2, PARAMS3, PARAMS4, PARAMS5, PARAMS6,
	PARAMS7, PARAMS8, PARAMS9, PARAMS10, PARAMS11
};

const MapAction::OpcodeFunction MapAction::OPCODE_FUNCTIONS[76] = {
	&MapAction::opcode00,
	&MapAction::opcode01,
	&MapAction::opcode02,
	&MapAction::opcode03,
	&MapAction::opcode04,
	&MapAction::opcode05,
	&MapAction::opcode06,
	&MapAction::opcode07,
	&MapAction::opcode08,
	&MapAction::opcode09,

	&MapAction::opcode10,
	&MapAction::opcode11,
	&MapAction::opcode12,
	&MapAction::opcode13,
	&MapAction::opcode14,
	&MapAction::opcode15,
	&MapAction::opcode16,
	&MapAction::opcode17,
	&MapAction::opcode18,
	&MapAction::opcode19,

	&MapAction::opcode20,
	&MapAction::opcode21,
	&MapAction::opcode22,
	&MapAction::opcode23,
	&MapAction::opcode24,
	&MapAction::opcode25,
	&MapAction::opcode26,
	&MapAction::opcode27,
	&MapAction::opcode28,
	&MapAction::opcode29,
	&MapAction::opcode28,

	&MapAction::opcode31,
	&MapAction::opcode32,
	&MapAction::opcodeNOP,
	&MapAction::opcode34,
	&MapAction::opcode35,
	&MapAction::opcode36,
	&MapAction::opcode37,
	&MapAction::opcode38,
	&MapAction::opcode39,
	&MapAction::opcode40,

	&MapAction::opcode00,
	&MapAction::opcode42,
	&MapAction::opcode43,
	&MapAction::opcode44,
	&MapAction::opcode45,
	&MapAction::opcode46,
	&MapAction::opcode47,
	&MapAction::opcode48,
	&MapAction::opcode49,
	&MapAction::opcode50,

	&MapAction::opcodeNOP,
	&MapAction::opcode52_movePerson,
	&MapAction::opcode53,
	&MapAction::opcode54,
	&MapAction::opcode55,
	&MapAction::opcode56,
	&MapAction::opcode57,
	&MapAction::opcode58,
	&MapAction::opcode59,
	&MapAction::opcode60,

	&MapAction::opcode61,
	&MapAction::opcode62,
	&MapAction::opcode63,
	&MapAction::opcode64,
	&MapAction::opcode65,
	&MapAction::opcode66,
	&MapAction::opcode67,
	&MapAction::opcode68_End,
	&MapAction::opcode69,
	&MapAction::opcode17,

	&MapAction::opcode00,
	&MapAction::opcode72,
	&MapAction::opcode73,
	&MapAction::opcode74,
	&MapAction::opcode52_movePerson
};

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
	_newPartyNum = partyNum;

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
		_newPartyNum = g_engine->_disk1._party.getMemberByStatus(1);

	int local1 = 0;		//, local2 = 0, local3 = 0;
	_breakFlag = false;
	const byte *scriptCurrP;

	if (action == 4)
		local1 = arg4;

	scriptCurrP = scriptP + 4;
	for (;;) {
		for (;;) {
			if (_breakFlag) {
				_val2 = 0;
				return true;
			}

			const byte *ptrNext = scriptP + READ_LE_UINT16(scriptP + 2);
			if (scriptCurrP > ptrNext) {
				_val2 = 0;
				return true;
			}

			OpcodeParams params = readParams(scriptCurrP, action);
			if (params._action != action)
				continue;

			int opcode = params._opcode;
			if (opcode > 75) {
				warning("Out of range opcode");
				continue;
			}

			(this->*OPCODE_FUNCTIONS[opcode])(params);
		}
	}
}

void MapAction::mapActionDone() {

}

void MapAction::setPartyMember(int partyNum) {
	_partyMember = &g_engine->_disk1._party[partyNum];
}

MapAction::OpcodeParams MapAction::readParams(const byte *&scriptP, int action) {
	OpcodeParams result;
	int nextVal = *scriptP++;
	result._action = nextVal & 0xf;
	int paramsType = nextVal >> 4;

	if (!action) {
		result._opcode = *scriptP++;

		int *dest = result._params;
		// Iterate through loading parameters
		for (const byte *paramP = PARAMS[paramsType]; *paramP; ++paramP) {
			if (*paramP == BYTE) {
				*dest++ = *scriptP++;
			} else {
				*dest++ = READ_LE_INT16(scriptP);
				scriptP += 2;
			}
		}
	} else {
		// Skip
		static const byte ARRAY[47] = {
			4, 2, 1, 1, 3, 3, 2, 2, 0, 3, 2, 2, 4, 2, 0, 1,
			1, 3, 1, 4, 3, 2, 0, 1, 3, 2, 4, 2, 5, 7, 5, 1,
			2, 0, 3, 2, 2, 1, 0, 1, 0, 4, 2, 2, 2, 1, 0
		};

		int offset = ARRAY[*scriptP] + 1;
		scriptP += offset;	
	}

	return result;
}

void MapAction::opcode00(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode01(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode02(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode03(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode04(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode05(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode06(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode07(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode08(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode09(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode10(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode11(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode12(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode13(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode14(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode15(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode16(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode17(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode18(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode19(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode20(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode21(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode22(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode23(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode24(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode25(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode26(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode27(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode28(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode29(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode31(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode32(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode34(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode35(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode36(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode37(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode38(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode39(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode40(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode42(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode43(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode44(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode45(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode46(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode47(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode48(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode49(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode50(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode52_movePerson(const OpcodeParams &params) {
	int charNum;
	Data::Map &map = g_engine->_disk._map;
	Data::Map::MapPerson *person = map.findPersonById(params._params[0], &charNum);

	if (person && !(g_engine->_disk1._unknown4[person->_id] & 2)) {
		if (params._opcode == 52) {
			// Remove character from old position
			map.updateTileForeground(person->_mapX, person->_mapY, 0);

			// Move them to new position
			person->_mapX = params[1];
			person->_mapY = params[2];
			map.updateTile(person->_diff, person->_mapX, person->_mapY, person->_charNum);
		}

		person->_oldX = -1;
		map.flagMap();

		if (params[4] == 1 || params._opcode == 75) {
			if (params[4] == 1)
				person->_field50 |= 0x80;

			_idPtr = &person->_talkId;
			_actionNum = 6;
			_partyNum = _newPartyNum;
			_redrawFlag = false;
			_breakFlag = true;
		}
	}
}

void MapAction::opcode53(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode54(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode55(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode56(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode57(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode58(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode59(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode60(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode61(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode62(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode63(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode64(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode65(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode66(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode67(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode68_End(const OpcodeParams &params) {
	_breakFlag = true;
}

void MapAction::opcode69(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode72(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode73(const OpcodeParams &params) { error("Unimplemented opcode"); }

void MapAction::opcode74(const OpcodeParams &params) { error("Unimplemented opcode"); }

} // namespace Logic
} // namespace FOD
} // namespace Wasteland
