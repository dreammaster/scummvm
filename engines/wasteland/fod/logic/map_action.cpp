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
	&MapAction::opcode52,
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
	&MapAction::opcode68,
	&MapAction::opcode69,
	&MapAction::opcode17,

	&MapAction::opcode00,
	&MapAction::opcode72,
	&MapAction::opcode73,
	&MapAction::opcode74,
	&MapAction::opcode52
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
	int params[8];
	bool breakFlag = false;
	const byte *scriptCurrP;

	if (action == 4)
		local1 = arg4;

	scriptCurrP = scriptP + 4;
	for (;;) {
		for (;;) {
			if (breakFlag) {
				_val2 = 0;
				return true;
			}

			const byte *ptrNext = scriptP + READ_LE_UINT16(scriptP + 2);
			if (scriptCurrP > ptrNext) {
				_val2 = 0;
				return true;
			}

			readParams(scriptCurrP, params, action);
			if (params[0] != action)
				continue;

			int opcode = params[1];
			if (opcode > 75) {
				warning("Out of range opcode");
				continue;
			}

			(this->*OPCODE_FUNCTIONS[opcode])(params);
			error("TODO: main loop");
		}
	}
}

void MapAction::mapActionDone() {

}

void MapAction::setPartyMember(int partyNum) {
	_partyMember = &g_engine->_disk1._party[partyNum];
}

void MapAction::readParams(const byte *&scriptP, int *dest, int action) {
	int nextVal = *scriptP++;
	*dest++ = nextVal & 0xf;
	int paramsType = nextVal >> 4;

	if (!action) {
		*dest++ = *scriptP++;

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
}

void MapAction::opcode00(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode01(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode02(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode03(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode04(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode05(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode06(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode07(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode08(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode09(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode10(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode11(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode12(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode13(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode14(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode15(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode16(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode17(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode18(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode19(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode20(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode21(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode22(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode23(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode24(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode25(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode26(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode27(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode28(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode29(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode31(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode32(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode34(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode35(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode36(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode37(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode38(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode39(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode40(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode42(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode43(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode44(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode45(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode46(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode47(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode48(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode49(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode50(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode52(int params[]) {
	error("Unimplemented opcode");
}

void MapAction::opcode53(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode54(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode55(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode56(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode57(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode58(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode59(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode60(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode61(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode62(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode63(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode64(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode65(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode66(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode67(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode68(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode69(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode72(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode73(int params[]) { error("Unimplemented opcode"); }

void MapAction::opcode74(int params[]) { error("Unimplemented opcode"); }

} // namespace Logic
} // namespace FOD
} // namespace Wasteland
