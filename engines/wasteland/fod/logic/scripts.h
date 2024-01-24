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

#ifndef WASTELAND_FOD_LOGIC_SCRIPTS_H
#define WASTELAND_FOD_LOGIC_SCRIPTS_H

#include "wasteland/fod/data/party.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

typedef void (*ScriptDoneCallback)();

class Scripts {
	struct OpcodeParams {
		byte _action = 0;
		byte _opcode = 0;
		int _params[5];

		const int &operator[](uint i) const {
			return _params[i];
		}
	};
	typedef void(Scripts:: *OpcodeFunction)(const OpcodeParams &params);
private:
	static const OpcodeFunction OPCODE_FUNCTIONS[76];
	ScriptDoneCallback _doneCallback;
	const uint16 *_idPtr = nullptr;
	int _actionNum = 0;
	int _charNum = 0;
	int _partyNum = 0;
	int _newPartyNum = 0;
	bool _redrawFlag = false;
	int _val2 = 0;
	Data::PartyMember *_partyMember = nullptr;
	bool _breakFlag = false;

	bool mapActionInner(const uint16 *idPtr, int action, int partyNum, int charNum, int arg4);
	void mapActionDone();
	void setPartyMember(int partyNum);
	OpcodeParams readParams(const byte *&scriptP, int action);

public:
	void execute(const uint16 *idPtr, int action, int arg2,
		ScriptDoneCallback doneCallback);

private:
	void opcodeNOP(const OpcodeParams &) {}
	void opcode00(const OpcodeParams &);
	void opcode01(const OpcodeParams &);
	void opcode02(const OpcodeParams &);
	void opcode03(const OpcodeParams &);
	void opcode04(const OpcodeParams &);
	void opcode05(const OpcodeParams &);
	void opcode06(const OpcodeParams &);
	void opcode07(const OpcodeParams &);
	void opcode08(const OpcodeParams &);
	void opcode09(const OpcodeParams &);
	void opcode10(const OpcodeParams &);
	void opcode11(const OpcodeParams &);
	void opcode12(const OpcodeParams &);
	void opcode13(const OpcodeParams &);
	void opcode14(const OpcodeParams &);
	void opcode15(const OpcodeParams &);
	void opcode16(const OpcodeParams &);
	void opcode17(const OpcodeParams &);
	void opcode18(const OpcodeParams &);
	void opcode19(const OpcodeParams &);
	void opcode20(const OpcodeParams &);
	void opcode21(const OpcodeParams &);
	void opcode22(const OpcodeParams &);
	void opcode23(const OpcodeParams &);
	void opcode24(const OpcodeParams &);
	void opcode25(const OpcodeParams &);
	void opcode26(const OpcodeParams &);
	void opcode27(const OpcodeParams &);
	void opcode28(const OpcodeParams &);
	void opcode29(const OpcodeParams &);
	void opcode31(const OpcodeParams &);
	void opcode32(const OpcodeParams &);
	void opcode34(const OpcodeParams &);
	void opcode35(const OpcodeParams &);
	void opcode36(const OpcodeParams &);
	void opcode37(const OpcodeParams &);
	void opcode38(const OpcodeParams &);
	void opcode39(const OpcodeParams &);
	void opcode40(const OpcodeParams &);
	void opcode42(const OpcodeParams &);
	void opcode43(const OpcodeParams &);
	void opcode44(const OpcodeParams &);
	void opcode45(const OpcodeParams &);
	void opcode46(const OpcodeParams &);
	void opcode47(const OpcodeParams &);
	void opcode48(const OpcodeParams &);
	void opcode49(const OpcodeParams &);
	void opcode50(const OpcodeParams &);
	void opcode52_movePerson(const OpcodeParams &);
	void opcode53(const OpcodeParams &);
	void opcode54(const OpcodeParams &);
	void opcode55(const OpcodeParams &);
	void opcode56(const OpcodeParams &);
	void opcode57(const OpcodeParams &);
	void opcode58(const OpcodeParams &);
	void opcode59(const OpcodeParams &);
	void opcode60(const OpcodeParams &);
	void opcode61(const OpcodeParams &);
	void opcode62(const OpcodeParams &);
	void opcode63(const OpcodeParams &);
	void opcode64(const OpcodeParams &);
	void opcode65(const OpcodeParams &);
	void opcode66(const OpcodeParams &);
	void opcode67(const OpcodeParams &);
	void opcode68_End(const OpcodeParams &);
	void opcode69(const OpcodeParams &);
	void opcode72(const OpcodeParams &);
	void opcode73(const OpcodeParams &);
	void opcode74(const OpcodeParams &);
};

} // namespace Logic
} // namespace FOD
} // namespace Wasteland

#endif
