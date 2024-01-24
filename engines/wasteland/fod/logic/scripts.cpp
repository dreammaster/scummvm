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

#include "wasteland/fod/logic/scripts.h"
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

const Scripts::OpcodeFunction Scripts::OPCODE_FUNCTIONS[76] = {
	&Scripts::opcode00,
	&Scripts::opcode01,
	&Scripts::opcode02,
	&Scripts::opcode03,
	&Scripts::opcode04,
	&Scripts::opcode05,
	&Scripts::opcode06,
	&Scripts::opcode07,
	&Scripts::opcode08,
	&Scripts::opcode09,

	&Scripts::opcode10,
	&Scripts::opcode11,
	&Scripts::opcode12,
	&Scripts::opcode13,
	&Scripts::opcode14,
	&Scripts::opcode15,
	&Scripts::opcode16,
	&Scripts::opcode17,
	&Scripts::opcode18,
	&Scripts::opcode19,

	&Scripts::opcode20,
	&Scripts::opcode21,
	&Scripts::opcode22,
	&Scripts::opcode23,
	&Scripts::opcode24,
	&Scripts::opcode25,
	&Scripts::opcode26,
	&Scripts::opcode27,
	&Scripts::opcode28,
	&Scripts::opcode29,
	&Scripts::opcode28,

	&Scripts::opcode31,
	&Scripts::opcode32,
	&Scripts::opcodeNOP,
	&Scripts::opcode34,
	&Scripts::opcode35,
	&Scripts::opcode36,
	&Scripts::opcode37,
	&Scripts::opcode38,
	&Scripts::opcode39,
	&Scripts::opcode40,

	&Scripts::opcode00,
	&Scripts::opcode42,
	&Scripts::opcode43,
	&Scripts::opcode44,
	&Scripts::opcode45,
	&Scripts::opcode46,
	&Scripts::opcode47,
	&Scripts::opcode48,
	&Scripts::opcode49,
	&Scripts::opcode50,

	&Scripts::opcodeNOP,
	&Scripts::opcode52_movePerson,
	&Scripts::opcode53,
	&Scripts::opcode54,
	&Scripts::opcode55,
	&Scripts::opcode56,
	&Scripts::opcode57,
	&Scripts::opcode58,
	&Scripts::opcode59,
	&Scripts::opcode60,

	&Scripts::opcode61,
	&Scripts::opcode62,
	&Scripts::opcode63,
	&Scripts::opcode64,
	&Scripts::opcode65,
	&Scripts::opcode66,
	&Scripts::opcode67,
	&Scripts::opcode68_End,
	&Scripts::opcode69,
	&Scripts::opcode17,

	&Scripts::opcode00,
	&Scripts::opcode72,
	&Scripts::opcode73,
	&Scripts::opcode74,
	&Scripts::opcode52_movePerson
};

void Scripts::execute(const uint16 *idPtr, int action, int charNum, ScriptDoneCallback doneCallback) {
	assert(doneCallback);
	_idPtr = idPtr;
	_actionNum = action;
	_charNum = charNum;
	_partyNum = 0;

	while (_actionNum != -1 && _actionNum != 8) {
		mapActionInner(_idPtr, _actionNum, _partyNum, _charNum, 0);

		if (_redrawFlag && !_actionNum)
			g_engine->findView("Game")->redraw();
	}

	doneCallback();
}

bool Scripts::mapActionInner(const uint16 *idPtr, int action, int partyNum, int charNum, int arg4) {
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
	const byte *ptrNext = scriptP + READ_LE_UINT16(scriptP + 2);

	while (!_breakFlag && scriptCurrP <= ptrNext) {
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

	_val2 = 0;
	return true;
}

void Scripts::mapActionDone() {

}

void Scripts::setPartyMember(int partyNum) {
	_partyMember = &g_engine->_disk1._party[partyNum];
}

Scripts::OpcodeParams Scripts::readParams(const byte *&scriptP, int action) {
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

void Scripts::opcode00(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode01(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode02(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode03(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode04(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode05(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode06(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode07(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode08(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode09(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode10(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode11(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode12(const OpcodeParams &params) {
	error("Unimplemented opcode");
}

void Scripts::opcode13(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode14(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode15(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode16(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode17(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode18(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode19(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode20(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode21(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode22(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode23(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode24(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode25(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode26(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode27(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode28(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode29(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode31(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode32(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode34(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode35(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode36(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode37(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode38(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode39(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode40(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode42(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode43(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode44(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode45(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode46(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode47(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode48(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode49(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode50(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode52_movePerson(const OpcodeParams &params) {
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

		if (params[3] == 1 || params._opcode == 75) {
			if (params[3] == 1)
				person->_field50 |= 0x80;

			_idPtr = &person->_talkId;
			_actionNum = 6;
			_partyNum = _newPartyNum;
			_redrawFlag = false;
			_breakFlag = true;
		}
	}
}

void Scripts::opcode53(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode54(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode55(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode56(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode57(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode58(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode59(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode60(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode61(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode62(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode63(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode64(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode65(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode66(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode67(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode68_End(const OpcodeParams &params) {
	_breakFlag = true;
}

void Scripts::opcode69(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode72(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode73(const OpcodeParams &params) { error("Unimplemented opcode"); }

void Scripts::opcode74(const OpcodeParams &params) { error("Unimplemented opcode"); }

} // namespace Logic
} // namespace FOD
} // namespace Wasteland
